/*
*	@file    KumachiLib.h
*	@brief   自作のライブラリのヘッダーファイル
*	@details C++14で標準搭載されていない関数やその他便利な関数のテンプレートなどをまとめたもの
*/
#pragma once

// 標準ライブラリ
#include <cmath>
#include <algorithm>
#include <random>
#include <type_traits> 
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/Screen/Screen.h"
/*
*	@brief   2Dベクトルの線形補間
*	@details 2Dベクトルの線形補間を行う関数
*	@param   start 開始点
*	@param   end 終了点
*	@param   t 補間係数 (0.0f <= t <= 1.0f)
*	@return  補間されたベクトル
*/
template <typename T>
inline T Lerp(const T& start, const T& end, float t)
{
	// 線形補間
	return start + t * (end - start);
}
/*
*	@brief   2Dベクトルのクランプ
*	@details 2Dベクトルのクランプを行う関数
*	@param   value クランプする値
*	@param   min 最小値
*	@param   max 最大値
*	@return  クランプされた値
*/
template <typename T>
inline T Clamp(T value, T min, T max)
{
	// クランプ
	return (value < min) ? min : (value > max) ? max : value;
}
/*
*	@brief   2Dベクトルのスフェリカル補間
*	@details 2Dベクトルのスフェリカル補間を行う関数
*	@param   start 開始点
*	@param   end 終了点
*	@param   t 補間係数 (0.0f <= t <= 1.0f)
*	@return  補間されたベクトル
*/
template <typename T>
inline T Slerp(const T& start, const T& end, float t)
{
	// 内積
	float dot = start.Dot(end);
	// クランプ
	dot = Clamp(dot, -1.0f, 1.0f);
	// 角度
	float theta = std::acos(dot) * t;
	// 相対ベクトル
	T relative = end - start * dot;
	// 正規化
	relative.Normalize();
	// スフェリカル補間
	return ((start * std::cos(theta)) + (relative * std::sin(theta)));
}
/*
*	@brief   2Dベクトルの角度を計算
*	@details 2Dベクトルの角度を計算する関数
*	@param   eye 視点
*	@param   target ターゲット
*	@return  角度
*/
inline float CalculateAngle(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target)
{
	// ベクトルの差分
	DirectX::SimpleMath::Vector3 direction = target - eye;
	// ベクトルを正規化
	direction.Normalize();
	// アークタンジェント
	float angle = std::atan2(direction.x, direction.z);
	// 角度
	return angle;
}
/*
*	@brief   物体Aが物体Bに向かって動く(高さは無視)
*	@details 物体Aが物体Bに向かって動くためのベクトルを計算する関数
*	@param   A 物体Aの位置
*	@param   B 物体Bの位置
*	@param   maxSpeed 最大速度
*/
inline DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& A,
	const DirectX::SimpleMath::Vector3& B,
	float maxSpeed)
{
	// 方向ベクトルを計算
	DirectX::SimpleMath::Vector3 desired = B - A;
	// 方向ベクトルを正規化
	desired.Normalize();
	// 最大速度
	desired *= maxSpeed;
	// 高さを無視
	desired.y = 0;
	// 目標位置
	return desired;
}

/*
*	@brief   整数型用のランダムな倍率を生成する
*	@details 整数型用のランダムな倍率を生成する関数
*	@param   min 最小値
*	@param   max 最大値
*	@return  ランダムな倍率
*/
inline int GenerateRandomMultiplier(int min, int max)
{
	// 乱数生成器
	std::random_device rd;
	// メルセンヌ・ツイスタ法
	std::mt19937 gen(rd());
	// 一様分布
	std::uniform_int_distribution<int> dis(min, max);
	// ランダムな倍率を生成
	return dis(gen);
}

/*
*	@brief   浮動小数点型用のランダムな倍率を生成する
*	@details 浮動小数点型用のランダムな倍率を生成する関数
*	@param   min 最小値
*	@param   max 最大値
*	@return  ランダムな倍率
*/
inline float GenerateRandomMultiplier(float min, float max)
{
	// 乱数生成器
	std::random_device rd;
	// メルセンヌ・ツイスタ法
	std::mt19937 gen(rd());
	// 一様分布
	std::uniform_real_distribution<float> dis(min, max);
	// ランダムな倍率を生成
	return dis(gen);
}

