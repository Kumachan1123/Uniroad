/*
*	@file	SceneManager.h
*	@brief	シーンマネージャクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "IScene.h"
#include "Game/Scenes/StageSelectScene/StageSelectScene.h"
#include "Game/Scenes/PlayScene/PlayScene.h"

// 前方宣言
class CommonResources;

// シーンマネージャクラス
class SceneManager
{

public:
	// publicメンバ関数
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// 終了
	void Finalize();
private:
	// privateメンバ関数
	// シーン変更
	void ChangeScene(IScene::SceneID sceneID);
	// シーン作成
	void CreateScene(IScene::SceneID sceneID);
	// シーン削除
	void DeleteScene();
	// シーンID取得
	IScene::SceneID GetSceneID() const { return m_nowSceneID; }
	// シーンID設定
	void SetSceneID(IScene::SceneID sceneID) { m_nowSceneID = sceneID; }
private:
	// privateメンバ変数
	// 現在のシーン
	std::unique_ptr<IScene> m_pCurrentScene;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
	// プレイシーンにステージ番号を渡すための変数
	int m_stageNumber;

};
