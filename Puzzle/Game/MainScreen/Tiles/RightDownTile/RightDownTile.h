/*
*	@file RightDownTile.h
*	@brief �E���^�C���̏������`����N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// �E��^�C���N���X
class RightDownTile : public TileBase
{
public:
	RightDownTile() = default;
	// �v���C���[�����̃^�C���ɓ������Ƃ��̏���
	void OnEnter(MiniCharacter* character) override;
	// �v���C���[�����̃^�C������o���Ƃ��̏���
	void OnExit(MiniCharacter* character) override;
	// �v���C���[�����̃^�C���̒��S�ɗ����Ƃ��̏���
	void OnCenterReached(MiniCharacter* character) override;
};