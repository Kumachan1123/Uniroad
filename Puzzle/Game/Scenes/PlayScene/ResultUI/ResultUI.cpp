/*
*	@file ResultUI.cpp
*	@brief 結果UIクラス
*/
#include "pch.h"
#include "ResultUI.h"
// 無効なメニューインデックス
const int ResultUI::INVALID_MENU_INDEX = -1;
// 通常の表示位置補正値(全画面基準）
const float ResultUI::DISPLAY_OFFSET = -400.0f;
// ボタンの移動にかかる時間
const float ResultUI::MOVE_DURATION = 0.3f;

/*
*	@brief コンストラクタ
*	@details 結果UIの初期化を行う
*	@param なし
*	@return なし
*/
ResultUI::ResultUI()
	: m_pDR(nullptr)// デバイスリソース
	, m_pCommonResources(nullptr) // 共通リソース
	, m_windowWidth(0)// ウィンドウの幅
	, m_windowHeight(0) // ウィンドウの高さ
	, m_prevWindowWidth(0) // 前のウィンドウの幅
	, m_prevWindowHeight(0) // 前のウィンドウの高さ
	, m_stageNum(0) // ステージ番号
	, m_buttonCount(0) // 生成したボタンの数
	, m_menuIndex(INVALID_MENU_INDEX) // メニューのインデックス
	, m_num(SceneID::NONE) // シーンID
	, m_time(0.0f) // 時間
	, m_hit(false) // ヒットフラグ
	, m_enable(false) // このクラスが有効かどうか
	, m_canPress(false) // ボタンが押せるかどうか
{
	// なし
}
/*
*	@brief デストラクタ
*	@details 結果UIの終了処理を行う
*	@param なし
*	@return なし
*/
ResultUI::~ResultUI()
{
	// なし
}
/*
*	@brief 初期化
*	@details 結果UIの初期化を行う
*	@param resources 共通リソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void ResultUI::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソース取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// ウィンドウサイズを設定
	m_windowHeight = height;
	m_windowWidth = width;
	// 進行度を初期化
	m_easeTimers.clear();
}
/*
*	@brief 更新
*	@details 結果UIの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void ResultUI::Update(const float elapsedTime)
{
	// 有効フラグが立っていない場合は何もしない
	if (!m_enable)return;
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 時間の更新
	m_time += elapsedTime;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 何かにヒットしたかどうかを初期化
	m_hit = false;
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズを設定
	GetScreenRect(hwnd, m_windowWidth, m_windowHeight);
	// 初回とボタン数変化時はリサイズし即座に座標を同期
	size_t btnCount = m_pButtons.size();
	// ボタンの数が変わった場合　
	if (m_prevPositions.size() != btnCount)
	{
		// 前の座標と目標座標、イージングタイマーをリサイズ
		m_prevPositions.resize(btnCount);
		m_targetPositions.resize(btnCount);
		m_easeTimers.resize(btnCount, 1.0f);
		// ボタンの数だけループ
		for (size_t i = 0; i < btnCount; ++i)
		{
			// ボタンの座標を初期化
			m_prevPositions[i] = m_pButtons[i]->GetPosition();
			// ボタンの目標座標を初期化
			m_targetPositions[i] = m_pButtons[i]->GetPosition();
			// イージングタイマーを初期化
			m_easeTimers[i] = 1.0f;
		}
	}
	// ウィンドウサイズが変わったかどうかをチェック
	bool resized = (m_windowWidth != m_prevWindowWidth) || (m_windowHeight != m_prevWindowHeight);
	// ウィンドウサイズが変わった場合はリサイズ処理を行う
	if (resized)
	{
		// リサイズ時の処理を呼び出す
		OnResize();
		// 前のウィンドウサイズを更新
		m_prevWindowWidth = m_windowWidth;
		m_prevWindowHeight = m_windowHeight;
		// リサイズ時はイージングせずreturn
		return;
	}

	// ボタンの数だけループ
	for (size_t i = 0; i < btnCount; ++i)
	{
		// ウィンドウサイズを設定
		m_pButtons[i]->SetWindowSize(m_windowWidth, m_windowHeight);
		// 目標座標決定
		Vector2 newTarget;
		// ウィンドウサイズに応じてボタンの位置とスケールを調整
		// ウィンドウモード
		if (m_windowWidth == Screen::WIDTH)
		{
			// ボタンの座標を更新
			if (m_pButtons.size() == 1)
				newTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				newTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET / 2) + (i * DISPLAY_OFFSET), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			// ボタンのスケールを更新
			m_pButtons[i]->SetScale(Vector2::One / 2.0f);
		}
		// 全画面
		else
		{
			// ボタンの座標を更新
			if (m_pButtons.size() == 1)
				newTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				newTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET)+(i * DISPLAY_OFFSET * 2), float(m_windowHeight / 2 - DISPLAY_OFFSET / 2.0f));
			// ボタンのスケールを更新
			m_pButtons[i]->SetScale(Vector2::One);
		}
		// 目標座標が変わったらイージングをリセット
		if ((m_targetPositions[i] - newTarget).LengthSquared() > 0.001f)
		{
			// 前の座標を更新
			m_prevPositions[i] = m_pButtons[i]->GetPosition();
			// 目標座標を更新
			m_targetPositions[i] = newTarget;
			// イージングタイマーをリセット
			m_easeTimers[i] = 0.0f;
			// ボタンを押せなくする
			m_canPress = false;
		}
		// イージングを適用
		if (m_easeTimers[i] < 1.0f)
		{
			// 経過時間をイージングタイマーに加算
			m_easeTimers[i] += elapsedTime / MOVE_DURATION;
			// イージングタイマーが1.0fを超えたら
			if (m_easeTimers[i] > 1.0f)
			{
				// イージングタイマーが1.0fを超えないように制限
				m_easeTimers[i] = 1.0f;
				// ボタンを押せるようにする
				m_canPress = true;
			}

		}
		// イージングを適用してボタンの位置を更新
		float t = Easing::EaseInCubic(m_easeTimers[i]);
		// 前の座標と目標座標を補間してボタンの位置を更新
		Vector2 pos = Vector2::Lerp(m_prevPositions[i], m_targetPositions[i], t);
		// ボタンの位置を設定
		m_pButtons[i]->SetPosition(pos);
		// 時間を加算
		m_pButtons[i]->SetTime(m_pButtons[i]->GetTime() + elapsedTime);
		// ボタンがヒットしたかどうかをチェック
		if (m_pButtons[i]->IsHit(mousePos) && m_canPress)
		{
			// ヒットフラグを立てる
			m_hit = true;
			// ボタンのスケールを更新
			m_pButtons[i]->SetScale(m_pButtons[i]->GetScale() * 1.2f);
			// ヒットしたメニューのインデックスを保存
			m_menuIndex = static_cast<int>(i);
			// ヒットしたらループを抜ける
			break;
		}
	}
	// ヒット無しなら選択インデックスを無効値に設定
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;
	// 左クリックされたら選択メニューのシーンIDを更新
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_canPress)
		m_num = static_cast<SceneID>(m_menuIndex);
}
/*
*	@brief 描画
*	@details 結果UIの描画を行う
*	@param なし
*	@return なし
*/
void ResultUI::Render()
{
	// 有効フラグが立っていない場合は何もしない
	if (!m_enable)return;
	// ボタンを一斉描画
	for (unsigned int i = 0; i < m_pButtons.size(); i++)m_pButtons[i]->Render();
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("ResultUI:Hit = %s", m_hit ? "true" : "false");
	debugString->AddString("ResultUI:MenuIndex = %d", m_menuIndex);
	debugString->AddString("ResultUI:CanPress = %s", m_canPress ? "true" : "false");
	for (unsigned int i = 0; i < m_easeTimers.size(); i++)
		debugString->AddString("ResultUI:EaseTimer[%d] = %f", i, m_easeTimers[i]);
}
/*
*	@brief ボタンを追加
*	@details 指定されたキーと位置、スケール、アンカーでUIを追加する
*	@param key 画像のキー
*	@param position 位置
*	@param scale スケール
*	@param anchor アンカー
*	@param type UIの種類
*	@return なし
*/
void ResultUI::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor,
	UIType type)
{
	using namespace DirectX::SimpleMath;
	// UIオブジェクトの生成
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// 指定画像でUI作成
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UIの種類に応じて保存
	if (type == UIType::BUTTON)m_pButtons.push_back(std::move(userInterface));
	// --- イージング用データも初期化 ---
	Vector2 pos = m_pButtons.back()->GetPosition();
	m_prevPositions.push_back(pos);   // 開始位置
	m_targetPositions.push_back(pos); // 目標位置
	m_easeTimers.push_back(0.0f);     // 最初は到達済み
}

