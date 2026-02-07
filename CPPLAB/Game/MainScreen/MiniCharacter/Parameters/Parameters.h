/*
*	@file Parts.h
*	@brief ミニキャラクターの部品の設置位置を定義するヘッダーファイル
*/
#pragma once

// 
class MiniCharacterParameters
{
public:
	// 静的public定数
	// 速度の極小値
	static const float SPEED_MIN;
	// タイヤの回転速度
	static const float WHEEL_ROTATE_SPEED;
	// 頭の位置
	static const DirectX::SimpleMath::Vector3 HEAD_POSITION;
	// 胴体の位置
	static const DirectX::SimpleMath::Vector3 BODY_POSITION;
	// 一輪車の車体の位置
	static const DirectX::SimpleMath::Vector3 UNICYCLE_BODY_POSITION;
	// 一輪車のタイヤの位置
	static const DirectX::SimpleMath::Vector3 UNICYCLE_WHEEL_POSITION;


	// パーティクル
	// 速度の範囲
	// 最小
	static const float VELOCITY_MIN;
	// 最大
	static const float VELOCITY_MAX;
	// 土煙パーティクル数
	static const int PARTICLE_COUNT;
	// 汗パーティクルの数
	static const int SWEAT_PARTICLE_COUNT;
	// 土煙
	// 寿命
	static const float LIFE;
	// 土煙の加速度（タイトル画面）
	static const DirectX::SimpleMath::Vector3 ACCELERATION;
	// 発生座標のオフセット
	static const DirectX::SimpleMath::Vector3 POSITION_OFFSET;
	// 初期カラー
	static const DirectX::SimpleMath::Vector4 INITIAL_COLOR;
	// 終了カラー
	static const DirectX::SimpleMath::Vector4 FINAL_COLOR;
	// 汗
	// 寿命
	static const float SWEAT_LIFE;
	// 発生座標のオフセット
	static const DirectX::SimpleMath::Vector3 SWEAT_POSITION_OFFSET;
};
