/*
*	@file StageSelectScene.h
*	@brief ステージセレクトシーンクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
// DirectX
#include <DeviceResources.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
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
#include "KumachiLib/FileCounter/FileCounter.h"
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/Camera/TrackingCamera/TrackingCamera.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterSelectStage/MiniCharacterSelectStage.h"
#include "Game/Scenes/StageSelectScene/StageSelect/StageSelect.h"
#include "Game/Scenes/StageSelectScene/PlaneArea/PlaneArea.h"
#include "Game/Scenes/StageSelectScene/StageGate/StageGate.h"
#include "Game/Sky/Sky.h"
#include "Game/Fade/Fade.h"

// 前方宣言
class CommonResources;
class MiniCharacterBase;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

// ステージセレクトシーンクラス
class StageSelectScene : public IScene
{
public:
	// アクセサ
	// ステージ番号を取得する
	int GetStageNumber() const override { return m_stageNumber; }
	// ステージ番号を設定する
	void SetStageNumber(int stageNumber) override { m_stageNumber = stageNumber; }
public:
	// public関数
	// コンストラクタ
	StageSelectScene(IScene::SceneID sceneID);
	// デストラクタ
	~StageSelectScene() override;
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
	// private関数
	// カメラに関する設定をする
	void CreateCamera();
	// 平面の中心座標を基に４つの頂点を設定する
	std::vector<DirectX::SimpleMath::Vector3> CreatePlaneVertices(const DirectX::SimpleMath::Vector3& center, float width, float depth, float y = 0.5f) const;
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 固定カメラ
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// トラッキングカメラ
	std::unique_ptr<TrackingCamera> m_pTrackingCamera;
	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// ミニキャラベース
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// ステージセレクト
	std::unique_ptr<StageSelect> m_pStageSelect;
	// ステージの入り口
	std::vector<std::unique_ptr<StageGate>> m_pStageGates;
	// 平面
	std::unique_ptr<PlaneArea> m_pPlaneArea;
	// 空
	std::unique_ptr<Sky> m_pSky;
	// フェード
	std::unique_ptr<Fade> m_pFade;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// ステージ番号
	int m_stageNumber;
	// 次のシーンID
	IScene::SceneID m_nextSceneID;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// グリッド床
	std::unique_ptr<mylib::GridFloor> m_pGridFloor;
	// ミニキャラの移動フラグ
	bool m_isMiniCharacterMove;
};

