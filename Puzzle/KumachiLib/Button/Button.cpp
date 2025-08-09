/*
*	@file Button.cpp
*	@brief ボタンの大元となるクラス
*/
#include "pch.h"
#include "Button.h"
// インプットレイアウトの定義
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Button::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief コンストラクタ
*	@details ボタンの初期化を行う
*	@param なし
*	@return なし
*/
Button::Button()
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
*	@details ボタンの終了処理を行う
*	@param なし
*	@return なし
*/
Button::~Button()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details ボタンの初期化を行う
*	@param resources 共通リソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Button::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// ウィンドウの幅と高さを設定
	m_viewportWidth = width;
	m_viewportHeight = height;
	// 画像
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("SpeedUP"));
	// 描画クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// シェーダーの作成
	CreateShaders();
}
/*
*	@brief 更新
*	@details ボタンの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Button::Update(const float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
}

/*
*	@brief シェーダーの作成
*	@details ボタンのシェーダーを作成する
*	@param なし
*	@return なし
*/
void Button::CreateShaders()
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
*	@brief 画像を表示
*	@details ボタンの画像を表示する
*	@param startX 開始X座標
*	@param startY 開始Y座標
*	@param width 画像の幅
*	@param height 画像の高さ
*	@param frameIndex アニメーションのコマ番号
*	@param frameCols 画像の列数
*	@param frameRows 画像の行数
*	@return なし
*/
void Button::DrawQuad(float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
{
	// 名前空間の使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// アスペクト比を考慮してY方向サイズを補正
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = height * aspect;
	// NDC座標系に変換
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + width, startY + correctedHeight));
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
/*
*	@brief 当たり判定
*	@details マウスの座標とボタンの矩形を比較して当たり判定を行う
*	@param mousePosition マウスの座標
*	@param buttonRect ボタンの矩形
*	@return 当たり判定があればtrue、なければfalse
*/
bool Button::Hit(const DirectX::SimpleMath::Vector2& mousePosition, const ButtonRect& buttonRect)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// 左上・右下をスクリーン座標へ変換
	// アスペクト比を考慮してY方向サイズを補正
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズ取得
	RECT rect;
	// クライアント領域サイズを取得
	GetClientRect(hwnd, &rect);
	// アスペクト比を考慮してY方向サイズを補正
	float aspect = static_cast<float>(rect.right) / static_cast<float>(rect.bottom);
	float correctedHeight = buttonRect.size.y * aspect;
	// NDC座標系に変換
	Vector2 ndcLT = (Vector2(buttonRect.position.x, buttonRect.position.y));
	Vector2 ndcRB = (Vector2(buttonRect.position.x + buttonRect.size.x, buttonRect.position.y + correctedHeight));
	// 左上と右下の座標をスクリーン座標へ変換
	Vector2 leftTop = Vector2(ndcLT.x * rect.right, ndcLT.y * rect.bottom);
	Vector2 rightBottom = Vector2(ndcRB.x * rect.right, ndcRB.y * rect.bottom);
	// マウスの座標が画像の範囲内にあるならtrueを返す
	if (leftTop.x <= mousePosition.x && mousePosition.x <= rightBottom.x && leftTop.y <= mousePosition.y && mousePosition.y <= rightBottom.y)
		return true;
	// 当たり判定なしならfalseを返す
	return false;
}
