/*
*	@file UIBack.cpp
*	@brief 操作画面の背景クラス
*/
#include <pch.h>
#include "UIBack.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC>  UIBack::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details 操作画面の背景のコンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
UIBack::UIBack(CommonResources* resources)
	: m_pDR(nullptr)// デバイスリソース
	, m_time(0.0f)// 経過時間
	, m_constBuffer()// コンスタントバッファ
	, m_pCommonResources{ resources }// 共通リソースへのポインタ
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	using namespace DirectX::SimpleMath;
	m_constBuffer.colors = Vector4(1.0f, 1.0f, 0.0f, 1.0f);// 色の初期化
}
/*
*	@brief	デストラクタ
*	@details 操作画面の背景のデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
UIBack::~UIBack()
{
	/*do nothing*/
}

void UIBack::Create(DX::DeviceResources* pDR)
{
	// デバイスリソースをセット
	m_pDR = pDR;
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
	// シェーダーの作成
	CreateShaders();
	// マネージャーからテクスチャを取得
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("UI_Back"));
}

void UIBack::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 時間更新 
	m_time += elapsedTime;
}

void UIBack::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	頂点情報 
	VertexPositionTexture vertex[4] =
	{
		//	頂点情報													UV情報
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),  Vector2(0.0f, 0.0f)),// 左上
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f),   Vector2(1.0f, 0.0f)),// 右上
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),    Vector2(1.0f, 1.0f)),// 右下
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f),   Vector2(0.0f, 1.0f)),// 左下
	};
	// シェーダーに渡す追加のバッファを作成する(ConstBuffer)
	// ビュー行列を転置して格納
	m_constBuffer.matView = m_view.Transpose();
	// プロジェクション行列を転置して格納
	m_constBuffer.matProj = m_proj.Transpose();
	// ワールド行列を転置して格納
	m_constBuffer.matWorld = m_world.Transpose();
	// 時間を格納
	m_constBuffer.time = Vector4(m_time);
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
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertex);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

void UIBack::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Radar/VS_Radar.cso", m_pVertexShader);
	// ピクセルシェーダーの作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Radar/PS_Radar.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// ジオメトリシェーダーは使用しないのでnullptrをセット
	m_shaders.gs = nullptr;
}
