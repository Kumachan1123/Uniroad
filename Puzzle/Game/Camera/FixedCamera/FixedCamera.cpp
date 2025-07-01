/*
*	@file FixedCamera.cpp
*	@brief 固定カメラクラス
*	@details 特定の位置と方向を持ち、常にその位置からシーンを描画する
*/
#include <pch.h>
#include "FixedCamera.h"
const float FixedCamera::DEFAULT_CAMERA_DISTANCE = 12.5f;
/*
*	@brief コンストラクタ
*	@details 固定カメラクラスのコンストラクタ
*	@param なし
*	@return なし
*/
FixedCamera::FixedCamera()
	: m_eye(0.0f, 0.0f, DEFAULT_CAMERA_DISTANCE) // 視点を初期化
	, m_target(0.0f, 0.0f, 0.0f)// 注視点を原点に設定
	, m_up(0.0f, 1.0f, 0.0f) // 上方向をY軸正方向に設定
	, m_sx(0.0f), m_sy(0.0f)// 相対スケールを初期化
	, m_view(DirectX::SimpleMath::Matrix::Identity) // ビュー行列を単位行列で初期化
{
}
/*
*	@brief 初期化する
*	@details 固定カメラクラスの初期化
*	@param screenWidth 画面の幅
*	@param screenHeight 画面の高さ
*	@return なし
*/
void FixedCamera::Initialize(int screenWidth, int screenHeight)
{
	// 相対スケールを計算
	CalculateRerativeScale(screenWidth, screenHeight);
	// ビュー行列を計算
	CalculateViewMatrix();
}
/*
*	@brief 更新する
*	@details 固定カメラクラスの更新
*	@param なし
*	@return なし
*/
void FixedCamera::Update()
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
void FixedCamera::CalculateViewMatrix()
{
	using namespace DirectX::SimpleMath;
	// 視点を設定
	Vector3 eye(0.0f, 1.0f, 1.0f);
	// 注視点を原点に設定
	Vector3 target(0.0f, 0.0f, 0.0f);
	// 上方向をY軸正方向に設定
	Vector3 up(0.0f, 1.0f, 0.0f);
	// デフォルトのカメラ距離を適用
	eye *= DEFAULT_CAMERA_DISTANCE;
	// 視点を設定
	m_eye = eye;
	// 注視点を設定
	m_target = target;
	// 上方向を設定
	m_up = up;
	// ビュー行列を計算する
	m_view = Matrix::CreateLookAt(eye, target, up);
}
/*
*	@brief 相対スケールを計算する
*	@details ウィンドウの幅と高さに基づいて相対スケールを計算する
*	@param windowWidth ウィンドウの幅
*	@param windowHeight ウィンドウの高さ
*	@return なし
*/
void FixedCamera::CalculateRerativeScale(int windowWidth, int windowHeight)
{
	// 画面サイズに対する相対的なスケールに調整
	m_sx = 1.0f / static_cast<float>(windowWidth);
	m_sy = 1.0f / static_cast<float>(windowHeight);
}
