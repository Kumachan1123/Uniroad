/*
*	@file FixedCamera.h
*	@brief 固定カメラクラス
*	@details 特定の位置と方向を持ち、常にその位置からシーンを描画する
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/Interface/ICamera.h"

// 固定カメラクラス
class FixedCamera : public ICamera
{
public:
	// アクセサ
	// ビュー行列（カメラ行列）を取得する
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const  override { return m_view; }
	// ビュー行列（カメラ行列）を設定する
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view)  override { m_view = view; }
	// 射影行列を取得する
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const  override { return m_projection; }
	// 射影行列を設定する
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj)  override { m_projection = proj; }
	// 視点を取得する
	const DirectX::SimpleMath::Vector3& GetEyePosition() const  override { return m_eye; }
	// 視点を設定する
	void SetEyePosition(const DirectX::SimpleMath::Vector3& eye)  override { m_eye = eye; }
	// 注視点を取得する
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const  override { return m_target; }
	// 注視点を設定する
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target)  override { m_target = target; }
	// カメラの頭の方向を取得する
	const DirectX::SimpleMath::Vector3& GetUpPosition() const  override { return m_up; }
	// カメラの距離を取得する
	const DirectX::SimpleMath::Vector3& GetCameraDistance() const { return m_cameraDistance; }
	// カメラの距離を設定する
	void SetCameraDistance(const DirectX::SimpleMath::Vector3& distance) { m_cameraDistance = distance; }
public:
	// publicなメンバ関数
	// コンストラクタ
	FixedCamera();
	// デストラクタ
	~FixedCamera() = default;
	// 初期化する
	void Initialize(int screenWidth, int screenHeight);
	// 更新する
	void Update();
private:
	// ビュー行列を計算する
	void CalculateViewMatrix();
	// 相対スケールを計算する
	void CalculateRerativeScale(int windowWidth, int windowHeight);
private:
	// デフォルトでカメラが注視点からどのくらい離れているか
	static const float DEFAULT_CAMERA_DISTANCE;
	// 相対スケール
	float m_sx, m_sy;
	// 生成されたビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// 視点
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;
	// カメラの頭の方向
	DirectX::SimpleMath::Vector3 m_up;
	// カメラの距離
	DirectX::SimpleMath::Vector3 m_cameraDistance;
};