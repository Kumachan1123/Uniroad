/*
	@file	InputManager.h
	@brief	マウスとキーボードの入力を請け負うクラス
*/
#pragma once

namespace mylib
{
	class InputManager
	{
	private:
		// マウス
		std::unique_ptr<DirectX::Mouse> m_mouse;
		std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouseTracker;
		DirectX::Mouse::State m_mouseState;

		// キーボード
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
		DirectX::Keyboard::State m_keyboardState;


	public:
		InputManager(const HWND& window);
		~InputManager() = default;

		void Update();

		// マウスステートを取得する
		const DirectX::Mouse::State& GetMouseState() const
		{
			return m_mouseState;
		}
		// マウスのボタンステートトラッカーを取得する
		const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& GetMouseTracker() const
		{
			return m_mouseTracker;
		}
		// キーボードステートを取得する
		const DirectX::Keyboard::State& GetKeyboardState() const
		{
			return m_keyboardState;
		}
		// キーボードのステートトラッカーを取得する
		const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& GetKeyboardTracker() const
		{
			return m_keyboardTracker;
		}
	};
}
