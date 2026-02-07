/*
	@file	DebugCamera.h
	@brief	デバッグ用カメラクラス
*/
#pragma once

namespace mylib
{
	class InputManager;
}

namespace mylib
{
	// デバッグ用カメラクラス
	class DebugCamera
	{
	private:
		// デフォルトでカメラが注視点からどのくらい離れているか
		static const float DEFAULT_CAMERA_DISTANCE;

		// 横回転（Ｙ軸回転）
		float m_yAngle, m_yTmp;

		// 縦回転（Ｘ軸回転）
		float m_xAngle, m_xTmp;

		// マウスでドラッグされた座標
		int m_x, m_y;

		// 相対スケール
		float m_sx, m_sy;

		// 生成されたビュー行列
		DirectX::SimpleMath::Matrix m_view;

		// スクロールホイール値
		int m_scrollWheelValue;

		// 視点
		DirectX::SimpleMath::Vector3 m_eye;

		// 注視点
		DirectX::SimpleMath::Vector3 m_target;

		// カメラの頭の方向
		DirectX::SimpleMath::Vector3 m_up;

	public:
		DebugCamera();
		~DebugCamera() = default;

		void Initialize(int screenWidth, int screenHeight);
		void Update(mylib::InputManager* inputManager);

		// ビュー行列（カメラ行列）を取得する
		const DirectX::SimpleMath::Matrix& GetViewMatrix() const
		{
			return m_view;
		}

		// 視点を取得する
		const DirectX::SimpleMath::Vector3& GetEyePosition() const
		{
			return m_eye;
		}

		// 注視点を取得する
		const DirectX::SimpleMath::Vector3& GetTargetPosition() const
		{
			return m_target;
		}

		// カメラの頭の方向を取得する
		const DirectX::SimpleMath::Vector3& GetUpPosition() const
		{
			return m_up;
		}


	private:
		// 変位を計測する
		void Motion(int x, int y);

		// ビュー行列を計算する
		void CalculateViewMatrix();

		// 相対スケールを計算する
		void CalculateRerativeScale(int windowWidth, int windowHeight);
	};
}

/*
	■変更履歴
	・2024/03/30 getterを「const参照」に変更した
	・2024/03/31 getter関数に「const」を付けた

*/