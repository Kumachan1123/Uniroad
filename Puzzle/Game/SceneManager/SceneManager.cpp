/*
	@file	SceneManager.cpp
	@brief	シーンマネージャクラス
*/
#include "pch.h"
#include "SceneManager.h"
#include "Game/Screen/Screen.h"
#include "Game/CommonResources/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
SceneManager::SceneManager()
	:
	m_currentScene{},
	m_commonResources{},
	m_stageNumber{ 0 },
	m_nowSceneID{ IScene::SceneID::NONE }
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
SceneManager::~SceneManager()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void SceneManager::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	ChangeScene(IScene::SceneID::LAB);
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void SceneManager::Update(float elapsedTime)
{
	m_currentScene->Update(elapsedTime);

	// 説明用変数：次のシーン
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// シーンを変更しないとき
	if (nextSceneID == IScene::SceneID::NONE) return;

	// シーンを変更するとき
	ChangeScene(nextSceneID);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void SceneManager::Render()
{
	m_currentScene->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void SceneManager::Finalize()
{
	ReleaseScene();
}

//---------------------------------------------------------
// シーンを変更する
//---------------------------------------------------------
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	// DeleteScene();
	CreateScene(sceneID);
}

//---------------------------------------------------------
// シーンを作成する
//---------------------------------------------------------
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
	case IScene::SceneID::LAB:
		m_currentScene = std::make_unique<Scene>(sceneID);
		break;
		/*case IScene::SceneID::TITLE:
			m_currentScene = std::make_unique<TitleScene>(sceneID);
			break;
		case IScene::SceneID::SETTING:
			m_currentScene = std::make_unique<SettingScene>(sceneID);
			break;
		case IScene::SceneID::STAGESELECT:
			m_currentScene = std::make_unique<StageSelectScene>(sceneID);
			break;
		case IScene::SceneID::PLAY:
			m_currentScene = std::make_unique<PlayScene>(sceneID);

			break;
		case IScene::SceneID::CLEAR:
		case IScene::SceneID::GAMEOVER:

			m_currentScene = std::make_unique<ResultScene>(sceneID);
			break;*/
	default:
		assert(!"SceneManager::CreateScene::シーン名が存在しません！");
		// no break
	}


	m_currentScene->Initialize(m_commonResources);
	SetSceneID(sceneID);
}

void SceneManager::ReleaseScene()
{
	m_currentScene.reset();
}

