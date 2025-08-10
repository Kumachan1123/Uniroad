/*
*	@file SpeedUpUI.cpp
*	@brief スピードアップボタンクラス
*/
#include "pch.h"
#include "SpeedUpUI.h"
// 表示位置（左上）の定義
const DirectX::SimpleMath::Vector2 SpeedUpUI::POSITION = DirectX::SimpleMath::Vector2(0.665f, 0.07f);
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
	, m_isHit(false) // 当たり判定フラグ
	, m_isPressed(false) // ボタンが押されたフラグ
	, m_frameRows(2) // 画像の行数
	, m_frameCols(1) // 画像の列数
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
	// ボタンの作成
	m_pButton = std::make_unique<Button>();
	// シェーダーパスを渡す
	m_pButton->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	m_pButton->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// 画像を設定
	m_pButton->SetTexture(resources->GetTextureManager()->GetTexture("SpeedUP"));
	// ボタンの初期化
	m_pButton->Initialize(resources, width, height);
	// ボタンの矩形を設定
	m_buttonRect.position = POSITION;
	m_buttonRect.size = SIZE;
}
/*
*	@brief 更新
*	@details スピードアップUIの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void SpeedUpUI::Update(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// 当たり判定を行う
	m_isHit = m_pButton->Hit(mousePos, m_buttonRect);
	// マウスが当たって左クリックされたら押された状態をトグル
	if (m_isHit && MouseClick::IsLeftMouseButtonPressed(mouseState))m_isPressed = !m_isPressed;
}
/*
*	@brief 描画
*	@details スピードアップUIの描画を行う
*	@param なし
*	@return なし
*/
void SpeedUpUI::Render()
{
	// ボタンの描画
	m_pButton->DrawQuad(POSITION.x, POSITION.y, SIZE.x, SIZE.y, m_isPressed, m_frameCols, m_frameRows);
}
