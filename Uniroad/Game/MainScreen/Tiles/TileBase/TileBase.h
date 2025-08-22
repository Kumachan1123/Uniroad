/*
*	@file TileBase.h
*	@brief �^�C���̃x�[�X�N���X
*/
#pragma once

// �O���錾
class MiniCharacter;

// �^�C���x�[�X�N���X
class TileBase
{
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~TileBase() = default;
	// �^�C���ɓ������Ƃ��̏���
	virtual void OnEnter(MiniCharacter* miniCharacter) = 0;
	// �^�C������o���Ƃ��̏���
	virtual void OnExit(MiniCharacter* miniCharacter) = 0;
	// �^�C���̒��S�ɗ������̏���
	virtual void OnCenterReached(MiniCharacter* miniCharacter) = 0;

};