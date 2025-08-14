/*
*	@file TitleButton.cpp
*	@brief タイトルシーンのボタンを管理するクラスの実装ファイル
*/
#include "pch.h"
#include "TitleButton.h"
// 最終的な座標の定義
const std::vector<DirectX::SimpleMath::Vector2> TitleButton::POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.88f, 0.657f), // ゲーム開始ボタンの位置
	DirectX::SimpleMath::Vector2(0.88f, 0.8f), // 設定メニューボタンの位置
	DirectX::SimpleMath::Vector2(0.88f, 0.925f)  // ゲーム終了ボタンの位置
};
// 最終的なサイズの定義
const std::vector<DirectX::SimpleMath::Vector2> TitleButton::SIZES =
{
	DirectX::SimpleMath::Vector2(0.2f, 0.09f), // ゲーム開始ボタンのサイズ
	DirectX::SimpleMath::Vector2(0.2f, 0.07f), // 設定メニューボタンのサイズ
	DirectX::SimpleMath::Vector2(0.2f, 0.07f)  // ゲーム終了ボタンのサイズ
};
// 何も押されていない状態のインデックス
const int TitleButton::NONE_BUTTON_INDEX = -1;



/*
*	@brief コンストラクタ
*	@details タイトルシーンのボタンを管理するクラスのコンストラクタ
*	@param なし
*	@return なし
*/
TitleButton::TitleButton()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pStartButton(std::make_unique<Button>()) // ゲーム開始ボタンへのポインタ
	, m_pSettingButton(std::make_unique<Button>()) // 設定メニューボタンへのポインタ
	, m_pExitButton(std::make_unique<Button>()) // ゲーム終了ボタンへのポインタ
	, m_pAnimation(std::make_unique<Animation>()) // アニメーションへのポインタ
	, m_isPressed(false) // ボタンが押されたかどうか
	, m_isHit(false) // 当たり判定フラグ
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // ボタンの位置
	, m_size(DirectX::SimpleMath::Vector2(0.2f, 0.1f)) // ボタンのサイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_pressedButtonIndex(-1) // 押されたボタンの番号
	, m_hitButtonIndex(-1) // 当たったボタンの番号
{

}
/*
*	@brief デストラクタ
*	@details タイトルシーンのボタンを管理するクラスのデストラクタ
*	@param なし
*	@return なし
*/
TitleButton::~TitleButton()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details タイトルシーンのボタンを管理するクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void TitleButton::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// 画像を設定
	m_pStartButton->SetTexture(resources->GetTextureManager()->GetTexture("StartButton"));
	// ゲーム開始ボタンを追加
	m_buttons.push_back(std::move(m_pStartButton));
	// 画像を設定
	m_pSettingButton->SetTexture(resources->GetTextureManager()->GetTexture("SettingButton"));
	// 設定メニューボタンを追加
	m_buttons.push_back(std::move(m_pSettingButton));
	// 画像を設定
	m_pExitButton->SetTexture(resources->GetTextureManager()->GetTexture("GameEndButton"));
	// ゲーム終了ボタンを追加
	m_buttons.push_back(std::move(m_pExitButton));
	// ボタンの数ループ
	for (auto& button : m_buttons)
	{
		// シェーダーパスを渡す
		button->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
		button->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
		// シェーダーバッファサイズを設定
		button->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
		// ボタンの初期化
		button->Initialize(resources, width, height);
	}
	// ボタンの矩形を設定
	for (size_t i = 0; i < m_buttons.size(); i++)
	{
		// ボタンの位置とサイズを設定
		Rect buttonRect;
		buttonRect.position = POSITIONS[i];
		buttonRect.size = SIZES[i];
		// ボタンの位置とサイズを配列に登録
		m_buttonRects.push_back(buttonRect);
		// 当たり判定フラグを初期化
		m_isHit.push_back(false);
		// ホバー時の拡大率を初期化
		m_hoverScales.push_back(1.0f);
	}
	// アニメーションシーケンスを作成
	CreateAnimationSequence();
}
/*
*	@brief 更新
*	@details タイトルシーンのボタンを管理するクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleButton::Update(float elapsedTime)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// ホバー時の拡大率を定義
	const float SCALE_ON = 1.125f;
	// ホバーしていないときの拡大率を定義
	const float SCALE_OFF = 1.0f;
	// 補間係数
	const float SCALE_SPEED = 8.0f;
	// 当たったボタンの番号を初期化
	m_hitButtonIndex = -1;
	// ボタンの数ループ
	for (int i = 0; i < m_buttons.size(); i++)
	{
		// 当たり判定を行う
		m_isHit[i] = m_buttons[i]->Hit(mousePos, m_buttonRects[i]);
		// スケールのターゲット値
		float target = m_isHit[i] ? SCALE_ON : SCALE_OFF;
		// スムーズに補間
		m_hoverScales[i] += (target - m_hoverScales[i]) * (1.0f - expf(-SCALE_SPEED * elapsedTime));
	}
	// マウスが当たったボタンの番号を設定
	for (int i = 0; i < m_buttons.size(); i++)
		if (m_isHit[i])m_hitButtonIndex = i;
	// マウスが当たって左クリックされたら
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_pAnimation->IsPaused() && m_hitButtonIndex > NONE_BUTTON_INDEX)
	{
		// クリックで再開
		m_pAnimation->Resume();
		// アニメーションシーケンスを進める
		m_pAnimation->AdvanceSequence();
		// 押されたボタンの番号を設定
		m_pressedButtonIndex = m_hitButtonIndex;
		// ボタンが押されたフラグを立てる
		m_isPressed = true;
	}
	// アニメーションフェーズが3（移動中）で、アニメーションが一時停止していない場合は一時停止する
	if (m_pAnimation->GetAnimationPhase() == 2 && !m_pAnimation->IsPaused()) m_pAnimation->Pause();
	// アニメーションを更新
	m_pAnimation->Update(elapsedTime);
	// ボタンを更新
	for (const auto& button : m_buttons)button->Update(elapsedTime);
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 描画
*	@details タイトルシーンのボタンを管理するクラスの描画を行う
*	@param なし
*	@return なし
*/
void TitleButton::Render()
{
	// ボタンを描画
	for (size_t i = 0; i < m_buttons.size(); i++)
	{
		// 矩形の定義
		Rect rect = m_buttonRects[i];
		// スケール反映
		rect.size = SIZES[i] * m_hoverScales[i];
		// 描画
		m_buttons[i]->DrawQuadWithBuffer(rect, m_spriteSheetBuffer);
	}
#ifdef _DEBUG
	// デバッグ文字を描画
	const auto& debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("AnimationPhase:%i", m_pAnimation->GetAnimationPhase());
	debugString->AddString("AllAnimationSequenceCount:%i", m_pAnimation->GetAnimationSequenceCount());
	debugString->AddString("AnimationPaused:%s", m_pAnimation->IsPaused() ? "true" : "false");
	debugString->AddString("PressedButtonIndex:%i", m_pressedButtonIndex);
	debugString->AddString("HitButtonIndex:%i", m_hitButtonIndex);
#endif
}
/*
*	@brief アニメーションシーケンスを作成
*	@details タイトルシーンのボタンのアニメーションシーケンスを作成する
*	@param なし
*	@return なし
*/
void TitleButton::CreateAnimationSequence()
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// 各ボタンの遅延
	const std::vector<float> DELAYS = { 0.0f, 0.15f, 0.3f };
	// フェーズ1: 待機
	m_pAnimation->CreateAnimationSequence({
			2.5f,// 待機時間
			[this](float) {
			// 0で動かした場所とサイズで固定
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				m_buttonRects[i].position = Vector2(2.5f + i * 0.5f, POSITIONS[i].y);
			}
	} });
	// フェーズ2: 移動 
	const float DURATION = 1.0f; // 各ボタンの移動にかける時間
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + DURATION, // 全体の演出時間
		[this, DELAYS, DURATION](float globalT) {
			// globalTは0～1でシーケンス全体に対応する進行度
			float totalTime = globalT * (DELAYS.back() + DURATION); // 実際の経過秒数
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 各ボタンの進行度
				float t = (totalTime - DELAYS[i]) / DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// 補間
				m_buttonRects[i].position = Vector2::Lerp(Vector2(2.5f + i * 0.1f, POSITIONS[i].y), POSITIONS[i], easing);
			}
		}
		});
	// フェーズ3: 固定 
	m_pAnimation->CreateAnimationSequence({
			0.0f, // 無限
			[this](float) {
			// 最終静止位置とサイズに設定
			// ボタンの数ループ
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 最終的な位置に設定
				m_buttonRects[i].position = POSITIONS[i];
				// 最終的なサイズに設定
				m_buttonRects[i].size = SIZES[i];
			}
	} });
	// フェーズ4: 移動（元の場所へ）
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + DURATION, // 全体の演出時間
		[this, DELAYS, DURATION](float globalT) {
			// globalTは0～1でシーケンス全体に対応する進行度
			float totalTime = globalT * (DELAYS.back() + DURATION);
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 各ボタンの進行度
				float t = (totalTime - DELAYS[i]) / DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// 補間
				m_buttonRects[i].position = Vector2::Lerp(POSITIONS[i], Vector2(1.5f + i  , POSITIONS[i].y), easing);
			}
		}
		});
	// フェーズ5: 待機
	m_pAnimation->CreateAnimationSequence({
			0.0f, // 無限
			[this](float) {
			// 0で動かした場所とサイズで固定
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				m_buttonRects[i].position = Vector2(1.5f + i, POSITIONS[i].y);
				m_buttonRects[i].size = SIZES[i];
			}
	} });
}
/*
*	@brief 定数バッファを更新
*	@details タイトルシーンのボタンの定数バッファを更新する
*	@param なし
*	@return なし
*/
void TitleButton::UpdateConstantBuffer()
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
	m_spriteSheetBuffer.count = Vector4(0.0f);
	// 高さを設定
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// 幅を設定
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}
