/*
*	@file MyMouse.h
*	@brief InputManager�݂̂ł͂ł��Ȃ��}�E�X�̓��͂��g������N���X
*/
#pragma once
// DirectX
#include <Mouse.h>

// �g���}�E�X���̓N���X
class MouseClick
{
public:
	// �֐�
	// ���N���b�N�������ꂽ�u�Ԃ��擾
	static bool IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState);
	// �E�N���b�N�������ꂽ�u�Ԃ��擾
	static bool IsRightMouseButtonPressed(const DirectX::Mouse::State& currentState);
private:
	// �ϐ�
	// �O��̃}�E�X�{�^���̏��(���j
	static bool m_prevLeftMouseButtonState;
	// �O��̃}�E�X�{�^���̏��(�E�j
	static bool m_prevRightMouseButtonState;
};
