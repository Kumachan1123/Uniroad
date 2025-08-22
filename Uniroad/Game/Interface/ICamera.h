/*
*	@file ICamera.h
*	@brief カメラのインターフェース
*/
#pragma once
// カメラのインターフェース
class ICamera
{
public:
	// アクセサ
	// ビュー行列を取得
	virtual const DirectX::SimpleMath::Matrix& GetViewMatrix() const = 0;
	// ビュー行列を設定
	virtual void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) = 0;
	// 射影行列を取得
	virtual const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const = 0;
	// 射影行列を設定
	virtual void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj) = 0;
	// 視点を取得
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const = 0;
	// 視点を設定
	virtual void SetEyePosition(const DirectX::SimpleMath::Vector3& eye) = 0;
	// 注視点を取得
	virtual const DirectX::SimpleMath::Vector3& GetTargetPosition() const = 0;
	// 注視点を設定
	virtual void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) = 0;
	// カメラの頭の方向を取得
	virtual const DirectX::SimpleMath::Vector3& GetUpPosition() const = 0;
public:
	// publicなメンバ関数
	// デストラクタ
	virtual ~ICamera() = default;
	// 初期化する
	virtual void Initialize(int screenWidth, int screenHeight) = 0;
	// 更新する
	virtual void Update() = 0;
private:
	// private関数
	// ビュー行列を計算する
	virtual void CalculateViewMatrix() = 0;
	// 相対スケールを計算する
	virtual void CalculateRerativeScale(int windowWidth, int windowHeight) = 0;
};