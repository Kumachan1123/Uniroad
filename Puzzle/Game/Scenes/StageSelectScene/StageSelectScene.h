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
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterSelectStage/MiniCharacterSelectStage.h"
#include "Game/Scenes/StageSelectScene/StageSelect/StageSelect.h"

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
	// カメラに関する設定をする
	void CreateCamera();
	// 各種ビューポートを設定する
	void CreateViewports();
	// マウス座標からワールドレイを生成
	DirectX::SimpleMath::Ray ScreenPointToRay(
		int mouseX, int mouseY,
		int screenWidth, int screenHeight,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
	// Planeとレイの交差判定
	bool RayIntersectPlane(
		const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		DirectX::SimpleMath::Vector3& outIntersection);
	// 線描画（デバッグ）
	void DrawDebugLine(const DirectX::SimpleMath::Vector3 p[4],
		const DirectX::SimpleMath::Color& color,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj);
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// カメラ
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// ミニキャラベース
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// ステージセレクト
	std::unique_ptr<StageSelect> m_pStageSelect;
	// CSVマップ
	std::unique_ptr<CSVMap> m_pCSVMap;
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
	// デバッグ矩形描画フラグ
	bool m_bDrawDebugPlane;
	// 頂点配列
	DirectX::SimpleMath::Vector3 m_debugPlaneVerticesPosition[4];
};

