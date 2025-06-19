/*
	@file	Scene.cpp
	@brief	一般的なシーンクラス
*/
#include "pch.h"
#include "Scene.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;




Scene::Scene(IScene::SceneID sceneID)
	: m_commonResources{}
	, m_debugCamera{}
	, m_gridFloor{}
	, m_projection{}
	, m_time{ 0.0f }
	, m_isChangeScene{}
{
}

Scene::~Scene()
{
}

void Scene::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	this->CreateCamera();

}

void Scene::Update(float elapsedTime)
{
	// デバッグカメラの更新
	m_debugCamera->Update(m_commonResources->GetInputManager());
}

void Scene::Render()
{
	// ビュー行列を取得する
	m_view = m_debugCamera->GetViewMatrix();
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
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}