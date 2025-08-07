/*
*	@file	PlayScene.cpp
*	@brief	プレイシーンクラス
*/
#include <pch.h>
#include "PlayScene.h"
/*
*	@brief コンストラクタ
*	@details プレイシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
PlayScene::PlayScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_debugCamera(nullptr) // デバッグカメラへのポインタ
	, m_pFixedCameraPlay(nullptr) // 固定カメラへのポインタ
	, m_projectionGame() // ゲーム画面用の射影行列
	, m_projectionControll() // 操作画面用の射影行列
	, m_time(0.0f) // 経過時間
	, m_sceneChangeCount(0.0f) // シーン変更カウント
	, m_isChangeScene(false) // シーン変更フラグ
	, m_viewPortGame() // ゲーム画面用のビューポート
	, m_viewPortControll() // 操作画面用のビューポート
	, m_nowSceneID(sceneID)// 現在のシーンID
	, m_stageNumber(-1) // ステージ番号
{}
/*
*	@brief デストラクタ
*	@details プレイシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
PlayScene::~PlayScene() {}
/*
*	@brief 初期化
*	@details プレイシーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void PlayScene::Initialize(CommonResources* resources)
{
	// 名前空間のエイリアス
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// 各カメラを作成
	CreateCamera();
	// 各種ビューポートを作成する
	CreateViewports();
	// デバイスリソースを取得
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// マウスを作成する
	m_pMouse = std::make_unique<MyMouse>();
	// マウスを初期化する
	m_pMouse->Initialize(m_pCommonResources);
	// マウスにビューポートを設定
	m_pMouse->SetViewport(m_viewPortControll);
	// ステージ番号を文字列にする
	std::string stagePath = std::to_string(m_stageNumber);
	// マップ生成
	m_pCSVMap = std::make_unique<CSVMap>(m_pCommonResources);
	// CSVマップを読み込む
	m_pCSVMap->LoadMap("Resources/Map/" + stagePath + "_map.csv");
	// CSVアイテムを作成する
	m_pCSVItem = std::make_unique<CSVItem>(m_pCommonResources);
	// CSVアイテムを読み込む
	m_pCSVItem->LoadItem("Resources/Item/" + stagePath + "_item.csv");
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// ミニキャラベースにCSVマップを設定
	m_pMiniCharacterBase->SetCSVMap(m_pCSVMap.get());
	// ミニキャラベースにCSVアイテムを設定
	m_pMiniCharacterBase->SetCSVItem(m_pCSVItem.get());
	// ミニキャラを初期化する
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// ミニキャラベースにミニキャラをアタッチ
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacter>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));
	// 操作画面の背景を作成する
	m_pUIBack = std::make_unique<UIBack>(m_pCommonResources);
	// 操作画面の背景を初期化する
	m_pUIBack->Create(deviceResources);
	// パネルを作成する
	m_pPanel = std::make_unique<Panel>(m_pCSVMap->GetMaxCol(), m_pCSVMap->GetMaxRow());
	// パネルにマウスを設定
	m_pPanel->SetMouse(m_pMouse.get());
	// パネルにマップ情報を渡す
	m_pPanel->SetCSVMap(m_pCSVMap.get());
	// パネルにアイテム情報を渡す
	m_pPanel->SetCSVItem(m_pCSVItem.get());
	// パネルを初期化する
	m_pPanel->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// 次のタイルを作成する
	m_pNextTiles = std::make_unique<NextTiles>();
	// 次のタイルにマウスを設定
	m_pNextTiles->SetMouse(m_pMouse.get());
	// 次のタイルにマップ情報を渡す
	m_pNextTiles->SetCSVMap(m_pCSVMap.get());
	// 次のタイルを初期化する
	m_pNextTiles->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// ミニキャラベースに次のタイルを設定
	m_pMiniCharacterBase->SetNextTiles(m_pNextTiles.get());
	// メダルカウンターを作成する
	m_pMedalCounter = std::make_unique<MedalCounter>();
	// メダルカウンターを初期化する
	m_pMedalCounter->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// 結果アニメーションを作成する
	m_pResultAnimation = std::make_unique<ResultAnimation>();
	// 結果アニメーションを初期化する
	m_pResultAnimation->Initialize(m_pCommonResources);
	// 結果UIを作成する
	m_pResultUI = std::make_unique<ResultUI>();
	// 結果UIを初期化する
	m_pResultUI->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// 結果UIにステージ番号を渡す
	m_pResultUI->SetStageNum(m_stageNumber);
	// スピードアップボタンを作成する
	m_pSpeedUpUI = std::make_unique<SpeedUpUI>();
	// スピードアップボタンを初期化する
	m_pSpeedUpUI->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
}
/*
*   @brief 更新処理
*   @details プレイシーンの全てのゲームオブジェクト・UIの更新
*   @param elapsedTime 前フレームからの経過時間
*	@return なし
*/
void PlayScene::Update(float elapsedTime)
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 経過時間を加算
	m_time += elapsedTime;
	// スピードアップボタンの更新
	m_pSpeedUpUI->Update(elapsedTime);
	// スピードアップボタンが押された場合、ゲーム内経過時間を倍にする
	float inGameTime = m_pSpeedUpUI->IsPressed() ? elapsedTime * 10.0f : elapsedTime;
	// 結果アニメーションが有効ならリザルト用のカメラに切り替えて書く処理を行う
	if (m_pResultAnimation->IsAnimationEnable())
	{
		// スピードアップボタンを強制的にオフにする
		m_pSpeedUpUI->SetPressed(false);
		// リザルトUIが無効な場合
		if (!m_pResultUI->IsEnable() && m_pResultAnimation->IsAnimationEnd())
		{
			// 有効にする
			m_pResultUI->SetEnable(true);
			// 結果の設定
			m_pResultUI->SetResult(m_pMiniCharacterBase->IsGameOver(), m_pMiniCharacterBase->IsGameClear());
		}
		// リザルト用固定カメラの更新
		m_pFixedCameraResult->Update();
		// ビュー行列を取得
		m_view = m_pFixedCameraResult->GetViewMatrix();
		// 結果アニメーションの更新
		m_pResultAnimation->Update(elapsedTime);
		// 結果UIの更新
		m_pResultUI->Update(elapsedTime);
		// ゲームクリアなら以下の処理も行う
		if (m_pMiniCharacterBase->IsGameClear())
		{
			// カメラの位置を滑らかに変える
			m_pFixedCameraResult->SetCameraDistance(Vector3(0.0f, 1.75f, 10.0f));
			Vector3 targetPos = m_pMiniCharacterBase->GetCameraPosition();
			// カメラのターゲット位置をミニキャラのカメラ位置に設定
			m_pFixedCameraResult->SetTargetPosition(Vector3(targetPos.x, targetPos.y + 3.0f, targetPos.z));
			// カメラの座標を更新
			m_pFixedCameraResult->SetEyePosition(m_pMiniCharacterBase->GetCameraPosition() + m_pFixedCameraResult->GetCameraDistance());
			// 次のステージ番号を取得
			m_stageNumber = m_pResultUI->GetStageNum();
		}
	}
	// リザルトでない場合は通常の更新を行う
	else
	{
		// プレイ画面用固定カメラの更新
		m_pFixedCameraPlay->Update();
		// ビュー行列を取得
		m_view = m_pFixedCameraPlay->GetViewMatrix();
		// マウスの更新
		m_pMouse->Update(inGameTime);
		// 操作画面の背景の更新
		m_pUIBack->Update(inGameTime);
		// パネルの更新
		m_pPanel->Update(inGameTime);
		// 次のタイルの更新
		m_pNextTiles->Update(inGameTime);
		// メダルカウンターに現在のメダル数を設定
		m_pMedalCounter->SetCollectedMedalCount(m_pCSVItem->GetCollectedMedals());
		// メダルカウンターの更新
		m_pMedalCounter->Update(inGameTime);
	}
	// CSVアイテムの更新
	m_pCSVItem->Update(inGameTime);
	// ミニキャラの更新
	m_pMiniCharacterBase->Update(inGameTime, Vector3::Zero, Quaternion::Identity);
	// 結果アニメーションに結果を渡す
	m_pResultAnimation->SetResult(m_pMiniCharacterBase->IsGameOver(), m_pMiniCharacterBase->IsGameClear());
	// アニメーションが終わったらシーン変更
	if (m_pResultAnimation->IsAnimationEnd() && m_pResultUI->GetSceneNum() > -1)m_isChangeScene = true;
}
/*
*	@brief 描画処理
*	@details プレイシーンの全てのゲームオブジェクト・UIの描画
*	@param なし
*	@return なし
*/
void PlayScene::Render()
{
	// デバイスコンテキストを取得
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 出力サイズを取得
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// 結果アニメーションが有効でない場合（プレイ中）は二つのビューポートに分けて描画
	if (!m_pResultAnimation->IsAnimationEnable())
	{
		// --- 左側: ゲーム画面用ビューポート ---
		context->RSSetViewports(1, &m_viewPortGame);
		// ここでゲーム画面を描画
		// CSVマップの描画
		m_pCSVMap->Render(m_view, m_projectionGame);
		// CSVアイテムの描画
		m_pCSVItem->Render(m_view, m_projectionGame);
		// ミニキャラの描画
		m_pMiniCharacterBase->Render(m_view, m_projectionGame);
		// --- 右側: 操作画面用ビューポート ---
		context->RSSetViewports(1, &m_viewPortControll);
		// 操作画面の背景を描画
		m_pUIBack->Render();
		// パネル(タイル)を描画
		m_pPanel->DrawTiles();
		// 設置済みタイルを描画
		m_pNextTiles->DrawPlacedTiles();
		// パネル(アイテム)を描画
		m_pPanel->DrawItems();
		// 設置候補のタイルを描画
		m_pNextTiles->Render();
		// ビューポートを元の設定に戻す
		const auto& screenViewport = m_pCommonResources->GetDeviceResources()->GetScreenViewport();
		context->RSSetViewports(1, &screenViewport);
		// メダルカウンターの描画
		m_pMedalCounter->Render();
		// スピードアップボタンの描画
		m_pSpeedUpUI->Render();
	}
	// 結果アニメーションが有効な場合は一つのビューポートでの描画
	if (m_pResultAnimation->IsAnimationEnable())
	{
		// リザルト用固定カメラのビュー行列を取得
		m_pCSVMap->Render(m_view, m_projectionResult);
		// CSVアイテムの描画
		m_pCSVItem->Render(m_view, m_projectionResult);
		// ミニキャラの描画
		m_pMiniCharacterBase->Render(m_view, m_projectionResult);
		// 結果アニメーションの描画
		m_pResultAnimation->Render();
		// 結果UIの描画
		if (m_pResultAnimation->IsAnimationEnd())m_pResultUI->Render();
	}

}
/*
*	@brief 終了
*	@details プレイシーンクラスの終了を行う
*	@param なし
*	@return なし
*/
void PlayScene::Finalize() {}
/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		auto sceneID = m_pResultUI->GetSceneNum();
		switch (sceneID)
		{
		case ResultUI::REPLAY: // リプレイ選択
			// リプレイ画面へ
			return IScene::SceneID::PLAY;
		case ResultUI::SELECT_STAGE:// ステージセレクト選択
			// ステージセレクト画面へ
			return IScene::SceneID::STAGESELECT;
		}

	}
	// シーン変更がない場合何もしない
	return IScene::SceneID::NONE;
}
/*
*	@brief カメラを作成する
*	@details プレイ中の固定カメラとリザルト用の固定カメラを作成する
*	@param なし
*	@return なし
*/
void PlayScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// プレイ中の固定カメラを作成する
	m_pFixedCameraPlay = std::make_unique<FixedCamera>();
	m_pFixedCameraPlay->Initialize((int)(rect.right * 0.7f), rect.bottom);
	// リザルト用固定カメラを作成する
	m_pFixedCameraResult = std::make_unique<FixedCamera>();
	m_pFixedCameraResult->Initialize(rect.right, rect.bottom);
	// 射影行列(ゲーム画面用)を作成する
	m_projectionGame = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.7f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
	// 射影行列(操作画面用)を作成する
	m_projectionControll = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.3f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
	// 射影行列(リザルト用)を作成する
	m_projectionResult = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief ビューポートを作成する
