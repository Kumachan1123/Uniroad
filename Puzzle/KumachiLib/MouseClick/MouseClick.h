/*
*	@file MyMouse.h
*	@brief InputManagerのみではできないマウスの入力を拡張するクラス
*/
#pragma once
// DirectX
#include <Mouse.h>

// 拡張マウス入力クラス
class MouseClick
{
public:
	// 関数
	// 左クリックが押された瞬間を取得
	static bool IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState);
	// 右クリックが押された瞬間を取得
	static bool IsRightMouseButtonPressed(const DirectX::Mouse::State& currentState);
private:
	// 変数
	// 前回のマウスボタンの状態(左）
	static bool m_prevLeftMouseButtonState;
	// 前回のマウスボタンの状態(右）
	static bool m_prevRightMouseButtonState;
};
