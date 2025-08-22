/*
*	@file Utility.cpp
*	@brief パーティクルユーティリティクラスの実装
*/
#include "pch.h"
#include "Utility.h"
/*
*	@brief コンストラクタ
*	@details パーティクルユーティリティクラスの初期化を行う
*	@param params パーティクルのパラメーター
*	@return なし
*/
Utility::Utility(const ParticleParams& params)
	: m_position(params.pos) // 座標
	, m_velocity(params.velocity) // 速度
	, m_accele(params.accele) // 加速度
	, m_rotateAccele(params.rotateAccele) // 回転加速度
	, m_rotate(params.rotate) // 回転
	, m_startScale(params.startScale) // 初期サイズ
	, m_endScale(params.endScale) // 最終的なサイズ
	, m_startColor(params.startColor) // 初期カラー
	, m_endColor(params.endColor) // 最終的なカラー
	, m_type(params.type) // タイプ
	, m_life(params.life) // 生存時間
	, m_startLife(params.life) // 初期生存時間
	, m_elapsedTime(0.0f) // 経過時間
{
}
/*
*	@brief デストラクタ
*	@details パーティクルユーティリティクラスの破棄処理を行う
*	@param なし
*	@return なし
*/
Utility::~Utility() {/* do nothing. */ }
/*
*	@brief 更新
*	@details パーティクルの更新処理を行う
*	@param elapsedTime フレーム時間
*	@return 更新後のパーティクルが生存しているかどうか false: 生存していない, true: 生存している
*/
bool Utility::Update(float elapsedTime)
{
	// DirectX::SimpleMath::Vector3とDirectX::SimpleMath::Colorを使用するための名前空間
	using namespace DirectX;
	// スケールの補間
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// 色の補間
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);
	// 加速度の適用
	m_velocity += m_accele * elapsedTime;
	// 座標更新
	m_position += m_velocity * elapsedTime;
	// ライフの減少
	m_life -= elapsedTime;
	// ライフが0以下になったらfalseを返し、ライフが0より大きい場合はtrueを返す
	if (m_life < 0.0f)return false;
	else return true;
}
