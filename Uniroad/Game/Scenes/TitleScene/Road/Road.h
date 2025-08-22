/*
*	@file Road.h
*	@brief 道路の描画を行うクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <vector>
#include <memory>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// 前方宣言
class CommonResources;

// 道路の描画を行うクラス
class Road
{
public:
	// アクセサ
	// シャドウマップライトを取得する
	ShadowMapLight* GetShadowMapLight() const { return m_pShadowMapLight; }
	// シャドウマップライトを設定する
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) { m_pShadowMapLight = shadowMapLight; }
public:
	// public関数
	// コンストラクタ
	Road(CommonResources* resources);
	// デストラクタ
	~Road();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();
private:
	// private関数
	// 深度ステンシルバッファを設定する
	void CreateDepthStencilBuffer(ID3D11Device* pDevice);
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// シャドウマップライト
	ShadowMapLight* m_pShadowMapLight;
	// 深度ステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	// 道路のモデルへのポインタ
	std::vector<DirectX::Model*> m_pModels;
	// 道路の位置
	std::vector<DirectX::SimpleMath::Vector3> m_positions;
	// 道路の回転角
	DirectX::SimpleMath::Quaternion m_rotation;
	// 道路のワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビューマトリックス
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクションマトリックス
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
	// スクロール速度
	float m_scrollSpeed;
};