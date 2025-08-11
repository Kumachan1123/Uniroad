/*
*	@file IImage.h
*	@brief �摜�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/Interface/IUI.h"

// �O���錾
class CommonResources;

// �摜�̃C���^�[�t�F�[�X�N���X
class IImage : public IUI
{
public:
	// �A�N�Z�T
	// ���W���擾
	virtual const DirectX::SimpleMath::Vector2& GetPosition() const = 0;
	// ���W��ݒ�
	virtual void SetPosition(const DirectX::SimpleMath::Vector2& position) = 0;
	// �T�C�Y���擾
	virtual const DirectX::SimpleMath::Vector2& GetSize() const = 0;
	// �T�C�Y��ݒ�
	virtual void SetSize(const DirectX::SimpleMath::Vector2& size) = 0;
private:
	// private�֐�
	virtual void UpdateConstantBuffer() = 0;
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IImage() = default;
};