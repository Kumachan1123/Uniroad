/*
*	@file		IUI.h
*	@brief	�@UI�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/Anchor/Anchor.h"

// �O���錾
class CommonResources;

// UI�̃C���^�[�t�F�[�X�N���X
class IUI
{


public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IUI() = default;
	// ������
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	// �X�V
	virtual void Update(const float elapsedTime) = 0;
	// �`��
	virtual void Render() = 0;

};