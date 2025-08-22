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
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, m_bufferSize);
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
