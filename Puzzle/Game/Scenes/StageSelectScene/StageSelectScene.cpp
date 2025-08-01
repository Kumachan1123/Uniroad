/*
*	@file StageSelectScene.cpp
*	@brief ステージ選択シーンクラス
*/
#include "pch.h"
#include "StageSelectScene.h"

/*
*	@brief コンストラクタ
*	@details ステージ選択シーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
StageSelectScene::StageSelectScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pFixedCamera(nullptr) // カメラへのポインタ
	, m_pMiniCharacterBase(nullptr) // ミニキャラベースへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_isMiniCharacterMove(false) // ミニキャラが移動したかどうか
	, m_stageNumber(-1) // ステージ番号
	, m_nextSceneID(sceneID) // 次のシーンID
{
}
/*
*	@brief デストラクタ
*	@details ステージ選択シーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
StageSelectScene::~StageSelectScene()
{
	// 何もしない
}

/*
*	@brief 初期化
*	@details ステージ選択シーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void StageSelectScene::Initialize(CommonResources* resources)
{
	// 名前空間のエイリアス
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// カメラを作成する
	CreateCamera();
	// ステージセレクトを作成する
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// ステージセレクトを初期化する
	m_pStageSelect->Initialize();
	// 平面を作成する
	m_pPlaneArea = std::make_unique<PlaneArea>(m_pCommonResources);
	// 頂点設定
	for (int i = 0; i < FileCounter::CountFilesInFolder("Resources/Map/", ".csv"); ++i)
	{
		// 中心座標を計算
		Vector3 center(4.0f * (float)i, 0.5f, 2.0f);
		// 横幅
		float width = 2.0f;
		// 奥行き
		float depth = 2.0f;
		// 平面の頂点を作成
		std::vector<Vector3> vertices = CreatePlaneVertices(center, width, depth, center.y);
		// 平面に頂点配列を登録
		m_pPlaneArea->AddPlane(vertices);
		// 平面の中心座標を登録する
		m_pPlaneArea->AddPlanePosition(center);
		// 平面の色を赤に設定
		m_pPlaneArea->SetPlaneColor(Color(1, 0, 0));
		// ステージの入り口を作成する
		m_pStageGates.push_back(std::make_unique<StageGate>(m_pCommonResources));
		// ステージの入り口を初期化する
		m_pStageGates.back()->Initialize();
		// ステージの入り口の位置を設定
		m_pStageGates.back()->SetPosition(center);
	}
	// 平面を初期化する
	m_pPlaneArea->Initialize();
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// ミニキャラベースにCSVマップを設定
	m_pMiniCharacterBase->SetCSVMap(nullptr);
	// ミニキャラベースにCSVアイテムを設定
	m_pMiniCharacterBase->SetCSVItem(nullptr);
	// ミニキャラベースに次のタイルを設定
	m_pMiniCharacterBase->SetNextTiles(nullptr);
	// ミニキャラに平面を設定
	m_pMiniCharacterBase->SetPlaneArea(m_pPlaneArea.get());
	// ミニキャラを初期化する
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// ミニキャラベースにミニキャラをアタッチ
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterSelectStage>(m_pMiniCharacterBase.get(), Vector3(0.0f, -0.5f, 0.0f), 0.0f));

}
/*
*	@brief 更新
*	@details ステージ選択シーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void StageSelectScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// トラッキングカメラに追従対象の座標を設定
	m_pTrackingCamera->SetTargetPosition(m_pMiniCharacterBase->GetCameraPosition());
	// トラッキングカメラの更新
	m_pTrackingCamera->Update();
	// デバッグカメラの更新
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	//#ifndef DEBUG
	//	const auto KeyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	//	if (KeyState.Space)
	//		m_view = m_pTrackingCamera->GetViewMatrix();
	//	else
	//		m_view = m_pFixedCamera->GetViewMatrix();
	//
	//#endif
		// ビュー行列を取得
	m_view = m_pTrackingCamera->GetViewMatrix();
	// 座標を初期化
	Vector3 position(0.0f, -0.5f, -1.75f);
	// 角度を初期化
	Quaternion angle(Quaternion::Identity);
	// ミニキャラの更新
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	// 平面にビュー行列を渡す
	m_pPlaneArea->SetView(m_view);
	// 平面に射影行列を渡す
	m_pPlaneArea->SetProjection(m_projection);
	// 平面を更新
	m_pPlaneArea->Update(elapsedTime);
	// ステートの入り口を更新
	for (auto& gate : m_pStageGates)gate->Update(elapsedTime);
	// マウスステートを取得
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 左クリックを検知
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_pPlaneArea->GetHitPlaneIndex() > -1)
	{
		// ステージ番号を取得
		m_stageNumber = m_pPlaneArea->GetHitPlaneIndex();
		// シーン遷移
		m_isChangeScene = true;
	}
	// 何か選ばれているなら移動フラグを立てる
	if (m_pPlaneArea->GetHitPlaneIndex() > -1)
	{
		m_pMiniCharacterBase->SetMoving(true);
	}

}

/*
*	@brief 描画
*	@details ステージ選択シーンクラスの描画を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::Render()
{
	using namespace DirectX::SimpleMath;
	//auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();

	// ステージセレクトの描画
	m_pStageSelect->Render(m_view, m_projection);
	// ステージの入り口の描画
	for (auto& gate : m_pStageGates)gate->Render(m_view, m_projection);
	// ミニキャラの描画
	m_pMiniCharacterBase->Render(m_view, m_projection);
	// 平面の描画
	m_pPlaneArea->Render();
	// --- デバッグ情報---
	const auto debugString = m_pCommonResources->GetDebugString();
	// レイと当たったかを表示
	debugString->AddString("Ray Hit:%s", m_pPlaneArea->IsHitPlane() ? "true" : "false");
	// レイと当たったかを表示
	debugString->AddString("HitRayIndex:%i", m_pPlaneArea->GetHitPlaneIndex());
}

/*
*	@brief 終了
*	@details ステージ選択シーンクラスの終了を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::Finalize()
{
}


/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// ゲームオーバーシーンへ
		return IScene::SceneID::PLAY;
	}
	// シーン変更がない場合何もしない
	return IScene::SceneID::NONE;
}

/*
*	@brief カメラに関する設定をする
*	@details ステージ選択シーンクラスのカメラ設定を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// デバッグカメラを作成する
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// 固定カメラを作成する
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);
	// トラッキングカメラを作成する
	m_pTrackingCamera = std::make_unique<TrackingCamera>();
	m_pTrackingCamera->Initialize((int)(rect.right), rect.bottom);
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief 平面の中心座標を基に４つの頂点を設定する
*	@details 平面の中心座標を基に、幅と奥行きを指定して４つの頂点を設定する
*	@param center 平面の中心座標
*	@param width 平面の幅
*	@param depth 平面の奥行き
*	@param y 平面の高さ（デフォルトは0.5f）
*	@return ４つの頂点の座標を格納したベクトル
*/
std::vector<DirectX::SimpleMath::Vector3> StageSelectScene::CreatePlaneVertices(const DirectX::SimpleMath::Vector3& center, float width, float depth, float y) const
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 平面の頂点を計算する
	float halfW = width / 2.0f;
	// 平面の奥行きの半分を計算する
	float halfD = depth / 2.0f;
	return {
		Vector3(center.x - halfW, y, center.z - halfD), // 左下
		Vector3(center.x + halfW, y, center.z - halfD), // 右下
		Vector3(center.x + halfW, y, center.z + halfD), // 右上
		Vector3(center.x - halfW, y, center.z + halfD)  // 左上
	};
}
