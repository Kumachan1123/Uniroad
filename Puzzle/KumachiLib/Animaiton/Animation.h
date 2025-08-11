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
	// アクセサ
	// シーケンスを進める
	void AdvanceSequence();
	// 全アニメーションが終了したことを確認
	bool IsAnimationFinished() const { return m_currentStep >= m_animSequence.size() - 1; }
	// 今のアニメーションフェーズを取得
	size_t GetAnimationPhase() const { return m_currentStep; }
	// 登録したアニメーションシーケンスの数を取得
	size_t GetAnimationSequenceCount() const { return m_animSequence.size() - 1; }
	// 一時停止と再開
	void Pause() { m_isPaused = true; }
	void Resume() { m_isPaused = false; }
	// 一時停止中かどうかを取得
	bool IsPaused() const { return m_isPaused; }
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
	// 一時停止
	bool m_isPaused;
	// 現在のアニメーションステップ
	size_t m_currentStep;
	// アニメーションステップ時間
	float m_animStepTime;
	// アニメーションシーケンス
	std::vector<AnimationStep> m_animSequence;
};