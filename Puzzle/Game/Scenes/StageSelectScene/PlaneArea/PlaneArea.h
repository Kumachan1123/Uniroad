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
	// 平面の色を設定
	void SetPlaneColor(const DirectX::SimpleMath::Color& color) { m_debugPlaneVerticesColor.push_back(color); }
	// 指定された平面の中心座標を取得する
	DirectX::SimpleMath::Vector3 GetPlanePosition(int index) const;
	// 平面の中心座標を登録する
	void AddPlanePosition(const DirectX::SimpleMath::Vector3& position) { m_positions.push_back(position); }
	// 当たった平面の番号を取得する
	int GetHitPlaneIndex() const { return m_hitPlaneIndex; }
	// 当たった平面の番号を設定する
	void SetHitPlaneIndex(int index) { m_hitPlaneIndex = index; }
	// 何らかの平面と当たっているかを取得する
	bool IsHitPlane() const { return m_isHitPlane; }
	// 何らかの平面と当たっているかを設定する
	void SetIsHitPlane(bool isHit) { m_isHitPlane = isHit; }

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
	DirectX::SimpleMath::Ray ScreenPointToRay(int mouseX, int mouseY, int screenWidth, int screenHeight);
	// Planeとレイの交差判定
	bool RayIntersectPlane(int index, const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		const std::vector<DirectX::SimpleMath::Vector3>& rectVertices,
		DirectX::SimpleMath::Vector3& outIntersection);
	// 線描画（デバッグ）
	void DrawDebugLine(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const DirectX::SimpleMath::Color& color);
private:
	// privateメンバ変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 頂点配列
	std::vector<std::vector<DirectX::SimpleMath::Vector3>> m_debugPlaneVerticesPosition;
	// 色
	std::vector<DirectX::SimpleMath::Color> m_debugPlaneVerticesColor;
	// 平面の中心座標
	std::vector<DirectX::SimpleMath::Vector3> m_positions;
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
	// 当たった平面の番号
	int m_hitPlaneIndex;
	// 何らかの平面と当たっているかのフラグ
	bool m_isHitPlane;

};