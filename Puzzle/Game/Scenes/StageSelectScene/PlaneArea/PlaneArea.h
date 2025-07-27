/*
*	@file PlaneArea.h
*	@brief 平面エリアクラス
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

// 前方宣言
class CommonResources;

// 平面エリアクラス
class PlaneArea
{
public:
	// アクセサ
	// ビュー行列を設定
	void SetView(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// 射影行列を設定
	void SetProjection(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
	// 平面の頂点配列を配列に登録
	void AddPlane(const std::vector<DirectX::SimpleMath::Vector3>& vertices) { m_debugPlaneVerticesPosition.push_back(vertices); }
public:
	// public関数
	// コンストラクタ
	PlaneArea(CommonResources* resources);
	// デストラクタ
	~PlaneArea();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// 終了処理
	void Finalize();
private:
	// private関数
	// マウス座標からワールドレイを生成
	DirectX::SimpleMath::Ray ScreenPointToRay(
		int mouseX, int mouseY,
		int screenWidth, int screenHeight);
	// Planeとレイの交差判定
	bool RayIntersectPlane(
		const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		DirectX::SimpleMath::Vector3& outIntersection);
	bool RayIntersectPlane(const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		const std::vector<DirectX::SimpleMath::Vector3>& rectVertices,
		DirectX::SimpleMath::Vector3& outIntersection);
	// 線描画（デバッグ）
	void DrawDebugLine(const std::vector<DirectX::SimpleMath::Vector3>& vertices);
private:
	// privateメンバ変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 頂点配列
	//DirectX::SimpleMath::Vector3 m_debugPlaneVerticesPosition[4];
	std::vector<std::vector<DirectX::SimpleMath::Vector3>> m_debugPlaneVerticesPosition;
	// 色
	DirectX::SimpleMath::Color m_color;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_pBatch;
	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

};