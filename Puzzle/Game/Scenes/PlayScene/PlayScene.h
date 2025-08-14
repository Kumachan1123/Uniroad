/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
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
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/CSVItem/CSVItem.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
#include "Game/ControllScreen/UIBack/UIBack.h"
#include "Game/ControllScreen/NextTiles/NextTiles.h"
#include "Game/ControllScreen/Panel/Panel.h"
#include "Game/Mouse/Mouse.h"
#include "Game/ControllScreen/MedalCounter/MedalCounter.h"
#include "Game/Scenes/PlayScene/ResultAnimation/ResultAnimation.h"
#include "KumachiLib/Easing/Easing.h"
#include "Game/Scenes/PlayScene/ResultUI/ResultUI.h"
#include "Game/MainScreen/SpeedUpButton/SpeedUpButton.h"
#include "Game/Fade/Fade.h"
#include "Game/Sky/Sky.h"

// 前方宣言
class CommonResources;
class CSVItem;
class MiniCharacterBase;
class NextTiles;
class Panel;
class MedalCounter;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

// 一般的なシーンクラス
class PlayScene : public IScene
{
public:
	// アクセサ
	// シーンIDを取得する
	SceneID GetNextSceneID() const override;
	// ステージ番号を取得する
	int GetStageNumber() const override { return m_stageNumber; }
	// ステージ番号を設定する
	void SetStageNumber(int stageNumber) override { m_stageNumber = stageNumber; }
public:
	// public関数
	// コンストラクタ
	PlayScene(IScene::SceneID sceneID);
	// デストラクタ
	~PlayScene()override;
	// 初期化
	void Initialize(CommonResources* resources) override;
	// 更新
	void Update(float elapsedTime)override;
	// 描画
	void Render()override;
	// 終了
	void Finalize()override;
private:
	// private関数
	// カメラに関する設定をする
	void CreateCamera();
	// 各種ビューポートを設定する
	void CreateViewports();
	// デバッグ文字を表示
	void DrawDebugString();
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// 固定カメラ(プレイ中）
	std::unique_ptr<FixedCamera> m_pFixedCameraPlay;
	// 固定カメラ(リザルト用)
	std::unique_ptr<FixedCamera> m_pFixedCameraResult;
	// CSVマップ
	std::unique_ptr<CSVMap> m_pCSVMap;
	// CSVアイテム
	std::unique_ptr<CSVItem> m_pCSVItem;
	// ミニキャラベース
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// 操作画面の背景
	std::unique_ptr<UIBack> m_pUIBack;
	// パネル
	std::unique_ptr<Panel> m_pPanel;
	// 次のタイル
	std::unique_ptr<NextTiles> m_pNextTiles;
	// マウス
	std::unique_ptr<MyMouse> m_pMouse;
	// メダルカウンター
	std::unique_ptr<MedalCounter> m_pMedalCounter;
	// 結果アニメーション
	std::unique_ptr<ResultAnimation> m_pResultAnimation;
	// 結果UI
	std::unique_ptr<ResultUI> m_pResultUI;
	// スピードアップボタン
	std::unique_ptr<SpeedUpButton> m_pSpeedUpButton;
	// フェード
	std::unique_ptr<Fade> m_pFade;
	// 空
	std::unique_ptr<Sky> m_pSky;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列(ゲーム画面用)
	DirectX::SimpleMath::Matrix m_projectionGame;
	// 射影行列(操作画面用)
	DirectX::SimpleMath::Matrix m_projectionControll;
	// 射影行列(リザルト用)
	DirectX::SimpleMath::Matrix m_projectionResult;
	// 現在のシーンID
	IScene::SceneID m_nowSceneID;
	// ゲーム画面用ビューポート　
	D3D11_VIEWPORT m_viewPortGame;
	// 操作用ビューポート
	D3D11_VIEWPORT m_viewPortControll;
	// 時間
	float m_time;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// ステージ番号
	int m_stageNumber;
	// シーンチェンジカウント
	float m_sceneChangeCount;
};
