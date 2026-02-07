/*
*	@file Parts.cpp
*	@brief ミニキャラクターの部品の設置位置を定義するソースファイル
*/
#include <pch.h>
#include "Parameters.h"

// 静的定数の初期化
// 速度の極小値
const float MiniCharacterParameters::SPEED_MIN = 0.0001f;
// タイヤの回転速度
const float MiniCharacterParameters::WHEEL_ROTATE_SPEED = 0.5f;


// 頭の位置
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::HEAD_POSITION = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
// 胴体の位置
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::BODY_POSITION = DirectX::SimpleMath::Vector3(0.0f, 3.5f, 0.0f);
// 一輪車の車体の位置
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::UNICYCLE_BODY_POSITION = DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);
// 一輪車のタイヤの位置
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::UNICYCLE_WHEEL_POSITION = DirectX::SimpleMath::Vector3(0.0f, -0.85f, 0.0f);


// パーティクル
// 速度の範囲
// 最小
const float MiniCharacterParameters::VELOCITY_MIN = 0.5f;
// 最大
const float MiniCharacterParameters::VELOCITY_MAX = 2.0f;
// 土煙パーティクル数
const int MiniCharacterParameters::PARTICLE_COUNT = 50;
// 汗パーティクルの数
const int MiniCharacterParameters::SWEAT_PARTICLE_COUNT = 5;

// 寿命
const float MiniCharacterParameters::LIFE = 0.75f;
// 土煙の加速度（タイトル画面）
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::ACCELERATION = DirectX::SimpleMath::Vector3(-150.0f, 0.0f, 0.0f);
// 発生座標のオフセット
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::POSITION_OFFSET = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
// 初期カラー
const DirectX::SimpleMath::Vector4 MiniCharacterParameters::INITIAL_COLOR = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.75f, 0.5f);
// 終了カラー
const DirectX::SimpleMath::Vector4 MiniCharacterParameters::FINAL_COLOR = DirectX::SimpleMath::Vector4(0.75f, 0.75f, 0.5f, 0.0f);

// 汗の寿命
const float MiniCharacterParameters::SWEAT_LIFE = 1.5f;
// 発生座標のオフセット
const DirectX::SimpleMath::Vector3 MiniCharacterParameters::SWEAT_POSITION_OFFSET = DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f);