/*
*	@brief シーンから結果を受け取る
*	@details ゲームオーバーやゲームクリアの結果を受け取り、UIの表示を切り替える
*	@param gameOver ゲームオーバーかどうか
*	@param gameClear ゲームクリアかどうか
*	@return なし
*/
void ResultUI::SetResult(bool gameOver, bool gameClear)
{
	UNREFERENCED_PARAMETER(gameOver); // ゲームオーバーは使用しない
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	using namespace KumachiLib;
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズを設定
	GetScreenRect(hwnd, m_windowWidth, m_windowHeight);
	// 前のウィンドウサイズにも同じ値を設定
	m_prevWindowHeight = m_windowHeight;
	m_prevWindowWidth = m_windowWidth;
	// ゲームクリアかつ今のステージ番号がステージ数以内なら次のステージに飛べるようにする
	if (gameClear)	m_stageNum++;
	// 収録しているステージの数を取得
	int fileCount = FileCounter::CountFilesInFolder("Resources/Map/", ".csv");
	// 初期座標を定義(ウィンドウモード基準)
	Vector2 initialPosition = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
	// 初期サイズを定義(ウィンドウモード基準)
	Vector2 initialScale = Vector2::One / 2.0f;
	// 全画面モードなら
	if (m_windowWidth == Screen::WIDTH_BUTTON)
	{
		// 初期座標を再定義
		initialPosition = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 2.0f);
		// 初期サイズを再定義
		initialScale = Vector2::One;
	}
	// 「ステージセレクトへ」ボタンを追加
	Add("ToStageSelect", initialPosition, initialScale, ANCHOR::MIDDLE_CENTER, UIType::BUTTON);
	// 設置したボタンをカウント
	m_buttonCount++;
	// 次のステージが存在していてゲームクリアなら
	if (m_stageNum < fileCount && gameClear)
	{
		// 「次のステージへ」ボタンを追加
		Add("ToNextStage", initialPosition, initialScale, ANCHOR::MIDDLE_CENTER, UIType::BUTTON);
		// 設置したボタンをカウント
		m_buttonCount++;
	}
	// ゲームオーバーなら
	else if (gameOver)
	{
		// 「リトライ」ボタンを追加
		Add("ReTry", initialPosition, initialScale, ANCHOR::MIDDLE_CENTER, UIType::BUTTON);
		// 設置したボタンをカウント
		m_buttonCount++;
	}
	// 最終ステージなら加算したステージ番号を戻す
	if (m_stageNum >= fileCount)
		m_stageNum--;
}