/*
*	@brief   物体同士が衝突したら押し戻す(球体同士)
*	@details 球体同士の衝突判定を行い、押し戻す処理を行う関数
*	@param   A 球体A
*	@param   B 球体B
*	@return  押し戻された位置
*/
inline DirectX::SimpleMath::Vector3 CheckHitOtherObject(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻す処理
	// Aの中心とBの中心との差分ベクトル
	Vector3 diffVector = A.Center - B.Center;
	// 差分ベクトルの長さを計算
	float distance = diffVector.Length();
	// Aの半径とBの半径の合計を計算
	float sumRadius = A.Radius + B.Radius;
	// AがBにめり込んだ距離
	float penetrationDistance = sumRadius - distance;
	// 差分ベクトルを正規化
	diffVector.Normalize();
	// 押し戻すベクトルを計算する
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// 押し戻すベクトルを使用して、Aの座標とAのコライダー座標を更新（実際に押し戻す）
	Vector3 newPosition = A.Center + pushBackVec;
	// Aの座標を更新する
	A.Center = newPosition;
	// 更新されたAの座標を返す
	return newPosition;
}
/*
*	@brief   オブジェクト同士が衝突したら押し戻す(境界球と境界ボックスの場合）
*	@details 境界球と境界ボックスの衝突判定を行い、押し戻す処理を行う関数
*	@param   A 球体A
*	@param   B ボックスB
*	@return  押し戻された位置
*/
inline DirectX::SimpleMath::Vector3 CheckHitWall(DirectX::BoundingSphere A, const DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻しベクトルを計算
	Vector3 pushBackVec = Vector3::Zero;
	// ボックスの最も近い点
	Vector3 closestPoint;
	// 各軸でクランプして、最も近い位置を取得
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));// X軸
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));// Y軸
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));// Z軸
	// 球体の中心と最も近い点のベクトル差
	Vector3 diffVector = A.Center - closestPoint;
	// 距離を計算
	float distance = diffVector.Length();
	// 衝突していない場合は元の位置を返す
	if (distance > A.Radius)return A.Center;
	// 距離が球体の半径より小さい場合は押し戻し処理
	// 押し戻し量を計算 (正規化して押し戻しベクトルを作成)
	float penetrationDistance = A.Radius - distance;
	// 差分ベクトルを正規化
	diffVector.Normalize();
	// 押し戻しベクトルを計算
	pushBackVec = diffVector * penetrationDistance;
	// 押し戻しベクトルを使用して、Aの座標を更新
	Vector3 newPosition = A.Center + pushBackVec;
	// 球体の位置を更新
	A.Center = newPosition;
	// 更新された位置を返す
	return newPosition;
}

/*
*	@brief   UV座標をNDC座標に変換
*	@details UV座標を正規化デバイス座標(NDC)に変換する関数
*	@param   uv UV座標
*	@return  NDC座標
*/
inline DirectX::SimpleMath::Vector2 ToNDC(const DirectX::SimpleMath::Vector2& uv)
{
	return DirectX::SimpleMath::Vector2(uv.x * 2.0f - 1.0f, 1.0f - uv.y * 2.0f);
}

/*
*	@brief   画面座標を取得する
*	@details 画面モードが変わっても大丈夫なように動的に受け取るための関数
*	@param   hwnd ウィンドウハンドル
*	@param   width ウィンドウの幅(引数で受け取って設定してから返す）
*	@param   height ウィンドウの高さ(引数で受け取って設定してから返す）
*	@return  なし
*/
inline void GetScreenRect(const HWND hwnd, int& width, int& height)
{
	// RECT構造体を初期化
	RECT rect = { 0, 0, width, height };
	// ウィンドウのクライアント領域のサイズを取得
	if (GetClientRect(hwnd, &rect))
	{
		// 幅と高さを設定
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
}