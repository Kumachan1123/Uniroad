/*
*	@file	SceneManager.cpp
*	@brief	シーンマネージャクラス
*/
#include <pch.h>
#include "SceneManager.h"
/*
*	@brief コンストラクタ
*	@details シーンマネージャクラスのコンストラクタ
*	@param なし
*	@return なし
*/
SceneManager::SceneManager()
	: m_pCurrentScene{}// 現在のシーン
	, m_pCommonResources{}// 共通リソース
	, m_stageNumber{ 0 }// ステージ番号
	, m_nowSceneID{ IScene::SceneID::NONE }// 現在のシーンID
{
}
/*
*	@brief デストラクタ
*	@details シーンマネージャクラスのデストラクタ
*	@param なし
*	@return	なし
*/
SceneManager::~SceneManager() { Finalize(); }
/*
*	@brief 初期化する
*	@details シーンマネージャクラスの初期化
*	@param resources 共通リソース
*	@return なし
*/
void SceneManager::Initialize(CommonResources* resources)
{
	// リソースがnullptrでないことを確認
	assert(resources);
	// 共通リソースを取得
	m_pCommonResources = resources;
	// タイトルシーンに変更
	ChangeScene(IScene::SceneID::STAGESELECT);
}
/*
*	@brief 更新する
*	@details シーンマネージャクラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void SceneManager::Update(float elapsedTime)
{
	// 現在のシーンを更新
	m_pCurrentScene->Update(elapsedTime);
	// 次のシーンIDがNONEの場合はここで処理を終わる
	if (m_pCurrentScene->GetNextSceneID() == IScene::SceneID::NONE) return;
	// シーンを変更するとき
	ChangeScene(m_pCurrentScene->GetNextSceneID());
}
/*
*	@brief 描画する
*	@details シーンマネージャクラスの描画
*	@param なし
*	@return なし
*/
void SceneManager::Render()
{
	// 現在のシーンを描画する
	m_pCurrentScene->Render();
}
/*
*	@brief 終了する
*	@details シーンを削除する
*	@param なし
*	@return なし
*/
void SceneManager::Finalize()
{
	// 現在のシーンを削除する
	DeleteScene();
}

/*
*	@brief シーンを変更する
*	@details 今のシーンを消して新しいシーンを作成する
*	@param sceneID 新しいシーンのID
*	@return なし
*/
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{

	// シーンを削除する
	DeleteScene();
	// 新しいシーンを作成する
	CreateScene(sceneID);
}

/*
*	@brief シーンを作成する
*	@details 新しいシーンを作成する
*	@param sceneID 新しいシーンのID
*	@return なし
*/
void SceneManager::CreateScene(IScene::SceneID sceneID)
{

	// 現在のシーンがnullptrであることを確認
	assert(m_pCurrentScene == nullptr);
	// シーンIDによって処理を分ける
	switch (sceneID)
	{
	case IScene::SceneID::STAGESELECT:
		m_pCurrentScene = std::make_unique<StageSelectScene>(sceneID);
		break;
	case IScene::SceneID::PLAY:
		m_pCurrentScene = std::make_unique<PlayScene>(sceneID);
		m_pCurrentScene->SetStageNumber(m_stageNumber); // ステージ番号を設定
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


	m_pCurrentScene->Initialize(m_pCommonResources);
	SetSceneID(sceneID);
}
/*
*	@brief シーンを削除する
*	@details 現在のシーンを削除する(セレクトシーンの場合は消す前にステージ番号を保存する)
*	@param なし
*	@return なし
*/
void SceneManager::DeleteScene()
{
	// ステージセレクトシーンの場合は、ステージ番号をマネージャーに渡す
	if (m_nowSceneID == IScene::SceneID::STAGESELECT)m_stageNumber = m_pCurrentScene->GetStageNumber();
	// 現在のシーンをリセットする
	m_pCurrentScene.reset();
}


