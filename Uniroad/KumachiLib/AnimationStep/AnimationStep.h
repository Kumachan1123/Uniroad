/*
*	@file AnimationStep.h
*	@brief イージング関数によるアニメーションステップのヘッダーファイル
*/
#pragma once
// 標準ライブラリ
#include <functional>
struct AnimationStep
{
	// 演出の長さ
	float duration;
	// 進行度
	std::function<void(float t)> updateFunc;
};