/*
*	@brief 画面リサイズ時の処理
*	@details 画面のリサイズに応じてUIの位置やサイズを調整する
*	@param なし
*	@return なし
*/
void ResultUI::OnResize()
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	using namespace DirectX::SimpleMath;
	size_t btnCount = m_pButtons.size();

	for (size_t i = 0; i < btnCount; ++i)
	{
		// ウィンドウサイズに応じて目標座標を再計算
		Vector2 snapTarget;
		// ウィンドウサイズが全画面モードなら
		if (m_windowWidth == Screen::WIDTH)
		{
			if (btnCount == 1)
				snapTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				snapTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET / 2) + (i * DISPLAY_OFFSET), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			m_pButtons[i]->SetScale(Vector2::One / 2.0f);
		}
		// ウィンドウサイズが全画面モードでないなら
		else
		{
			if (btnCount == 1)
				snapTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				snapTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET)+(i * DISPLAY_OFFSET * 2), float(m_windowHeight / 2 - DISPLAY_OFFSET / 2.0f));
			m_pButtons[i]->SetScale(Vector2::One);
		}
		// 前の座標と目標座標、イージングタイマーを更新
		m_prevPositions[i] = snapTarget;
		m_targetPositions[i] = snapTarget;
		m_easeTimers[i] = 1.0f;
		m_pButtons[i]->SetPosition(snapTarget);
	}
}