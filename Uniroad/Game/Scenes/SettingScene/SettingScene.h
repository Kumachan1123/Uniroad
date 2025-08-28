/*
	@file	SettingScene.h
	@brief	設定画面シーンクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/SceneManager/IScene.h"
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/MouseClick/MouseClick.h"
#include "Game/Fade/Fade.h"
#include "Game/Scenes/StageSelectScene/BackButton/BackButton.h"
#include "Game/Scenes/SettingScene/SettingBar/SettingBar.h"
#include "Game/Scenes/SettingScene/MinTexture/MinTexture.h"
#include "Game/Scenes/SettingScene/MaxTexture/MaxTexture.h"
#include "Game/Scenes/SettingScene/SettingPanel/SettingPanel.h"
#include "Game/Scenes/SettingScene/BGMTexture/BGMTexture.h"
#include "Game/Scenes/SettingScene/SETexture/SETexture.h"
#include "Game/Scenes/SettingScene/Back/Back.h"

// 前方宣言
class CommonResources;

// 設定画面シーンクラス
class SettingScene : public IScene
{
public:
	// アクセサ
	// ステージ番号を取得する
	int GetStageNumber() const override { return 0; }
	// ステージ番号を設定する
	void SetStageNumber(int stageNumber) override { stageNumber; }
public:
	// public関数
	// コンストラクタ
	SettingScene(IScene::SceneID sceneID);
	// デストラクタ
	~SettingScene() override;
	// 初期化
	void Initialize(CommonResources* resources) override;
	// 更新
	void Update(float elapsedTime) override;
	// 描画
	void Render() override;
	// 終了
	void Finalize() override;
	// シーンIDを取得する
	SceneID GetNextSceneID() const override;
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// フェード
	std::unique_ptr<Fade> m_pFade;
	// ボタン
	std::unique_ptr<BackButton> m_pBackButton;
	// 設定バー
	std::unique_ptr<SettingBar> m_pSettingBar;
	// Minテクスチャ
	std::vector<std::unique_ptr<MinTexture>> m_pMinTextures;
	// Maxテクスチャ
	std::vector<std::unique_ptr<MaxTexture>> m_pMaxTextures;
	// 設定画面のパネル
	std::unique_ptr<SettingPanel> m_pSettingPanel;
	// BGM画像
	std::unique_ptr<BGMTexture> m_pBGMTexture;
	// SE画像
	std::unique_ptr<SETexture> m_pSETexture;
	// 背景画像
	std::unique_ptr<Back> m_pBack;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 次のシーンID
	IScene::SceneID m_nextSceneID;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 時間
	float m_time;
};