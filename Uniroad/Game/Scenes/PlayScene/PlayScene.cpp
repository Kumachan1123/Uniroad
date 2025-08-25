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
	// シャドウマップライトを作成する
	m_pShadowMapLight = std::make_unique<ShadowMapLight>(m_pCommonResources);
	// 空を作成する
	m_pSky = std::make_unique<Sky>(m_pCommonResources);
	// 空を初期化する
	m_pSky->Initialize();
	// 空の位置を設定
	m_pSky->SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	// 天球のスケールを設定
	m_pSky->SetScale(Vector3(0.1f));
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
	// マップにシャドウマップライトを設定する
	m_pCSVMap->SetShadowMapLight(m_pShadowMapLight.get());
	// CSVマップを読み込む
	m_pCSVMap->LoadMap("Resources/Map/" + stagePath + "_map.csv");
	// CSVアイテムを作成する
	m_pCSVItem = std::make_unique<CSVItem>(m_pCommonResources);
	// CSVアイテムにカメラを設定する
	m_pCSVItem->SetCamera(m_pFixedCameraPlay.get());
	// CSVアイテムにシャドウマップライトを設定する
	m_pCSVItem->SetShadowMapLight(m_pShadowMapLight.get());
	// CSVアイテムを読み込む
	m_pCSVItem->LoadItem("Resources/Item/" + stagePath + "_item.csv");
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// ミニキャラベースにCSVマップを設定
	m_pMiniCharacterBase->SetCSVMap(m_pCSVMap.get());
	// ミニキャラベースにCSVアイテムを設定
	m_pMiniCharacterBase->SetCSVItem(m_pCSVItem.get());
	// ミニキャラにカメラを設定する
	m_pMiniCharacterBase->SetCamera(m_pFixedCameraPlay.get());
	// ミニキャラのベースにシャドウマップライトを設定する
	m_pMiniCharacterBase->SetShadowMapLight(m_pShadowMapLight.get());
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
	m_pSpeedUpButton = std::make_unique<SpeedUpButton>();
	// スピードアップボタンを初期化する
	m_pSpeedUpButton->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// フェードを作成する
	m_pFade = std::make_unique<Fade>();
	// フェードを初期化する
	m_pFade->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// フェードインに移行
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// パーティクルを作成する
	m_pConfetti = std::make_unique<Particle>(Utility::Type::CONFETTI, 200);
	// パーティクルを初期化する
	m_pConfetti->Initialize(m_pCommonResources);
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
	// カメラ用の座標をシャドウマップライトに設定
	Vector3 playerPosition = m_pMiniCharacterBase->GetCameraPosition();
	// シャドウマップライト更新
	m_pShadowMapLight->SetLightPosition(Vector3(playerPosition.x, 30.0f, playerPosition.z));

	// 空の更新
	m_pSky->Update(elapsedTime);
	// スピードアップボタンの更新
	m_pSpeedUpButton->Update(elapsedTime);
	// スピードアップボタンが押された場合、ゲーム内経過時間を倍にする
	float inGameTime = m_pSpeedUpButton->IsPressed() ? elapsedTime * 10.0f : elapsedTime;
	// 結果アニメーションが有効ならリザルト用のカメラに切り替えて書く処理を行う
	if (m_pResultAnimation->IsAnimationEnable())
	{
		// スピードアップボタンを強制的にオフにする
		m_pSpeedUpButton->SetPressed(false);
		// リザルトUIが無効な場合
		if (!m_pResultUI->IsEnable() && m_pResultAnimation->IsAnimationEnd())
		{
			// 有効にする
			m_pResultUI->SetEnable(true);
			// ミニキャラにリザルト用カメラを設定する
			m_pMiniCharacterBase->SetCamera(m_pFixedCameraResult.get());
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
			// パーティクルを更新
			m_pConfetti->SetParams(SetConfettiParams());
			m_pConfetti->Update(elapsedTime);
			// カメラの位置を調整
			m_pFixedCameraResult->SetCameraDistance(Vector3(0.0f, 5.75f, 10.0f));
			// カメラのターゲット位置をミニキャラのカメラ位置に設定
			Vector3 targetPos = m_pMiniCharacterBase->GetCameraPosition();
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
		// パネルにプレイヤーの座標を設定
		m_pPanel->SetPlayerPosition(m_pMiniCharacterBase->GetCameraPosition());
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
	// フェードの更新
	m_pFade->Update(elapsedTime);
	// フェードインが終わったらフェード状態をなくす
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)m_pFade->SetState(Fade::FadeState::None);
	// アニメーションが終わったらフェードアウトに移行
	if (m_pResultAnimation->IsAnimationEnd() && //	アニメーションが終わって
		m_pResultUI->GetSceneNum() != ResultUI::SceneID::NONE &&// 	// リザルトUIのシーン番号が無効でなくて
		m_pResultUI->IsMouseClicked()) //	マウスがクリックされていたら
		m_pFade->SetState(Fade::FadeState::FadeOut);
	// フェードアウトが完了していたら、シーン遷移フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
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
		// 	// 空を描画する
		m_pSky->Render(m_view, m_projectionGame);
		// CSVマップの描画
		m_pCSVMap->Render(m_view, m_projectionGame);
		// シャドウマップライトにビューポートを設定する
		m_pShadowMapLight->SetViewport(m_viewPortGame);
		// シャドウマップライトをレンダリングする
		m_pShadowMapLight->RenderShadow();
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
		// プレイヤーのアイコンを描画
		m_pPanel->Render();
		// ビューポートを元の設定に戻す
		const auto& screenViewport = m_pCommonResources->GetDeviceResources()->GetScreenViewport();
		context->RSSetViewports(1, &screenViewport);
		// メダルカウンターの描画
		m_pMedalCounter->Render();
		// スピードアップボタンの描画
		m_pSpeedUpButton->Render();
	}
	// 結果アニメーションが有効な場合は一つのビューポートでの描画
	if (m_pResultAnimation->IsAnimationEnable())
	{
		// 空を描画する
		m_pSky->Render(m_view, m_projectionResult);
		// CSVマップの描画
		m_pCSVMap->Render(m_view, m_projectionResult);
		// シャドウマップライトにビューポートを設定する
		m_pShadowMapLight->SetViewport(m_pCommonResources->GetDeviceResources()->GetScreenViewport());
		// シャドウマップライトをレンダリングする
		m_pShadowMapLight->RenderShadow();
		// CSVアイテムの描画
		m_pCSVItem->Render(m_view, m_projectionResult);
		// ミニキャラの描画
		m_pMiniCharacterBase->Render(m_view, m_projectionResult);
		// 紙吹雪パーティクルのビルボード行列の作成
		m_pConfetti->CreateBillboard(m_pFixedCameraResult->GetTargetPosition(), m_pFixedCameraResult->GetEyePosition(), m_pFixedCameraResult->GetUpPosition());
		// ゲームクリアなら紙吹雪パーティクルの描画
		if (m_pMiniCharacterBase->IsGameClear())m_pConfetti->Render(m_view, m_projectionResult);
		// 結果アニメーションの描画
		m_pResultAnimation->Render();
		// 結果UIの描画
		if (m_pResultAnimation->IsAnimationEnd())m_pResultUI->Render();
	}
	// フェードを描画する
	m_pFade->Render();
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
		// 遷移先のシーン番号によって分岐
		switch (m_pResultUI->GetSceneNum())
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
		0.1f, 1000.0f
	);
	// 射影行列(操作画面用)を作成する
	m_projectionControll = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.3f) / static_cast<float>(rect.bottom),
		0.1f, 1000.0f
	);
	// 射影行列(リザルト用)を作成する
	m_projectionResult = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 1000.0f
	);
	// カメラに射影行列をセット
	m_pFixedCameraPlay->SetProjectionMatrix(m_projectionGame);
	m_pFixedCameraResult->SetProjectionMatrix(m_projectionResult);
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
#ifdef _DEBUG
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
#endif  
}
/*
*	@brief 紙吹雪パーティクルのパラメーターを設定
*	@details 紙吹雪パーティクルのパラメーターを設定する
*	@param なし
*	@return 紙吹雪パーティクルのパラメーター
*/
Utility::ParticleParams PlayScene::SetConfettiParams()
{
	// 名前空間のエイリアス
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 乱数の設定
	std::random_device seed;
	std::default_random_engine engine(seed());
	// ランダムな角度
	std::uniform_real_distribution<> angleDist(0, XM_2PI);
	// ランダムなXY平面の速度の大きさ
	std::uniform_real_distribution<> horizSpeedDist(0.8f, 1.5f);
	// ランダムなX座標
	std::uniform_real_distribution<> xDist(-15.0f, 15.0f);
	// ランダムなZ座標
	std::uniform_real_distribution<> zDist(-1.8f, 1.8f);
	// ランダムな回転スピード
	std::uniform_real_distribution<> rotSpeedDist(-2.5f, 2.5f);
	// ランダムなスケール
	std::uniform_real_distribution<> scaleDist(0.35f, 0.7f);
	// ランダムな色
	std::uniform_real_distribution<> colorDist(0.7f, 1.0f);
	float randAngleXY = static_cast<float>(angleDist(engine));
	float horizSpeed = static_cast<float>(horizSpeedDist(engine));
	float randX = static_cast<float>(xDist(engine));
	float randZ = static_cast<float>(zDist(engine));
	float scale = static_cast<float>(scaleDist(engine));
	// ゲーミングカラー
	Vector4 gamingColor(
		static_cast<float>(colorDist(engine)),
		static_cast<float>(colorDist(engine)),
		static_cast<float>(colorDist(engine)),
		static_cast<float>(0.75f + colorDist(engine) * 0.15f)
	);
	// ランダムな回転速度
	Vector3 rotateSpeed(
		static_cast<float>(rotSpeedDist(engine)),
		static_cast<float>(rotSpeedDist(engine)),
		static_cast<float>(rotSpeedDist(engine))
	);
	// ランダムな方向の速度ベクトル
	Vector3 randomVelocity = Vector3(
		cosf(randAngleXY) * horizSpeed,  // X成分
		0.0f,                         // Y成分（上方向に初速）
		sinf(randAngleXY) * horizSpeed   // Z成分
	);
	// パーティクルのパラメーターを設定して返す
	Utility::ParticleParams params{};
	params.life = 3.5f;
	params.pos = Vector3(randX, 7.0f, randZ);  // 高さ7から広がって生える
	params.velocity = randomVelocity;
	params.accele = Vector3(0.0f, -9.8f * 0.4f, 0.0f); // 重力はやや弱め（ひらひら感）
	params.rotateAccele = rotateSpeed; // 回転加速度なし（一定速度でひらひら）
	params.rotate = rotateSpeed; // 初期回転
	params.startScale = Vector3(scale, scale, 0.0f);
	params.endScale = Vector3(scale * 0.7f, scale * 0.7f, 0.0f); // 少しだけ縮む
	params.startColor = gamingColor; // 虹色ゲーミングカラー
	params.endColor = Vector4(gamingColor.x, gamingColor.y, gamingColor.z, 0.0f); // 色はそのまま透明へ
	params.type = Utility::Type::CONFETTI; // パーティクルのタイプ
	return params;
}
