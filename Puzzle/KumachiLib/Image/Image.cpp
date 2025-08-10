/*
*	@file Image.cpp
*	@brief 画像クラス
*/
#include "pch.h"
#include "Image.h"
// インプットレイアウトの定義
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Image::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief コンストラクタ
*	@details 画像クラスのコンストラクタ
*	@param なし
*	@return なし
*/
Image::Image()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_viewportWidth(0) // ビューポートの幅
	, m_viewportHeight(0) // ビューポートの高さ
	, m_pCreateShader(CreateShader::GetInstance()) // シェーダー作成クラス
	, m_pDrawPolygon(DrawPolygon::GetInstance()) // 描画クラス
	, m_pVertexShader(nullptr) // 頂点シェーダ
	, m_pPixelShader(nullptr) // ピクセルシェーダ
	, m_shaders{} // シェーダーの構造体
	, m_vertices{} // 頂点配列
	, m_constBuffer{} // コンスタントバッファ
{
}
/*
*	@brief デストラクタ
*	@details 画像クラスのデストラクタ
*	@param なし
*	@return なし
*/
Image::~Image()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details 画像クラスの初期化を行う
*	@param resources 共通リソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Image::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// ウィンドウの幅と高さを設定
	m_viewportWidth = width;
	m_viewportHeight = height;
	// 描画クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// シェーダーの作成
	CreateShaders();
}
/*
*	@brief シェーダーの作成
*	@details 画像のシェーダーを作成する
*	@param なし
*	@return なし
*/
void Image::CreateShaders()
{
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 文字列変換
	std::wstring wpathVS = std::wstring(m_vertexShaderFilePath.begin(), m_vertexShaderFilePath.end());
	std::wstring wpathPS = std::wstring(m_pixelShaderFilePath.begin(), m_pixelShaderFilePath.end());
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(wpathVS.c_str(), m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(wpathPS.c_str(), m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーをセット（使わないのでnullptr）
	m_shaders.gs = nullptr;
}

/*
*	@brief 更新
*	@details 画像の更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Image::Update(const float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// 何もしない
}
/*
*	@brief 画像を表示
*	@details 画像を表示する
*	@param buttonRect ボタンの矩形
*	@param frameIndex フレームのインデックス
*	@param frameCols フレームの列数
*	@param frameRows フレームの行数
*	@return なし
*/
void Image::DrawQuad(const Rect& buttonRect, int frameIndex, int frameCols, int frameRows)
{
	// 名前空間の使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// アスペクト比を考慮してY方向サイズを補正
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = buttonRect.size.y * aspect;
	// ボタンの矩形を設定
	float startX = buttonRect.position.x - buttonRect.size.x / 2.0f;
	float startY = buttonRect.position.y - correctedHeight / 2.0f;
	// NDC座標系に変換
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + buttonRect.size.x, startY + correctedHeight));
	// 頂点座標の設定
	VertexPositionTexture vertices[VERTEX_COUNT]{};
	vertices[0] = { VertexPositionTexture(Vector3(ndcLT.x, ndcLT.y, 0), Vector2(0, 0)) };// 左上
	vertices[1] = { VertexPositionTexture(Vector3(ndcRB.x, ndcLT.y, 0), Vector2(1, 0)) };// 右上
	vertices[2] = { VertexPositionTexture(Vector3(ndcRB.x, ndcRB.y, 0), Vector2(1, 1)) };// 右下
	vertices[3] = { VertexPositionTexture(Vector3(ndcLT.x, ndcRB.y, 0), Vector2(0, 1)) };// 左下
	// コンスタントバッファに渡すデータを設定
	// ワールド行列を単位行列に設定
	m_constBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_constBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_constBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_constBuffer.count = Vector4((float)(frameIndex));
	// 高さを設定
	m_constBuffer.height = Vector4((float)(frameRows));
	// 幅を設定
	m_constBuffer.width = Vector4((float)(frameCols));
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
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// デプスステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertices);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

