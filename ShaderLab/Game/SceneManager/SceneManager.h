/*
	@file	SceneManager.h
	@brief	シーンマネージャクラス
*/
#pragma once
#include "IScene.h"
#include "Game/Scenes/Scene/Scene.h"


// 前方宣言
class CommonResources;

class SceneManager
{
private:
	std::unique_ptr<IScene> m_currentScene;
	CommonResources* m_commonResources;
	IScene::SceneID m_nowSceneID;
public:
	// プレイシーンにステージ番号を渡すための変数
	int m_stageNumber;
public:
	SceneManager();
	~SceneManager();

	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	void ChangeScene(IScene::SceneID sceneID);
	void CreateScene(IScene::SceneID sceneID);
	void ReleaseScene();
	IScene::SceneID GetSceneID() const { return m_nowSceneID; }
	void SetSceneID(IScene::SceneID sceneID) { m_nowSceneID = sceneID; }
};
