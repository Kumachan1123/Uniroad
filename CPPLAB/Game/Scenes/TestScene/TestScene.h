/*
*	@file	TestScene.h
*	@brief	テストシーンクラス
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
#include "Game/Fade/Fade.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterTitle/MiniCharacterTitle.h"
#include "Game/Sky/Sky.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"
#include "KumachiLib/OutLine/OutLine.h"
#include "KumachiLib/UIText/UIText.h"
// 前方宣言
class CommonResources;
class MiniCharacterBase;

// テストシーンクラス
class TestScene : public IScene
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
	TestScene(IScene::SceneID sceneID);
	// デストラクタ
	~TestScene() override;
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

private:
	// private定数
	// フェード開始時間
	static constexpr float FADE_START_TIME = 0.5f;
	// 天球のスケール
	static constexpr DirectX::SimpleMath::Vector3 SKY_SCALE = { 0.2f, 0.2f, 0.2f };
	// 天球の回転速度
	static constexpr float SKY_ROTATION_SPEED = 1.0f;
	// ひつじの初期生成位置
	static constexpr DirectX::SimpleMath::Vector3 SHEEP_START_POS = { -10.0f, -0.45f, 0.0f };
	// カメラの位置
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_POSITION = { 0.0f, 1.8f, 5.0f };
	// カメラの注視点の補正値
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_LOOK_OFFSET = { 0.0f, 2.5f, 0.0f };
	// シャドウマップライトの位置
	static constexpr DirectX::SimpleMath::Vector3 SHADOW_MAP_LIGHT_POSITION = { 0.0f, 30.0f, 0.0f };
	// 視野角(FOV)の角度（デグリー）
	static constexpr float FOV = 60.0f;
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 次のシーンID
	IScene::SceneID m_nextSceneID;
	// カメラ
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// フェード
	std::unique_ptr<Fade> m_pFade;
	// シャドウマップライト
	std::unique_ptr<ShadowMapLight> m_pShadowMapLight;
	// 輪郭線描画
	std::unique_ptr<OutLine> m_pOutLine;
	// UIテキスト
	std::unique_ptr<UIText> m_pUIText;
	// シーンチェンジフラグ
	bool m_isChangeScene;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 時間
	float m_time;

};