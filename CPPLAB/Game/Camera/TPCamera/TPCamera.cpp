#include "pch.h"
#include "TPCamera.h"
// 通常時のカメラの距離
const float TPCamera::DEFAULT_TPCAMERA_DISTANCE = 12.5f;
TPCamera::TPCamera()
	:m_eye(0.0f, 0.0f, DEFAULT_TPCAMERA_DISTANCE) // 視点を初期化
	, m_target(0.0f, 0.0f, 0.0f) // 注視点を原点に設定
	, m_up(DirectX::SimpleMath::Vector3::Up) // 上方向をY軸正方向に設定
	, m_sx(0.0f), m_sy(0.0f) // 相対スケールを初期化
	, m_cameraDistance(DEFAULT_TPCAMERA_DISTANCE) // カメラの距離をデフォルト値に設定
	, m_view(DirectX::SimpleMath::Matrix::Identity) // ビュー行列を単位行列で初期化
	, m_projection(DirectX::SimpleMath::Matrix::Identity) // 射影行列を単位行列で初期化
	, m_time(0.0f) // 時間を初期化
{}

void TPCamera::Initialize(int screenWidth, int screenHeight)
{
	// 名前空間を使用
	using namespace DirectX::SimpleMath;
	// 相対スケールを計算
	CalculateRerativeScale(screenWidth, screenHeight);
	// カメラの距離をデフォルト値に設定
	m_cameraDistance = Vector3(DEFAULT_TPCAMERA_DISTANCE);
	// ビュー行列を計算
	CalculateViewMatrix();
	// 視点をデフォルトの位置に設定
	// 視点を設定
	Vector3 eye(0.0f, 1.0f, 1.0f);
	// デフォルトのカメラ距離を適用
	eye *= m_cameraDistance;
	// 視点を設定
	m_eye = eye;
}

void TPCamera::Update()
{
	// ビュー行列を算出する
	CalculateViewMatrix();
}

void TPCamera::CalculateViewMatrix()
{
	// 名前空間を使用
	using namespace DirectX::SimpleMath;

	if (MyResources::Get().GetInputManager())
	{
		auto keyState = MyResources::Get().GetInputManager()->GetKeyboardState();
		// 左右矢印でカメラの視点を回転させる
		float rotateAngle = 0.0f;
		const float rotateSpeed = 0.03f;
		if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Left))
		{
			rotateAngle += rotateSpeed;
		}
		if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Right))
		{
			rotateAngle -= rotateSpeed;
		}

		if (rotateAngle != 0.0f)
		{
			Vector3 offset = m_eye - m_target;
			offset = Vector3::Transform(offset, Matrix::CreateRotationY(rotateAngle));
			m_eye = m_target + offset;
		}
	}

	// 上方向を設定
	m_up = Vector3::Up;
	// ビュー行列を計算する
	m_view = Matrix::CreateLookAt(m_eye, m_target, m_up);
}

void TPCamera::CalculateRerativeScale(int windowWidth, int windowHeight)
{
	// 画面サイズに対する相対的なスケールに調整
	m_sx = 1.0f / static_cast<float>(windowWidth);
	m_sy = 1.0f / static_cast<float>(windowHeight);
}
