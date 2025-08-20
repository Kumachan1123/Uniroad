/*
*	@file Utility.h
*	@brief パーティクルユーティリティクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cmath>
#include <random>
// DirectXライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>



// パーティクルユーティリティクラス
class Utility
{
public:
	// 列挙型
	// パーティクルの種類
	enum class Type
	{
		STEAM,	// 水蒸気
		SHINE,	// 光
	};
public:
	// パーティクルのパラメーター構造体
	struct ParticleParams
	{
		// 生存時間
		float life;
		//	座標
		DirectX::SimpleMath::Vector3 pos;
		// 速度
		DirectX::SimpleMath::Vector3 velocity;
		// 加速度
		DirectX::SimpleMath::Vector3 accele;
		// 回転加速度
		DirectX::SimpleMath::Vector3 rotateAccele;
		// 回転
		DirectX::SimpleMath::Vector3 rotate;
		// 初期サイズ
		DirectX::SimpleMath::Vector3 startScale;
		// 最終的なサイズ
		DirectX::SimpleMath::Vector3 endScale;
		// 初期カラー
		DirectX::SimpleMath::Color startColor;
		// 最終的なカラー
		DirectX::SimpleMath::Color endColor;
		// タイプ
		Utility::Type type;
	};
public:
	// アクセサ
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 速度取得
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// 加速度取得
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	// 現在のスケール取得
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	// 開始スケール取得
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	// 終了スケール取得
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }
	// 生存時間取得
	float GetLife() const { return m_life; }
	// 開始生存時間取得
	float GetStartLife() const { return m_startLife; }
	// 現在の色取得
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }
	// 開始色取得
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; }
	// 終了色取得
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; }
public:
	// public関数
	// コンストラクタ　
	Utility(const ParticleParams& params);// タイプ
	// デストラクタ
	~Utility();
	// 更新
	bool Update(float elapsedTime);

private:
	// private変数
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 初期位置
	DirectX::SimpleMath::Vector3 m_startPosition;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 加速度
	DirectX::SimpleMath::Vector3 m_accele;
	// 回転加速度
	DirectX::SimpleMath::Vector3 m_rotateAccele;
	// 現在のスケール
	DirectX::SimpleMath::Vector3 m_nowScale;
	// 開始スケール
	DirectX::SimpleMath::Vector3 m_startScale;
	// 終了スケール
	DirectX::SimpleMath::Vector3 m_endScale;
	// 生存時間
	float m_life;
	// 開始生存時間
	float m_startLife;
	// 経過時間
	float m_elapsedTime;
	// 現在の色
	DirectX::SimpleMath::Color m_nowColor;
	// 開始色
	DirectX::SimpleMath::Color m_startColor;
	// 終了色
	DirectX::SimpleMath::Color m_endColor;
	// タイプ
	Type m_type;

};