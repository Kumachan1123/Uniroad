/*
*	@file MyMouse.cpp
*	@brief InputManagerのみではできないマウスの入力を拡張するクラス
*/
#include <pch.h>
#include "MouseClick.h"

// 前回のマウスボタンの状態(左）を初期化
bool MouseClick::m_prevLeftMouseButtonState = false;
// 前回のマウスボタンの状態(右）を初期化
bool MouseClick::m_prevRightMouseButtonState = false;

/*
*	@brief 左クリックが押された瞬間を取得
*	@details マウスの状態を受け取り、左クリックが押された瞬間を判定する
*	@param currentState マウスの現在の状態
*	@return 左クリックが押された瞬間ならtrue、そうでなければfalse
*/
bool MouseClick::IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState)
{
	// 前回のマウスボタンの状態と現在の状態を比較して、左クリックが押された瞬間を判定する
	bool isPressed = false;
	// 左クリックが押されていて、前回の状態では押されていなかった場合
	if (currentState.leftButton && !m_prevLeftMouseButtonState)
	{
		// 左クリックが押された瞬間と判定
		isPressed = true;
	}
	// 前回のマウスボタンの状態を更新
	m_prevLeftMouseButtonState = currentState.leftButton;
	// 判定結果を返す
	return isPressed;
}
/*
*	@brief 右クリックが押された瞬間を取得
*	@details マウスの状態を受け取り、右クリックが押された瞬間を判定する
*	@param currentState マウスの現在の状態
*	@return 右クリックが押された瞬間ならtrue、そうでなければfalse
*/
bool MouseClick::IsRightMouseButtonPressed(const DirectX::Mouse::State& currentState)
{
	// 前回のマウスボタンの状態と現在の状態を比較して、右クリックが押された瞬間を判定する
	bool isPressed = false;
	// 右クリックが押されていて、前回の状態では押されていなかった場合
	if (currentState.rightButton && !m_prevRightMouseButtonState)
	{
		// 右クリックが押された瞬間と判定
		isPressed = true;
	}
	// 前回のマウスボタンの状態を更新
	m_prevRightMouseButtonState = currentState.rightButton;
	// 判定結果を返す
	return isPressed;
}
