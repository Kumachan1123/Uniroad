/*
*	@file SpeedUpButton.cpp
*	@brief スピードアップボタンクラス
*/
#include "pch.h"
#include "SpeedUpButton.h"
/*
*	@brief コンストラクタ
*	@details スピードアップUIの初期化を行う
*	@param なし
*	@return なし
*/
SpeedUpButton::SpeedUpButton()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_isHit(false) // 当たり判定フラグ
	, m_isPressed(false) // ボタンが押されたフラグ
	, m_frameRows(2) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_position(POSITION) // 座標
	, m_size(SIZE) // サイズ
{
}
/*
*	@brief デストラクタ
*	@details スピードアップUIの終了処理を行う
*	@param なし
*	@return なし
*/
SpeedUpButton::~SpeedUpButton()
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
void SpeedUpButton::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// ボタンの作成
	m_pButton = std::make_unique<Button>();
	// 頂点シェーダーのパスを渡す
	m_pButton->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	// ピクセルシェーダーのパスを渡す	
	m_pButton->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// 画像を設定
	m_pButton->SetTexture(resources->GetTextureManager()->GetTexture("SpeedUP"));
	// シェーダーバッファサイズを設定
	m_pButton->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
	// ボタンの初期化
	m_pButton->Initialize(resources, width, height);
	// ボタンの矩形を設定
	// 座標
	m_buttonRect.position = m_position;
	// サイズ
	m_buttonRect.size = m_size;
}
/*
*	@brief 更新
*	@details スピードアップUIの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void SpeedUpButton::Update(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// 当たり判定を行う
	m_isHit = m_pButton->Hit(mousePos, m_buttonRect);
	// マウスが当たって左クリックされたら
	if (m_isHit && MouseClick::IsLeftMouseButtonPressed(mouseState))
	{
		// 押された状態をトグル
		m_isPressed = !m_isPressed;
		// スピードアップの効果音を再生(押された状態ならスピードアップ、そうでなければスピードダウン)
		if (m_isPressed) m_pCommonResources->GetAudioManager()->PlaySound("SpeedUp", m_pCommonResources->GetSettingManager()->GetSEVolume());
		// スピードダウンの効果音を再生
		else m_pCommonResources->GetAudioManager()->PlaySound("SpeedDown", m_pCommonResources->GetSettingManager()->GetSEVolume());
	}
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 描画
*	@details スピードアップUIの描画を行う
*	@param なし
*	@return なし
*/
void SpeedUpButton::Render()
{
	// ボタンの描画
	m_pButton->DrawQuadWithBuffer(m_buttonRect, m_spriteSheetBuffer);
}
/*
*	@brief 定数バッファを更新
*	@details スピードアップUIの定数バッファを更新する
*	@param なし
*	@return なし
*/
void SpeedUpButton::UpdateConstantBuffer()
{
	// 名前空間を使用
	using namespace DirectX::SimpleMath;
	// 定数バッファを更新
	// ワールド行列を単位行列に設定
	m_spriteSheetBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_spriteSheetBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_spriteSheetBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_spriteSheetBuffer.count = Vector4((float)(m_isPressed));
	// 高さを設定
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// 幅を設定
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}
