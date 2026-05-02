/*
*		@file    RehabiliScene.h
*		@brief   ゲームづくりのリハビリ用シーンクラス
*		@details ぷよ盤面の生成・落下操作・設置・消去・重力処理を管理する。
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
#include <vector>
#include <utility>
// DirectX
#include <DeviceResources.h>
#include <d3d11.h>
#include <SimpleMath.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/SceneManager/IScene.h"
#include <Game\MyResources\MyResources.h>
#include "Game/CommonResources/CommonResources.h"
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/Camera/TPCamera/TPCamera.h"
#include "Game/Fade/Fade.h"
#include "Game/Android/Android.h"
#include "Game/Puyo/Puyo.h"
#include "Game/PuyoGrid/PuyoGrid.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "Game/PuyoManager/PuyoManager.h"
#include <KumachiLib\BillboardSprite\BillboardSprite.h>
#include <Game\Player2D\Player2D.h>
#include <Game\TileMap\TileMap.h>
// 前方宣言
class CommonResources;

// ゲームづくりのリハビリ用シーンクラス
// 6x12の盤面を持ち、落下中の2連ぷよ（軸＋子）を入力で操作する。
// 設置後は盤面解決（下詰め・4連結消去・連鎖）を行う。
class RehabiliScene : public IScene
{
public:
	// アクセサ
	// ステージ番号を取得する（リハビリシーンは固定値0）
	int GetStageNumber() const override { return 0; }
	// ステージ番号を設定する（現在は未使用）
	void SetStageNumber(int stageNumber) override { stageNumber; }
public:
	// public関数
	// コンストラクタ
	RehabiliScene(IScene::SceneID sceneID);
	// デストラクタ
	~RehabiliScene() override;
	// 初期化
	// カメラ生成、グリッド生成、固定盤面初期化を行う。
	void Initialize() override;
	// 更新
	// 入力処理、落下更新、設置判定、盤面更新を行う。
	void Update(float elapsedTime) override;
	// 描画
	// グリッド、固定ぷよ、落下中ぷよの順で描画する。
	void Render() override;
	// 終了
	void Finalize() override;
	// シーンIDを取得する
	SceneID GetNextSceneID() const override;

private:
	// private関数
	// カメラに関する設定をする
	void CreateCamera();
	// SDKメッシュを作る（現状は未使用）
	void CreateSDKMesh(std::wstring name);

private:
	// private定数
	// 視野角(FOV)の角度（デグリー）
	static constexpr float FOV = 60.0f;
private:
	// privateメンバ変数
	// 次のシーンID
	IScene::SceneID m_nextSceneID;
	// 三人称カメラ
	std::unique_ptr<TPCamera> m_pTPCamera;

	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 経過時間（次ぷよ生成までの待機に使用）
	float m_time;
	// モデル（未使用）
	std::unique_ptr<DirectX::Model> m_pModel;
	// Android（未使用）
	std::unique_ptr<Android> m_pAndroid;
	// 2Dプレイヤー
	std::unique_ptr<Player2D> m_pPlayer2D;
	// ?t?@?~???[???n?
	std::unique_ptr<TileMap> m_pTileMap;

};