/*
*	@file Animation.cpp
*	@brief アニメーションクラスの実装ファイル
*/
#include "pch.h"
#include "Animation.h"
/*
*	@brief コンストラクタ
*	@details アニメーションクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Animation::Animation()
	: m_animationPhase(0) // アニメーションフェーズ
	, m_animationTime(0.0f) // アニメーション時間
	, m_currentStep(0) // 現在のアニメーションステップ
	, m_animStepTime(0.0f) // アニメーションステップ時間
{
}
/*
*	@brief アニメーションシーケンスを作成
*	@details アニメーションシーケンスを作成する
*	@param sequence アニメーションステップのシーケンス
*	@return なし
*/
void Animation::CreateAnimationSequence(const AnimationStep& sequence)
{
	// 新しいシーケンスを追加
	m_animSequence.push_back(sequence);
}
/*
*	@brief アニメーションを更新
*	@details アニメーションの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Animation::Update(float elapsedTime)
{
	// アニメーションシーケンスが未作成ならば終了
	if (m_currentStep >= m_animSequence.size()) return;
	// アニメーション時間を更新
	m_animStepTime += elapsedTime;
	// アニメーションステップの更新
	float duration = m_animSequence[m_currentStep].duration;
	// 進行度を計算
	float t = duration > 0.0f ? std::min(m_animStepTime / duration, 1.0f) : 0.0f;
	// アニメーションを実行
	m_animSequence[m_currentStep].updateFunc(t);
	// アニメーションが終了したら
	if (duration > 0.0f && m_animStepTime >= duration)
	{
		// 次のステップへ進む
		m_currentStep++;
		// アニメーションステップ時間をリセット
		m_animStepTime = 0.0f;
	}
}
