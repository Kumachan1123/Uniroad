/*
*	@file BackButton.cpp
*	@brief 戻るボタンの処理を定義するクラス
*/
#include "pch.h"
#include "BackButton.h"
// 最終的な座標の定義
const std::vector<DirectX::SimpleMath::Vector2> BackButton::POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.0f, 0.99f), // ゲーム開始ボタンの位置
};
// 最終的なサイズの定義
const std::vector<DirectX::SimpleMath::Vector2> BackButton::SIZES =
{
	DirectX::SimpleMath::Vector2(0.4f, 0.18f), // ゲーム開始ボタンのサイズ
};
// 何も押されていない状態のインデックス
const int BackButton::NONE_BUTTON_INDEX = -1;

/*
*	@brief コンストラクタ
*	@details 戻るボタンの初期化を行う
*	@param なし
*	@return なし
*/
BackButton::BackButton()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pBackButton(std::make_unique<Button>()) // タイトルに戻るボタンへのポインタ
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
*	@details 戻るボタンのデストラクタ
*	@param なし
*	@return なし
*/
BackButton::~BackButton()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
	// 戻るボタンのポインタをリセット
	m_pBackButton.reset();
	// アニメーションのポインタをリセット
	m_pAnimation.reset();
	// ボタン配列をクリア
	m_buttons.clear();
	// ボタンの矩形をクリア
	m_buttonRects.clear();
	// 当たり判定フラグをクリア
	m_isHit.clear();
}
/*
*	@brief 初期化
*	@details 戻るボタンの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void BackButton::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// 画像を設定
	m_pBackButton->SetTexture(resources->GetTextureManager()->GetTexture("ToTitle"));
	// ゲーム終了ボタンを追加
	m_buttons.push_back(std::move(m_pBackButton));
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
*	@brief 更新する
*	@details 戻るボタンの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void BackButton::Update(float elapsedTime)
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
	for (int i = 0; i < m_buttons.size(); i++)	if (m_isHit[i])m_hitButtonIndex = i;
	// マウスが当たって左クリックされたら
	if (m_pAnimation->IsPaused() && m_hitButtonIndex > NONE_BUTTON_INDEX)
	{
		// 左クリックされたら
		if (MouseClick::IsLeftMouseButtonPressed(mouseState))
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
*	@brief 描画する
*	@details 戻るボタンの描画処理を行う
*	@param なし
*	@return なし
*/
void BackButton::Render()
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
*	@details 戻るボタンのアニメーションシーケンスを作成する
*	@param なし
*	@return なし
*/
void BackButton::CreateAnimationSequence()
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// 各ボタンの遅延
	const std::vector<float> DELAYS = { 0.0f, 0.15f, 0.3f };
	// フェーズ1: 待機
	m_pAnimation->CreateAnimationSequence({
			0.001f,// 待機時間
			[this](float) {
			// 0で動かした場所とサイズで固定
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				m_buttonRects[i].position = Vector2(-1.5f + i * 0.5f, POSITIONS[i].y + 0.0f);
			}
	} });
	// フェーズ2: 移動 
	// 各ボタンの移動にかける時間
	const float START_DURATION = 0.25f;
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + START_DURATION, // 全体の演出時間
		[this, DELAYS, START_DURATION](float globalT) {
			// globalTは0～1でシーケンス全体に対応する進行度
			float totalTime = globalT * (DELAYS.back() + START_DURATION); // 実際の経過秒数
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 各ボタンの進行度
				float t = (totalTime - DELAYS[i]) / START_DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// 補間
				m_buttonRects[i].position = Vector2::Lerp(Vector2(-1.5f + i * 0.1f, POSITIONS[i].y + 0.0f), POSITIONS[i], easing);
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
	// 各ボタンの移動にかける時間
	const float END_DURATION = 1.0f;
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + END_DURATION, // 全体の演出時間
		[this, DELAYS, END_DURATION](float globalT) {
			// globalTは0～1でシーケンス全体に対応する進行度
			float totalTime = globalT * (DELAYS.back() + END_DURATION);
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 各ボタンの進行度
				float t = (totalTime - DELAYS[i]) / END_DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// 補間
				m_buttonRects[i].position = Vector2::Lerp(POSITIONS[i], Vector2(-1.5f + i  , POSITIONS[i].y + 0.0f), easing);
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
				m_buttonRects[i].position = Vector2(-1.5f + i, POSITIONS[i].y + 0.0f);
				m_buttonRects[i].size = SIZES[i];
			}
	} });
}
/*
*	@brief 定数バッファを更新
*	@details 戻るボタンの定数バッファを更新する
*	@param なし
*	@return なし
*/
void BackButton::UpdateConstantBuffer()
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
