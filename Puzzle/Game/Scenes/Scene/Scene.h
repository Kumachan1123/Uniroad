/*
	@file	Scene.h
	@brief	一般的なシーンクラス
*/
#pragma once
#include <cassert>
#include <memory>
#include <DeviceResources.h>

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
class Scene : public IScene
{
public:
	Scene(IScene::SceneID sceneID);
	~Scene()override;
	void Initialize(CommonResources* resources) override;
	void Update(float elapsedTime)override;
	void Render()override;
	void Finalize()override;
	SceneID GetNextSceneID() const override;
private:
	// カメラに関する設定をする
	void CreateCamera();
	// 各種ビューポートを設定する
	void CreateViewports();
private:
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// 固定カメラ
	std::unique_ptr<FixedCamera> m_pFixedCamera;
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
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projectionGame;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projectionControll;
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

};
