#pragma once
#include <SimpleMath.h>
#include <cmath>
#include <algorithm>
#include <random>
#include <type_traits> 

// 線形補間
template <typename T>
inline T Lerp(const T& start, const T& end, float t)
{
	return start + t * (end - start);
}
// 範囲制限
template <typename T>
inline T Clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;
}
// 円形補間
template <typename T>
inline T Slerp(const T& start, const T& end, float t)
{
	float dot = start.Dot(end);
	dot = Clamp(dot, -1.0f, 1.0f);
	float theta = std::acos(dot) * t;
	T relative = end - start * dot;
	relative.Normalize();
	return ((start * std::cos(theta)) + (relative * std::sin(theta)));
}

// eyeからtargetへの角度を計算する
inline float CalculateAngle(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target)
{
	DirectX::SimpleMath::Vector3 direction = target - eye;
	direction.Normalize();
	float angle = std::atan2(direction.x, direction.z);
	return angle;
}


// 物体Aが物体Bに向かって動く(高さは無視)
inline DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& A,
	const DirectX::SimpleMath::Vector3& B,
	float maxSpeed)
{
	DirectX::SimpleMath::Vector3 desired = B - A;
	desired.Normalize();
	desired *= maxSpeed;
	desired.y = 0;
	return desired;
}

// 整数型用のランダムな倍率を生成する関数
inline int GenerateRandomMultiplier(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

// 浮動小数点型用のランダムな倍率を生成する関数
inline float GenerateRandomMultiplier(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

// オブジェクト同士が衝突したら押し戻す(境界球同士の場合）
inline DirectX::SimpleMath::Vector3 CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻す処理
	// Ａの中心とＢの中心との差分ベクトル（ＢからＡに向かうベクトル）…①
	Vector3 diffVector = A.Center - B.Center;
	// Ａの中心とＢの中心との距離（①の長さ）…②
	float distance = diffVector.Length();
	// Ａの半径とＢの半径の合計…③
	float sumRadius = A.Radius + B.Radius;
	// （ＡがＢに）めり込んだ距離（③－②）…④
	float penetrationDistance = sumRadius - distance;
	// ①を正規化する…⑤
	diffVector.Normalize();
	// 押し戻すベクトルを計算する（⑤と④で表現する）…⑥
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// ⑥を使用して、Ａの座標とＡのコライダー座標を更新する（実際に押し戻す）
	Vector3 newPosition = A.Center + pushBackVec;
	A.Center = newPosition;
	return newPosition;
}

// オブジェクト同士が衝突したら押し戻す(境界球と境界ボックスの場合）
inline DirectX::SimpleMath::Vector3 CheckHitWall(DirectX::BoundingSphere& A, DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	// 押し戻しベクトルを計算
	Vector3 pushBackVec = Vector3::Zero;
	// 球体の中心とボックスのクランプされた位置の差分を求める
	Vector3 closestPoint; // ボックスの最も近い点

	// 各軸でクランプして、最も近い位置を取得
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));

	// 球体の中心と最も近い点のベクトル差
	Vector3 diffVector = A.Center - closestPoint;

	// 距離を計算
	float distance = diffVector.Length();

	// 距離が球体の半径より小さい場合は押し戻し処理
	if (distance < A.Radius)
	{
		// 押し戻し量を計算 (正規化して押し戻しベクトルを作成)
		float penetrationDistance = A.Radius - distance;
		diffVector.Normalize();
		pushBackVec = diffVector * penetrationDistance;
		Vector3 newPosition = A.Center + pushBackVec;
		A.Center = newPosition;
		return newPosition;
	}
	return A.Center;
}