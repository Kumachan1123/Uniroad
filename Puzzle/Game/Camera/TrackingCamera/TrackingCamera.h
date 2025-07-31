/*
*	@file TrackingCamera.h
*	@brief 追従カメラクラス
*	@details 特定の方向を持ち、常に対象を追いかけシーンを描画する
*/
#pragma once
// 追従カメラクラス
class TrackingCamera
{
public:
	// アクセサ
	// ビュー行列（カメラ行列）を取得する
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// 視点を取得する
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }
	// 注視点を取得する
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// 注視点を設定する
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) { m_target = target; }
	// カメラの頭の方向を取得する
	const DirectX::SimpleMath::Vector3& GetUpPosition() const { return m_up; }
	// カメラの向いている向きを取得する
	const DirectX::SimpleMath::Vector3& GetDirection() const { return m_direction; }
	// カメラの距離を取得する
	float GetCameraDistance() const { return m_cameraDistance; }
	// カメラの高さを取得する
	float GetCameraHeight() const { return m_cameraHeight; }
	// カメラの角度（Y軸回転）を取得する	
	float GetCameraAngle() const { return m_cameraAngle; }
	// カメラのパラメータを設定する
	void SetCameraParameters(float distance, float height, float angle)
	{
		m_cameraDistance = distance;
		m_cameraHeight = height;
		m_cameraAngle = angle;
	}
public:
	// publicなメンバ関数
	// コンストラクタ
	TrackingCamera();
	// デストラクタ
	~TrackingCamera() = default;
	// 初期化する
	void Initialize(int screenWidth, int screenHeight);
	// 更新する
	void Update();

private:
	// private関数
	// ビュー行列を計算する
	void CalculateViewMatrix();
	// 相対スケールを計算する
	void CalculateRerativeScale(int windowWidth, int windowHeight);
private:
	// デフォルト値
	// カメラが注視点からどのくらい離れているか
	static const float DEFAULT_CAMERA_DISTANCE;
	// カメラの高さ
	static const float DEFAULT_CAMERA_HEIGHT;
	// カメラの角度（Y軸回転）
	static const float DEFAULT_CAMERA_ANGLE;
private:
	// privateメンバ変数
	// 相対スケール
	float m_sx, m_sy;
	// 生成されたビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 視点
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;
	// カメラの頭の方向
	DirectX::SimpleMath::Vector3 m_up;
	// カメラの向いている向き
	DirectX::SimpleMath::Vector3 m_direction;
	// カメラのパラメータ
	// カメラの距離
	float m_cameraDistance;
	// カメラの高さ
	float m_cameraHeight;
	// カメラの角度（Y軸回転）
	float m_cameraAngle;
};