/*
	@file	Scene.cpp
	@brief	一般的なシーンクラス
*/
#include <pch.h>
#include "Scene.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;




Scene::Scene(IScene::SceneID sceneID)
	: m_commonResources{}
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

Scene::~Scene()
{
}

void Scene::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	CreateCamera();
	const auto deviceResources = m_commonResources->GetDeviceResources();
	// 各種ビューポートを作成する
	CreateViewports();
	// マウスを作成する
	m_pMouse = std::make_unique<MyMouse>();
	// マウスを初期化する
	m_pMouse->Initialize(m_commonResources);
	// マウスにビューポートを設定
	m_pMouse->SetViewport(m_viewPortControll);
	// マップ生成
	m_pCSVMap = std::make_unique<CSVMap>(m_commonResources);
	// CSVマップを読み込む
	m_pCSVMap->LoadMap("Resources/Map/test.csv");
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// ミニキャラベースにCSVマップを設定
	m_pMiniCharacterBase->SetCSVMap(m_pCSVMap.get());
	// ミニキャラを初期化する
	m_pMiniCharacterBase->Initialize(m_commonResources);
	// ミニキャラベースにミニキャラをアタッチ
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacter>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));

	// 操作画面の背景を作成する
	m_pUIBack = std::make_unique<UIBack>(m_commonResources);
	// 操作画面の背景を初期化する
	m_pUIBack->Create(deviceResources);
	// パネルを作成する
	m_pPanel = std::make_unique<Panel>(m_pCSVMap->GetMaxCol(), m_pCSVMap->GetMaxRow());
	// パネルにマウスを設定
	m_pPanel->SetMouse(m_pMouse.get());
	// パネルにマップ情報を渡す
	m_pPanel->SetCSVMap(m_pCSVMap.get());
	// パネルを初期化する
	m_pPanel->Initialize(m_commonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// 次のタイルを作成する
	m_pNextTiles = std::make_unique<NextTiles>();
	// 次のタイルにマウスを設定
	m_pNextTiles->SetMouse(m_pMouse.get());
	// 次のタイルにマップ情報を渡す
	m_pNextTiles->SetCSVMap(m_pCSVMap.get());
	// 次のタイルを初期化する
	m_pNextTiles->Initialize(m_commonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);

}

void Scene::Update(float elapsedTime)
{
	m_time += elapsedTime;
	//// デバッグカメラの更新
	//m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// マウスの更新
	m_pMouse->Update(elapsedTime);
	// 操作画面の背景の更新
	m_pUIBack->Update(elapsedTime);
	// パネルの更新
	m_pPanel->Update(elapsedTime);
	// 次のタイルの更新
	m_pNextTiles->Update(elapsedTime);
	// ミニキャラの更新
	Vector3 position(0.0f, 0.0f, 0.0f);
	Quaternion angle(Quaternion::Identity);
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
}
void Scene::Render()
{
	const auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	RECT rect = m_commonResources->GetDeviceResources()->GetOutputSize();

	// --- 左側: ゲーム画面用ビューポート ---
	context->RSSetViewports(1, &m_viewPortGame);

	// ここでゲーム画面を描画
	// ビュー行列を取得
	m_view = m_pFixedCamera->GetViewMatrix();
	// CSVマップの描画
	m_pCSVMap->Render(m_view, m_projectionGame);
	// ミニキャラの描画
	m_pMiniCharacterBase->Render(m_view, m_projectionGame);

	// --- 右側: 操作画面用ビューポート ---
	context->RSSetViewports(1, &m_viewPortControll);
	// 操作画面の背景を描画
	m_pUIBack->Render();
	// パネルを描画
	m_pPanel->Render();
	// 次のタイルを描画
	m_pNextTiles->Render();


	// --- デバッグ情報（例） ---
	// ビューポートを元の設定に戻す
	const auto& viewPort = m_commonResources->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewPort);

	const auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Use ViewPort.");
}

void Scene::Finalize()
{
}

IScene::SceneID Scene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::LAB;// ゲームオーバーシーンへ
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;// 何もしない
}

//---------------------------------------------------------
// カメラ、ビュー行列、射影行列を作成する
//---------------------------------------------------------
void Scene::CreateCamera()
{
	// デバッグカメラを作成する
	RECT rect = m_commonResources->GetDeviceResources()->GetOutputSize();
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

void Scene::CreateViewports()
{
	RECT rect = m_commonResources->GetDeviceResources()->GetOutputSize();
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
