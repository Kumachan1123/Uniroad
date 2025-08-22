/*
*	@file IButton.h
*	@brief �{�^���̃C���^�[�t�F�[�X�N���X
*/
#pragma once
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/Interface/IImage.h"

// �O���錾
class CommonResources;

// �{�^���̃C���^�[�t�F�[�X�N���X
class IButton : public IImage
{
public:
	// �A�N�Z�T
	// �X�s�[�h�A�b�v�{�^���������ꂽ���ǂ������擾
	virtual bool IsPressed() const = 0;
	// �X�s�[�h�A�b�v�{�^���������ꂽ���ǂ�����ݒ�
	virtual void SetPressed(bool isPressed) = 0;
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IButton() = default;

};