*	@details プレイシーンのビューポートを設定する
*	@param なし
*	@return なし
*/
void PlayScene::CreateViewports()
{
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// --- 左側: ゲーム画面用ビューポートの設定 ---
	D3D11_VIEWPORT viewportLeft = {};
	// 左上の座標を設定
	viewportLeft.TopLeftX = 0;
	viewportLeft.TopLeftY = 0;
	// 幅と高さを設定
	viewportLeft.Width = (FLOAT)rect.right * 0.7f; // 左7割
	viewportLeft.Height = (FLOAT)rect.bottom;
	// 深度の範囲を設定
	viewportLeft.MinDepth = 0.0f;
	viewportLeft.MaxDepth = 1.0f;
	// ゲーム画面用ビューポートを設定
	m_viewPortGame = viewportLeft;
	// --- 右側: 操作画面用ビューポートの設定 ---
	D3D11_VIEWPORT viewportRight = {};
	// 左上の座標を設定
	viewportRight.TopLeftX = (FLOAT)rect.right * 0.7f;
	viewportRight.TopLeftY = 0;
	// 幅と高さを設定
	viewportRight.Width = (FLOAT)rect.right * 0.3f; // 右3割
	viewportRight.Height = (FLOAT)rect.bottom;
	// 深度の範囲を設定
	viewportRight.MinDepth = 0.0f;
	viewportRight.MaxDepth = 1.0f;
	// 操作画面用ビューポートを設定
	m_viewPortControll = viewportRight;
}
/*
*	@brief デバッグ文字列を描画する
*	@details プレイシーンのデバッグ文字列を描画する
*	@param なし
*	@return なし
*/
void PlayScene::DrawDebugString()
{
	// デバッグ文字列を取得
	const auto debugString = m_pCommonResources->GetDebugString();
	// ゲームオーバーやゲームクリアの状態をデバッグ文字列に追加
	debugString->AddString("IsGameOver:%s", m_pMiniCharacterBase->IsGameOver() ? "true" : "false");
	debugString->AddString("IsGameClear:%s", m_pMiniCharacterBase->IsGameClear() ? "true" : "false");
	// カメラの位置と被写体座標をデバッグ文字列に追加
	debugString->AddString("CameraEye:%f,%f,%f", m_pFixedCameraPlay->GetEyePosition().x, m_pFixedCameraPlay->GetEyePosition().y, m_pFixedCameraPlay->GetEyePosition().z);
	debugString->AddString("CameraTarget:%f,%f,%f", m_pFixedCameraPlay->GetTargetPosition().x, m_pFixedCameraPlay->GetTargetPosition().y, m_pFixedCameraPlay->GetTargetPosition().z);
	// 当たったボタン
	debugString->AddString("HitButtonIndex:%i", m_pResultUI->GetSceneNum());
	debugString->AddString("StageNum:%i", m_stageNumber);
}
