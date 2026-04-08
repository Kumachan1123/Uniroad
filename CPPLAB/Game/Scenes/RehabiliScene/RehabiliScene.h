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
#include "Game/CommonResources/CommonResources.h"
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/Camera/TPCamera/TPCamera.h"
#include "Game/Fade/Fade.h"
#include "Game/Android/Android.h"
#include "Game/Puyo/Puyo.h"
#include "Game/PuyoGrid/PuyoGrid.h"
#include "KumachiLib/Math/KumachiLib.h"

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
	void Initialize(CommonResources* resources) override;
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
	// 新しい落下ぷよペアを生成する
	void GeneratePuyo();
	// 落下中ぷよの更新（移動・回転入力、自然落下、着地判定）
	void UpdateFallingPuyo(float elapsedTime);
	// 落下中ぷよの平行移動を試行する
	bool TryMoveFallingPuyo(int dx, int dy);
	// 落下中ぷよの回転を試行する（true:右回転 / false:左回転）
	void TryRotateFallingPuyo(bool isRightRotation);
	// 落下中ぷよを固定盤面へ配置する
	void LockFallingPuyo();
	// 盤面の連鎖解決
	// 1秒ごとに「消去判定」と「重力落下」を交互に進める。
	void ResolveBoard(float elapsedTime);
	// 4つ以上連結したぷよを消去する
	bool EraseConnectedPuyo();
	// 同色連結を深さ優先で収集する
	void CollectConnectedPuyo(int col, int row, Puyo::PuyoColor color,
							  bool visited[6][12], std::vector<std::pair<int, int>>& outCells) const;
	// 重力で盤面を下詰めする
	bool ApplyGravityToBoard();
	// 指定マスの固定ぷよを作り直す
	void RecreateFixedPuyo(int col, int row, Puyo::PuyoColor color);
	// 落下中ぷよの表示位置を盤面座標から再同期する
	void SyncFallingPuyoPosition();
	// 盤面内判定
	bool IsInsideBoard(int col, int row) const;
	// 空きマス判定（固定盤面ベース）
	bool IsCellEmpty(int col, int row) const;
	// 落下中ぷよの占有判定（デバッグ用）
	bool IsFallingCellAt(int col, int row) const;
	// 盤面座標から描画座標へ変換
	DirectX::SimpleMath::Vector2 GridToPosition(int col, int row) const;
private:
	// private定数
	// 視野角(FOV)の角度（デグリー）
	static constexpr float FOV = 60.0f;
	// 自然落下間隔（秒）
	static constexpr float FALL_INTERVAL = 1.0f;

private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 次のシーンID
	IScene::SceneID m_nextSceneID;
	// 三人称カメラ
	std::unique_ptr<TPCamera> m_pTPCamera;
	// 固定されたぷよ（盤面本体）
	std::unique_ptr<Puyo> m_pFixedPuyo[6][12];
	// 落ちてきてるぷよ（3x3の相対スロットで管理）
	std::unique_ptr<Puyo> m_pFallingPuyo[3][3];
	// 落下ペア中心の列（軸ぷよ）
	int m_fallCenterCol;
	// 落下ペア中心の行（軸ぷよ）
	int m_fallCenterRow;
	// 子ぷよの相対列（-1/0/1）
	int m_subOffsetCol;
	// 子ぷよの相対行（-1/0/1）
	int m_subOffsetRow;
	// 自然落下の経過時間
	float m_fallTimer;
	// 盤面解決中フラグ（重力・消去をフレーム分割で進める）
	bool m_isResolving;
	// 次ステップで重力適用するか
	bool m_resolveNeedsGravity;
	// 次ステップで消去判定するか
	bool m_resolveNeedsErase;
	// 連鎖解決ステップ用タイマー
	float m_resolveTimer;
	// 左入力の前フレーム状態（エッジ判定用）
	bool m_prevLeftKey;
	// 右入力の前フレーム状態（エッジ判定用）
	bool m_prevRightKey;
	// A入力の前フレーム状態（左回転エッジ判定用）
	bool m_prevAKey;
	// D入力の前フレーム状態（右回転エッジ判定用）
	bool m_prevDKey;
	// 落下中ぷよが存在するか
	bool m_isFalling;
	// グリッド背景
	std::unique_ptr<PuyoGrid> m_pPuyoGrid[6][12];
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
};