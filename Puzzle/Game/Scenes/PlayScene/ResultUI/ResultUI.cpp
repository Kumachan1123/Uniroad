/*
*	@file ResultUI.cpp
*	@brief 結果UIクラス
*/
#include "pch.h"
#include "ResultUI.h"
// 無効なメニューインデックス
const int ResultUI::INVALID_MENU_INDEX = -1;// 無効なメニューインデックス
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
	, m_stageNum(0) // ステージ番号
	, m_menuIndex(INVALID_MENU_INDEX) // メニューのインデックス
	, m_num(SceneID::NONE) // シーンID
	, m_time(0.0f) // 時間
	, m_hit(false) // ヒットフラグ
	, m_enable(false) // このクラスが有効かどうか
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
	// ウィンドウ幅
	m_windowWidth = width;
	// ウィンドウ高さ
	m_windowHeight = height;

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
	// ボタンの数だけループ
	for (int i = 0; i < m_pButtons.size(); i++)
	{	// ウィンドウハンドルを取得
		const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
		// ウィンドウサイズ取得
		RECT rect;
		// クライアント領域サイズを取得
		GetClientRect(hwnd, &rect);
		// ウィンドウの幅（ピクセル単位）
		m_windowWidth = static_cast<int>(rect.right);
		// ウィンドウの高さ（ピクセル単位）
		m_windowHeight = static_cast<int>(rect.bottom);
		// ウィンドウサイズを設定
		m_pButtons[i]->SetWindowSize(m_windowWidth, m_windowHeight);
		// ウィンドウサイズに応じてボタンの位置とスケールを調整
		if (m_windowWidth == Screen::WIDTH)
		{
			// ボタンの座標を更新
			if (m_pButtons.size() == 1)
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) + 100));
			else
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2) - (200) + (i * 200 * 2), float(m_windowHeight / 2) + 100));
			// ボタンのスケールを更新
			m_pButtons[i]->SetScale(Vector2(0.5f, 0.5f));
		}
		else
		{
			// ボタンの座標を更新
			if (m_pButtons.size() == 1)
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) + 100));
			else
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2) - (400) + (i * 400 * 2), float(m_windowHeight / 2 + 200)));
			// ボタンのスケールを更新
			m_pButtons[i]->SetScale(Vector2(1.0f, 1.0f));
		}

		// 時間を加算
		m_pButtons[i]->SetTime(m_pButtons[i]->GetTime() + elapsedTime);
		// ボタンがヒットしたかどうかをチェック
		if (m_pButtons[i]->IsHit(mousePos))
		{
			// ヒットフラグを立てる
			m_hit = true;
			// 前回選択したボタンと違う場合はSE再生フラグを立てる（今後実装）
			//if ((int(m_menuIndex)) != i) m_isSEPlay = false;
			//// SEが再生されていない場合
			//if (!m_isSEPlay)
			//{
			//	// 選択音の再生
			//	m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
			//	// 再生フラグを立てる
			//	m_isSEPlay = true;
			//}
			// ボタンのスケールを更新
			m_pButtons[i]->SetScale(m_pButtons[i]->GetScale() * 1.2f);
			// ヒットしたメニューのインデックスを保存
			m_menuIndex = i;
			// ヒットしたらループを抜ける
			break;
		}
	}
	// ヒット無しなら選択インデックスを無効値に設定
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;
	// 左クリックされたら選択メニューのシーンIDを更新
	if (MouseClick::IsLeftMouseButtonPressed(mouseState))
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
	// UIオブジェクトの生成
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// 指定画像でUI作成
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UIの種類に応じて保存
	if (type == UIType::BUTTON)m_pButtons.push_back(std::move(userInterface));

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
	// ゲームクリアなら次のステージに飛べるようにする
	if (gameClear)m_stageNum++;
	// 次のステージが存在していてゲームクリアなら
	if (m_stageNum < FileCounter::CountFilesInFolder("Resources/Map/", ".csv") && gameClear)
	{
		// ボタンを追加
		Add("ToNextStage"
			, Vector2(Screen::CENTER_X_BUTTON - 400, Screen::CENTER_Y_BUTTON + 200)
			, Vector2(1.0f)
			, KumachiLib::ANCHOR::MIDDLE_CENTER
			, UIType::BUTTON);
	}
	// ゲームオーバーなら
	else if (gameOver)
	{
		// ボタンを追加
		Add("ReTry"
			, Vector2(Screen::CENTER_X_BUTTON - 400, Screen::CENTER_Y_BUTTON + 200)
			, Vector2(1.0f)
			, KumachiLib::ANCHOR::MIDDLE_CENTER
			, UIType::BUTTON);
	}
	// ボタンを追加
	Add("ToStageSelect"
		, Vector2(Screen::CENTER_X_BUTTON + 400, Screen::CENTER_Y_BUTTON + 200)
		, Vector2(1.0f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::BUTTON);

}