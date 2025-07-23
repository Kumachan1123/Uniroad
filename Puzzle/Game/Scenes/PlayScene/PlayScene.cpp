/*
	@file	PlayScene.cpp
	@brief	一般的なシーンクラス
*/
#include <pch.h>
#include "PlayScene.h"

PlayScene::PlayScene(IScene::SceneID sceneID)
	: m_pCommonResources{}
	, m_debugCamera{}
	, m_pFixedCamera{}
	, m_projectionGame{}
	, m_projectionControll{}
	, m_time{ 0.0f }
	, m_isChangeScene{}
	, m_viewPortGame{}
	, m_viewPortControll{}
	, m_nowSceneID{ sceneID }// 現在のシーンID

{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;
	CreateCamera();
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// 各種ビューポートを作成する
	CreateViewports();
	// マウスを作成する
	m_pMouse = std::make_unique<MyMouse>();
	// マウスを初期化する
	m_pMouse->Initialize(m_pCommonResources);
	// マウスにビューポートを設定
	m_pMouse->SetViewport(m_viewPortControll);
	// マップ生成
	m_pCSVMap = std::make_unique<CSVMap>(m_pCommonResources);
	// CSVマップを読み込む
	m_pCSVMap->LoadMap("Resources/Map/test.csv");
	// CSVアイテムを作成する
	m_pCSVItem = std::make_unique<CSVItem>(m_pCommonResources);
	// CSVアイテムを読み込む
	m_pCSVItem->LoadItem("Resources/Item/test.csv");
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

}

void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;
	//// デバッグカメラの更新
	//m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// マウスの更新
	m_pMouse->Update(elapsedTime);
	// 操作画面の背景の更新
	m_pUIBack->Update(elapsedTime);
	// CSVアイテムの更新
	m_pCSVItem->Update(elapsedTime);

	// パネルの更新
	m_pPanel->Update(elapsedTime);
	// 次のタイルの更新
	m_pNextTiles->Update(elapsedTime);
	// ミニキャラの更新
	Vector3 position(0.0f, 0.0f, 0.0f);
	Quaternion angle(Quaternion::Identity);
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	// メダルカウンターに現在のメダル数を設定
	m_pMedalCounter->SetCollectedMedalCount(m_pCSVItem->GetCollectedMedals());

	// メダルカウンターの更新
	m_pMedalCounter->Update(elapsedTime);
}
void PlayScene::Render()
{
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();

	// --- 左側: ゲーム画面用ビューポート ---
	context->RSSetViewports(1, &m_viewPortGame);

	// ここでゲーム画面を描画
	// ビュー行列を取得
	m_view = m_pFixedCamera->GetViewMatrix();
	//m_view = m_debugCamera->GetViewMatrix();
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


	// --- デバッグ情報（例） ---
	// ビューポートを元の設定に戻す
	const auto& viewPort = m_pCommonResources->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewPort);
	// メダルカウンターの描画
	m_pMedalCounter->Render();

	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("Use ViewPort.");
}

void PlayScene::Finalize()
{
}

IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::STAGESELECT;// ゲームオーバーシーンへ
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;// 何もしない
}

//---------------------------------------------------------
// カメラ、ビュー行列、射影行列を作成する
//---------------------------------------------------------
void PlayScene::CreateCamera()
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
	m_pFixedCamera->Initialize((int)(rect.right * 0.7f), rect.bottom);

	// 射影行列を作成する
	m_projectionGame = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.7f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
	m_projectionControll = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.3f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}

void PlayScene::CreateViewports()
{
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// --- 左側: ゲーム画面用ビューポート ---
	D3D11_VIEWPORT viewportLeft = {};
	viewportLeft.TopLeftX = 0;
	viewportLeft.TopLeftY = 0;
	viewportLeft.Width = (FLOAT)rect.right * 0.7f; // 例: 左7割
	viewportLeft.Height = (FLOAT)rect.bottom;
	viewportLeft.MinDepth = 0.0f;
	viewportLeft.MaxDepth = 1.0f;
	m_viewPortGame = viewportLeft;

	// --- 右側: 操作画面用ビューポート ---
	D3D11_VIEWPORT viewportRight = {};
	viewportRight.TopLeftX = (FLOAT)rect.right * 0.7f;
	viewportRight.TopLeftY = 0;
	viewportRight.Width = (FLOAT)rect.right * 0.3f; // 例: 右3割
	viewportRight.Height = (FLOAT)rect.bottom;
	viewportRight.MinDepth = 0.0f;
	viewportRight.MaxDepth = 1.0f;
	m_viewPortControll = viewportRight;
}
