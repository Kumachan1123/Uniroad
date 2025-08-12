/*
*	@file TitleAnimationState.h
*	@brief タイトルシーンのアニメーションステート
*/
#pragma once
// タイトルシーンのひつじのアニメーションステートを定義する
enum TitleAnimation
{
	NONE = -1,	// アニメーションなし
	START = 0,	// アニメーション開始
	WAIT,		// アニメーション待機
	CONTINUE,	// アニメーション再開
	END			// アニメーション終了
};