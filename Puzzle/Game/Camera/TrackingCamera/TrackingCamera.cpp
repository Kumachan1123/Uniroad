/*
*	@file TrackingCamera.cpp
*	@brief 追従カメラクラスの実装
*/
#include "pch.h"
#include "TrackingCamera.h"
// デフォルトのカメラ距離・高さ・角度
const float TrackingCamera::DEFAULT_CAMERA_DISTANCE = 12.5f;
const float TrackingCamera::DEFAULT_CAMERA_HEIGHT = 9.0f;
const float TrackingCamera::DEFAULT_CAMERA_ANGLE = 30.0f;
/*
*	@brief コンストラクタ
*	@details 追従カメラクラスの初期化を行う
*	@param なし
*	@return なし
*/
TrackingCamera::TrackingCamera()
	: m_eye(0.0f, 0.0f, DEFAULT_CAMERA_DISTANCE) // 視点を初期化
	, m_target(0.0f, 0.0f, 0.0f) // 注視点を原点に設定
	, m_up(0.0f, 1.0f, 0.0f) // 上方向をY軸正方向に設定
	, m_sx(0.0f), m_sy(0.0f)// 相対スケールを初期化
	, m_view(DirectX::SimpleMath::Matrix::Identity) // ビュー行列を単位行列で初期化
	, m_direction(0.0f, 0.0f, 0.0f) // カメラの向いている向きを初期化
	, m_cameraDistance(DEFAULT_CAMERA_DISTANCE) // カメラの距離を初期化
	, m_cameraHeight(DEFAULT_CAMERA_HEIGHT) // カメラの高さを初期化
	, m_cameraAngle(DEFAULT_CAMERA_ANGLE) // カメラの角度（Y軸回転）を初期化
{
}
/*
*	@brief 初期化する
*	@details 固定カメラクラスの初期化
*	@param screenWidth 画面の幅
*	@param screenHeight 画面の高さ
*	@return なし
*/
void TrackingCamera::Initialize(int screenWidth, int screenHeight)
{
	// 相対スケールを計算
	CalculateRerativeScale(screenWidth, screenHeight);
	// 終始展の初期値は（0,0,0）に設定
	SetTargetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
	// ビュー行列を計算
	CalculateViewMatrix();
}
/*
*	@brief 更新する
*	@details 固定カメラクラスの更新
*	@param なし
*	@return なし
*/
void TrackingCamera::Update()
{
	// ビュー行列を算出する
	CalculateViewMatrix();
}
/*
*	@brief ビュー行列を計算する
*	@details 固定カメラのビュー行列を計算する
*	@param なし
*	@return なし
*/
void TrackingCamera::CalculateViewMatrix()
{
	using namespace DirectX::SimpleMath;
	// 0.0 ～ 1.0の範囲で「どれだけ速く追従するか」を決める
	float followSpeed = 0.1f; // 0.05～0.2くらいで調整

	// ターゲットのX座標
	float targetX = m_target.x;
	// カメラの現在X座標
	float currentX = m_eye.x;

	// Lerp（線形補間）で少しずつ近づける
	float newX = currentX + (targetX - currentX) * followSpeed;

	// Y/Zは固定
	float camY = m_cameraHeight;
	float camZ = m_cameraDistance;

	// eyeとtargetを更新（targetはXだけ追従）
	m_eye = DirectX::SimpleMath::Vector3(newX, camY, camZ);
	m_target = DirectX::SimpleMath::Vector3(newX, 0.0f, 0.0f);

	// ビュー行列を再計算
	m_up = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
	m_direction = (m_target - m_eye);
	m_direction.Normalize();
}
/*
*	@brief 相対スケールを計算する
*	@details ウィンドウの幅と高さに基づいて相対スケールを計算する
*	@param windowWidth ウィンドウの幅
*	@param windowHeight ウィンドウの高さ
*	@return なし
*/
void TrackingCamera::CalculateRerativeScale(int windowWidth, int windowHeight)
{
	// 画面サイズに対する相対的なスケールに調整
	m_sx = 1.0f / static_cast<float>(windowWidth);
	m_sy = 1.0f / static_cast<float>(windowHeight);
}
