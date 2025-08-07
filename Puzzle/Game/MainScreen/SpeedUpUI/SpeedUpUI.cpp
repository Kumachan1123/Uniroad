/*
*	@file SpeedUpUI.cpp
*	@brief スピードアップボタンクラス
*/
#include "pch.h"
#include "SpeedUpUI.h"
// インプットレイアウトの定義
const std::vector<D3D11_INPUT_ELEMENT_DESC>  SpeedUpUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// 表示位置（左上）の定義
const DirectX::SimpleMath::Vector2 SpeedUpUI::POSITION = DirectX::SimpleMath::Vector2(0.625f, 0.0f);
// 表示サイズの定義
const DirectX::SimpleMath::Vector2 SpeedUpUI::SIZE = DirectX::SimpleMath::Vector2(0.07f, 0.07f);

/*
*	@brief コンストラクタ
*	@details スピードアップUIの初期化を行う
*	@param なし
*	@return なし
*/
SpeedUpUI::SpeedUpUI()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_viewportWidth(0) // ビューポートの幅
	, m_viewportHeight(0) // ビューポートの高さ
	, m_pCreateShader(CreateShader::GetInstance()) // シェーダー作成クラス
	, m_pDrawPolygon(DrawPolygon::GetInstance()) // 描画クラス
	, m_isHit(false) // 当たり判定フラグ
	, m_isPressed(false) // ボタンが押されたフラグ
	, m_frameRows(2) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_pVertexShader(nullptr) // 頂点シェーダ
	, m_pPixelShader(nullptr) // ピクセルシェーダ
	, m_shaders{} // シェーダーの構造体
	, m_vertices{} // 頂点配列
	, m_constBuffer{} // コンスタントバッファ

{
}
/*
*	@brief デストラクタ
*	@details スピードアップUIの終了処理を行う
*	@param なし
*	@return なし
*/
SpeedUpUI::~SpeedUpUI()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details スピードアップUIの初期化を行う
*	@param resources 共通リソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void SpeedUpUI::Initialize(CommonResources* resources, int width, int height)
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
*	@brief シェーダーの作成
*	@details スピードアップUIのシェーダーを作成する
*	@param なし
*	@return なし
*/
void SpeedUpUI::CreateShaders()
{
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);
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

void SpeedUpUI::Update(float elapsedTime)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// 当たり判定を行う
	m_isHit = Hit(mousePos);
	if (m_isHit && MouseClick::IsLeftMouseButtonPressed(mouseState))
	{
		// マウスが当たったら押された状態をトグル
		m_isPressed = !m_isPressed;
	}
}

void SpeedUpUI::Render()
{
	const auto& keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	int frameIndex = 0;
	if (keyState.Space)frameIndex = 1;
	DrawQuad(m_pTextures, m_vertices, POSITION.x, POSITION.y, SIZE.x, SIZE.y, m_isPressed, m_frameCols, m_frameRows);
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("SpeedUpUI:Hit = %s", m_isHit ? "true" : "false");

}


void SpeedUpUI::DrawQuad(
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height,
	int frameIndex, int frameCols, int frameRows)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// アスペクト比を考慮してY方向サイズを補正
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = height * aspect;

	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + width, startY + correctedHeight));

	// 頂点座標の設定
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
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertices);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

bool SpeedUpUI::Hit(const DirectX::SimpleMath::Vector2& position)
{
	using namespace DirectX::SimpleMath;
	// 左上・右下をスクリーン座標へ変換
	// アスペクト比を考慮してY方向サイズを補正
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズ取得
	RECT rect;
	// クライアント領域サイズを取得
	GetClientRect(hwnd, &rect);
	float aspect = static_cast<float>(rect.right) / static_cast<float>(rect.bottom);
	float correctedHeight = SIZE.y * aspect;

	Vector2 ndcLT = (Vector2(POSITION.x, POSITION.y));
	Vector2 ndcRB = (Vector2(POSITION.x + SIZE.x, POSITION.y + correctedHeight));
	Vector2 leftTop = Vector2(ndcLT.x * rect.right, ndcLT.y * rect.bottom);
	Vector2 rightBottom = Vector2(ndcRB.x * rect.right, ndcRB.y * rect.bottom);
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("SpeedUpUI:MousePosition = (%.2f, %.2f)", position.x, position.y);
	debugString->AddString("SpeedUpUI:LeftTop = (%.2f, %.2f)", leftTop.x, leftTop.y);
	debugString->AddString("SpeedUpUI:RightBottom = (%.2f, %.2f)", rightBottom.x, rightBottom.y);
	// マウスの座標が画像の範囲内にあるならtrueを返す
	if (leftTop.x <= position.x && position.x <= rightBottom.x && leftTop.y <= position.y && position.y <= rightBottom.y)
		return true;
	// 当たり判定なしならfalseを返す
	return false;
}
