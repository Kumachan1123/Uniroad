/*
*	@file Animation.h
*	@brief アニメーションクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// 自作ヘッダーファイル
#include "KumachiLib/AnimationStep/AnimationStep.h"

// アニメーションクラス
class Animation
{
public:
	// public関数
	// コンストラクタ
	Animation();
	// デストラクタ
	~Animation() = default;
	// アニメーションシーケンスを作成
	void CreateAnimationSequence(const AnimationStep& sequence);
	// アニメーションを更新
	void Update(float elapsedTime);
private:
	// private変数
	// アニメーション進行度
	float m_animationTime;
	// アニメーションのフェーズ
	int m_animationPhase;
	// 現在のアニメーションステップ
	size_t m_currentStep;
	// アニメーションステップ時間
	float m_animStepTime;
	// アニメーションシーケンス
	std::vector<AnimationStep> m_animSequence;
};