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
	// デバイスリソースを取得する
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	const auto context = deviceResources->GetD3DDeviceContext();
	const auto device = deviceResources->GetD3DDevice();
	const auto states = m_pCommonResources->GetCommonStates();
	// グリッド床を作成する
	m_pGridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// ミニキャラベースにCSVマップを設定
	m_pMiniCharacterBase->SetCSVMap(nullptr);
	// ミニキャラベースにCSVアイテムを設定
	m_pMiniCharacterBase->SetCSVItem(nullptr);
	// ミニキャラベースに次のタイルを設定
	m_pMiniCharacterBase->SetNextTiles(nullptr);
	// ミニキャラを初期化する
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// ミニキャラベースにミニキャラをアタッチ
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterSelectStage>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));
	// ステージセレクトを作成する
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// ステージセレクトを初期化する
	m_pStageSelect->Initialize();
	// 平面を作成する
	m_pPlaneArea = std::make_unique<PlaneArea>(m_pCommonResources);
	// 頂点設定
	std::vector<Vector3> vertices1 =
	{
		Vector3(-5, 0, -1),
		Vector3(-3, 0, -1),
		Vector3(-3, 0, 1),
		Vector3(-5, 0, 1)
	};
	std::vector<Vector3> vertices2 =
	{
		Vector3(-1, 0, -1),
		Vector3(1, 0, -1),
		Vector3(1, 0, 1),
		Vector3(-1, 0, 1)
	};
	std::vector<Vector3> vertices3 =
	{
		Vector3(3, 0, -1),
		Vector3(5, 0, -1),
		Vector3(5, 0, 1),
		Vector3(3, 0, 1)
	};
	// 平面に頂点配列を登録
	m_pPlaneArea->AddPlane(vertices1);
	m_pPlaneArea->SetPlaneColor(Color(1, 0, 0));
	m_pPlaneArea->AddPlane(vertices2);
	m_pPlaneArea->SetPlaneColor(Color(1, 0, 0));
	m_pPlaneArea->AddPlane(vertices3);
	m_pPlaneArea->SetPlaneColor(Color(1, 0, 0));
	// 平面を初期化する
	m_pPlaneArea->Initialize();
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
	// ビュー行列を取得
	m_view = m_pFixedCamera->GetViewMatrix();
	// 座標を初期化
	Vector3 position(0.0f, 0.0f, 0.0f);
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
	// マウスステートを取得
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 左クリックを検知
	if (mouseState.leftButton && m_pPlaneArea->GetHitPlaneIndex() > -1)
	{
		// ステージ番号を取得
		m_stageNumber = m_pPlaneArea->GetHitPlaneIndex();
		// シーン遷移
		m_isChangeScene = true;
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
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();

	// ステージセレクトの描画
	m_pStageSelect->Render(m_view, m_projection);
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
	//// デバッグカメラを作成する
	//m_debugCamera = std::make_unique<mylib::DebugCamera>();
	//m_debugCamera->Initialize(rect.right * 0.7f, rect.bottom);
	// 固定カメラを作成する
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);

	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
