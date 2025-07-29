/*
*	@file MyMouse.cpp
*	@brief InputManager�݂̂ł͂ł��Ȃ��}�E�X�̓��͂��g������N���X
*/
#include <pch.h>
#include "MouseClick.h"

// �O��̃}�E�X�{�^���̏��(���j��������
bool MouseClick::m_prevLeftMouseButtonState = false;
// �O��̃}�E�X�{�^���̏��(�E�j��������
bool MouseClick::m_prevRightMouseButtonState = false;

/*
*	@brief ���N���b�N�������ꂽ�u�Ԃ��擾
*	@details �}�E�X�̏�Ԃ��󂯎��A���N���b�N�������ꂽ�u�Ԃ𔻒肷��
*	@param currentState �}�E�X�̌��݂̏��
*	@return ���N���b�N�������ꂽ�u�ԂȂ�true�A�����łȂ����false
*/
bool MouseClick::IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState)
{
	// �O��̃}�E�X�{�^���̏�Ԃƌ��݂̏�Ԃ��r���āA���N���b�N�������ꂽ�u�Ԃ𔻒肷��
	bool isPressed = false;
	// ���N���b�N��������Ă��āA�O��̏�Ԃł͉�����Ă��Ȃ������ꍇ
	if (currentState.leftButton && !m_prevLeftMouseButtonState)
	{
		// ���N���b�N�������ꂽ�u�ԂƔ���
		isPressed = true;
	}
	// �O��̃}�E�X�{�^���̏�Ԃ��X�V
	m_prevLeftMouseButtonState = currentState.leftButton;
	// ���茋�ʂ�Ԃ�
	return isPressed;
}
/*
*	@brief �E�N���b�N�������ꂽ�u�Ԃ��擾
*	@details �}�E�X�̏�Ԃ��󂯎��A�E�N���b�N�������ꂽ�u�Ԃ𔻒肷��
*	@param currentState �}�E�X�̌��݂̏��
*	@return �E�N���b�N�������ꂽ�u�ԂȂ�true�A�����łȂ����false
*/
bool MouseClick::IsRightMouseButtonPressed(const DirectX::Mouse::State& currentState)
{
	// �O��̃}�E�X�{�^���̏�Ԃƌ��݂̏�Ԃ��r���āA�E�N���b�N�������ꂽ�u�Ԃ𔻒肷��
	bool isPressed = false;
	// �E�N���b�N��������Ă��āA�O��̏�Ԃł͉�����Ă��Ȃ������ꍇ
	if (currentState.rightButton && !m_prevRightMouseButtonState)
	{
		// �E�N���b�N�������ꂽ�u�ԂƔ���
		isPressed = true;
	}
	// �O��̃}�E�X�{�^���̏�Ԃ��X�V
	m_prevRightMouseButtonState = currentState.rightButton;
	// ���茋�ʂ�Ԃ�
	return isPressed;
}
