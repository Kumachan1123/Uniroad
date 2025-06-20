/*
*	@file Canvas.cpp
*	@brief キャンバスクラス
*/
#include <pch.h>
#include "Canvas.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> Canvas::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief	コンストラクタ
*	@details キャンバスクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Canvas::Canvas(CommonResources* pCommonResources)
	: m_pCommonResources(pCommonResources)
	, m_pDR(nullptr)
	, m_textureWidth(0)
	, m_textureHeight(0)
	, m_windowWidth(0)
	, m_windowHeight(0)
	, m_position(0.0f, 0.0f)
	, m_scale(1.0f, 1.0f)
	, m_baseScale(1.0f, 1.0f)
	, m_anchor(KumachiLib::ANCHOR::TOP_LEFT)
	, m_shaderType(ShaderType::OTHER)
	, m_pTexture(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pGeometryShader(nullptr)
	, m_pInputLayout(nullptr)
	, m_pCBuffer(nullptr)
	, m_pDrawPolygon(DrawPolygon::GetInstance())
	, m_pCreateShader(CreateShader::GetInstance())
{
}
/*
*	@brief	デストラクタ
*	@details 何もしない
*	@param なし
*	@return なし
*/
Canvas::~Canvas(/*do nothing*/)
{
}
/*
*	@brief	テクスチャの読み込み
*	@details テクスチャの読み込み
*	@param key テクスチャのキー
*	@return なし
*/
void Canvas::LoadTexture(std::string key)
{
	// テクスチャマネージャーからテクスチャを取得
	m_pTexture = m_pCommonResources->GetTextureManager()->GetTexture(key);
	// サイズ取得のための準備
	// 一時保存用リソースハンドル
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	// テクスチャからリソースを取得
	m_pTexture->GetResource(resource.GetAddressOf());
	// 一時保存用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	// リソースをテクスチャに変換
	DX::ThrowIfFailed(resource.As(&texture2D));
	// テクスチャの情報を取得
	// テクスチャの情報を格納する構造体
	D3D11_TEXTURE2D_DESC desc;
	// テクスチャの情報を取得
	texture2D->GetDesc(&desc);
	// テクスチャを配列に追加
	m_pTextures.push_back(m_pTexture.Get());
	// テクスチャの幅を取得
	m_textureWidth = desc.Width;
	// テクスチャの高さを取得
	m_textureHeight = desc.Height;
}
/*
*	@brief	UIの生成
*	@details UIの生成
*	@param pDR デバイスリソース
*	@param key テクスチャのキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void Canvas::Create(DX::DeviceResources* pDR, const std::string& key, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor)
{
	// デバイスリソース
	m_pDR = pDR;
	// 位置
	m_position = position;
	// スケール
	m_baseScale = m_scale = scale;
	// アンカー
	m_anchor = anchor;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// テクスチャを読み込む
	LoadTexture(key);
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief	描画関数
*	@details 板ポリゴンを描画する
*	@param なし
*	@return なし
*/
void Canvas::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点情報
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))// 大きさとアンカー
		, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))// 位置と幅と高さ
		, Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))// ウィンドウの幅と高さ
	};
	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// ウィンドウサイズを設定
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	// 時間を設定
	m_constBuffer.time = m_time;
	// 色を設定
	m_constBuffer.color = Vector3(0.5, 0.5, 0.5);
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// 深度ステンシルステート
	// 描画開始
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	////	シェーダをセットする

	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief	シェーダーの作成
*	@details 各種シェーダーを作成
*	@param なし
*	@return なし
*/
void Canvas::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Tile/VS_Tile.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Tile/PS_Tile.cso", m_pPixelShader);
	// ジオメトリシェーダーの作成
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Tile/GS_Tile.cso", m_pGeometryShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にジオメトリシェーダーを渡す
	m_shaders.gs = m_pGeometryShader.Get();
	// シェーダーの構造体にピクセルシェーダーを渡す
	m_shaders.ps = m_pPixelShader.Get();
}
/*
*	@brief	ウィンドウのサイズを設定
*	@details ウィンドウのサイズを設定
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Canvas::SetWindowSize(const int& width, const int& height)
{
	// ウィンドウの幅
	m_windowWidth = width;
	// ウィンドウの高さ
	m_windowHeight = height;
}
/*
*	@brief	当たり判定
*	@details マウスの座標がUIの範囲内にあるかを判定する
*	@param pos マウスの座標
*	@return true:当たり判定あり, false:当たり判定なし
*/
bool Canvas::IsHit(const DirectX::SimpleMath::Vector2& pos) const
{
	// 画像の左上の座標を取得
	DirectX::SimpleMath::Vector2 leftTop = m_position - DirectX::SimpleMath::Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.x / 2;
	// 画像の右下の座標を取得
	DirectX::SimpleMath::Vector2 rightBottom = m_position + DirectX::SimpleMath::Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.y / 2;
	// マウスの座標が画像の範囲内にあるならtrueを返す
	if (leftTop.x <= pos.x && pos.x <= rightBottom.x && leftTop.y <= pos.y && pos.y <= rightBottom.y)return true;
	// 当たり判定なしならfalseを返す
	return false;
}