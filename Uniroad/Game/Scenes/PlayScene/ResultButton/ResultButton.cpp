/*
*	@file	ResultButton.cpp
*	@brief	リザルト画面のボタンを管理するクラスの実装ファイル
*/
#include "pch.h"
#include "ResultButton.h"
// 最終的な座標の定義
const std::vector<DirectX::SimpleMath::Vector2> ResultButton::POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.700f, 0.657f), // リトライ/次のステージへ進むボタンの位置
	DirectX::SimpleMath::Vector2(0.300f, 0.657f), // ステージセレクトに進むボタンの位置 
};
// 最終的なサイズの定義
const std::vector<DirectX::SimpleMath::Vector2> ResultButton::SIZES =
{
	DirectX::SimpleMath::Vector2(0.3f, 0.105f),  // リトライ/次のステージへ進むボタンのサイズ
	DirectX::SimpleMath::Vector2(0.3f, 0.105f),// ステージセレクトに進むボタンのサイズ 
};


/*
*	@brief コンストラクタ
*	@details リザルト画面のボタンを管理するクラスのコンストラクタ
*	@param なし
*	@return なし
*/
ResultButton::ResultButton()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pRetryNextStageButton(std::make_unique<Button>()) // リトライ/次のステージへ進むボタン
	, m_pToStageSelectButton(std::make_unique<Button>()) // ステージセレクトに進むボタン
	, m_pAnimation(std::make_unique<Animation>()) // アニメーションへのポインタ
	, m_isPressed(false) // ボタンが押されたかどうか
	, m_isHit(false) // 当たり判定フラグ
	, m_prevIsHit(false) // 前フレームの当たり判定フラグ
	, m_enable(false) // このクラスが有効かどうか
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // ボタンの位置
	, m_size(DirectX::SimpleMath::Vector2(0.2f, 0.1f)) // ボタンのサイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_pressedButtonIndex(-1) // 押されたボタンの番号
	, m_hitButtonIndex(-1) // 当たったボタンの番号
	, m_gameOver(false) // ゲームオーバーかどうか
	, m_gameClear(false) // ゲームクリアかどうか
	, m_stageNum(0) // 今のステージ番号
	, m_num(NONE) // シーンID
{
}
/*
*	@brief デストラクタ
*	@details リザルト画面のボタンを管理するクラスのデストラクタ
*	@param なし
*	@return なし
*/
ResultButton::~ResultButton()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details リザルト画面のボタンを管理するクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void ResultButton::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// 次のステージへ進ませる
	if (m_gameClear)	m_stageNum++;
	// 画像を設定
	m_pToStageSelectButton->SetTexture(resources->GetTextureManager()->GetTexture("ToStageSelect"));
	// 設定メニューボタンを追加
	m_buttons.push_back(std::move(m_pToStageSelectButton));
	// 収録しているステージの数を取得
	int fileCount = FileCounter::CountFilesInFolder("Resources/Map/", ".csv");
	// 画像を設定
	if (m_stageNum < fileCount)
	{
		// ゲームオーバーの時はリトライ/次のステージへ進むボタンのテクスチャを設定
		if (m_gameOver)m_pRetryNextStageButton->SetTexture(resources->GetTextureManager()->GetTexture("ReTry"));
		// ゲームクリアの時はリトライ/次のステージへ進むボタンのテクスチャを設定
		else if (m_gameClear)m_pRetryNextStageButton->SetTexture(resources->GetTextureManager()->GetTexture("ToNextStage"));
		// ゲーム開始ボタンを追加
		m_buttons.push_back(std::move(m_pRetryNextStageButton));
	}
	// ボタンの数ループ
	for (auto& button : m_buttons)
	{
		// 頂点シェーダーのパスを渡す
		button->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
		// ピクセルシェーダーのパスを渡す
		button->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
		// シェーダーバッファサイズを設定
		button->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
		// ボタンの初期化
		button->Initialize(resources, width, height);
	}
	// ボタンの矩形を設定
	for (size_t i = 0; i < m_buttons.size(); i++)
	{
		// ボタンの矩形を定義
		Rect buttonRect;
		// 座標を設定
		buttonRect.position = POSITIONS[i];
		// サイズを設定
		buttonRect.size = SIZES[i];
		// ボタンの位置とサイズを配列に登録
		m_buttonRects.push_back(buttonRect);
		// 当たり判定フラグを初期化
		m_isHit.push_back(false);
		// 前フレームの当たり判定フラグを初期化
		m_prevIsHit.push_back(false);
		// ホバー時の拡大率を初期化
		m_hoverScales.push_back(1.0f);
	}
	// アニメーションシーケンスを作成
	CreateAnimationSequence();
	// 最終ステージなら加算したステージ番号を戻す
	if (m_stageNum >= fileCount) m_stageNum--;
}
/*
*	@brief 更新
*	@details リザルト画面のボタンを管理するクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void ResultButton::Update(float elapsedTime)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// 有効フラグが立っていない場合は何もしない
	if (!m_enable)return;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// 当たったボタンの番号を初期化
	m_hitButtonIndex = NONE_BUTTON_INDEX;
	// ボタンの数ループ
	for (int i = 0; i < m_buttons.size(); i++)
	{
		// 当たり判定を行う
		m_isHit[i] = m_buttons[i]->Hit(mousePos, m_buttonRects[i]);
		// ホバーした瞬間だけ音を鳴らす
		if (!m_prevIsHit[i] && m_isHit[i])
			m_pCommonResources->GetAudioManager()->PlaySound("UISelect", m_pCommonResources->GetSettingManager()->GetSEVolume());

		// スケールのターゲット値
		float target = m_isHit[i] ? SCALE_ON : SCALE_OFF;
		// スムーズに補間
		m_hoverScales[i] += (target - m_hoverScales[i]) * (1.0f - expf(-SCALE_SPEED * elapsedTime));
	}
	// ループの後で前フレーム記録を更新）
	m_prevIsHit = m_isHit;
	// マウスが当たったボタンの番号を設定
	for (int i = 0; i < m_buttons.size(); i++)if (m_isHit[i])m_hitButtonIndex = i;
	// マウスが当たって左クリックされたら
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_pAnimation->IsPaused() && m_hitButtonIndex > NONE_BUTTON_INDEX)
	{
		// クリックで再開
		m_pAnimation->Resume();
		// アニメーションシーケンスを進める
		m_pAnimation->AdvanceSequence();
		// 押されたボタンの番号を設定
		m_pressedButtonIndex = m_hitButtonIndex;
		// 移動するシーンのIDに変換
		m_num = (static_cast<SceneID>(m_pressedButtonIndex));
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
*	@brief 画像を表示
*	@details リザルト画面のボタンを管理するクラスの画像を表示する
*	@param なし
*	@return なし
*/
void ResultButton::Render()
{
	// 有効フラグが立っていない場合は何もしない
	if (!m_enable)return;
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
	// アニメーションフェーズを表示
	debugString->AddString("AnimationPhase:%i", m_pAnimation->GetAnimationPhase());
	// アニメーションシーケンス数を表示
	debugString->AddString("AllAnimationSequenceCount:%i", m_pAnimation->GetAnimationSequenceCount());
	// アニメーションの停止状態を表示
	debugString->AddString("AnimationPaused:%s", m_pAnimation->IsPaused() ? "true" : "false");
	// 押されたボタンの番号を表示
	debugString->AddString("PressedButtonIndex:%i", m_pressedButtonIndex);
	// 当たったボタンの番号を表示
	debugString->AddString("HitButtonIndex:%i", m_hitButtonIndex);
#endif
}
/*
*	@brief アニメーションシーケンスを作成
*	@details リザルト画面のボタンのアニメーションシーケンスを作成する
*	@param なし
*	@return なし
*/
void ResultButton::CreateAnimationSequence()
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// 各ボタンの遅延
	const std::vector<float> DELAYS = { 0.0f,0.0f };
	// フェーズ1: 待機
	m_pAnimation->CreateAnimationSequence({
			ANIMATION_WAIT_TIME,// 待機時間
			[this](float) {
			// 0で動かした場所とサイズで固定
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 最初の位置に設定
				m_buttonRects[i].position = Vector2(POSITIONS[i].x, ANIMATION_START_Y + i * ANIMATION_MOVE_OFFSET_1);
			}
	} });
	// フェーズ2: 移動 
	// 各ボタンの移動にかける時間
	const float DURATION = 1.0f;
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + DURATION, // 全体の演出時間
		[this, DELAYS, DURATION](float globalT) {
			// globalTは0～1でシーケンス全体に対応する進行度
			float totalTime = globalT * (DELAYS.back() + DURATION); // 実際の経過秒数
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 各ボタンの進行度
				float t = (totalTime - DELAYS[i]) / DURATION;
				// Clampで0～1に制限
				t = Clamp(t, 0.0f, 1.0f);
				// イージング計算
				float easing = Easing::EaseInOutCubic(t);
				// 補間
				m_buttonRects[i].position = Vector2::Lerp(Vector2(POSITIONS[i].x, ANIMATION_START_Y + i * ANIMATION_MOVE_OFFSET_2), POSITIONS[i], easing);
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
			// 各ボタンの数ループ
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// 各ボタンの進行度
				float t = (totalTime - DELAYS[i]) / DURATION;
				// Clampで0～1に制限
				t = Clamp(t, 0.0f, 1.0f);
				// イージング計算
				float easing = Easing::EaseInOutCubic(t);
				// 補間
				m_buttonRects[i].position = Vector2::Lerp(POSITIONS[i], Vector2(POSITIONS[i].x, ANIMATION_END_Y + i), easing);
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
				// 最終的な位置に設定
				m_buttonRects[i].position = Vector2(POSITIONS[i].x, ANIMATION_END_Y + i);
				// サイズを設定
				m_buttonRects[i].size = SIZES[i];
			}
	} });
}
/*
*	@brief 定数バッファを更新
*	@details リザルト画面のボタンを管理するクラスの定数バッファを更新する
*	@param なし
*	@return なし
*/
void ResultButton::UpdateConstantBuffer()
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
/*
*	@brief シーンから結果を受け取る
*	@details シーンからゲームオーバーかゲームクリアかの結果を受け取る
*	@param gameOver ゲームオーバーかどうか
*	@param gameClear ゲームクリアかどうか
*	@return なし
*/
void ResultButton::SetResult(bool gameOver, bool gameClear)
{
	//各フラグを受け取る
	m_gameOver = gameOver;
	m_gameClear = gameClear;
}