/*
*	@file CrossTile.cpp
*	@brief �\���^�C���̏������`����N���X
*/
#include "pch.h"
#include "CrossTile.h"

void CrossTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this)) return;
	// �L�����N�^�[�𓮂����t���O�𗧂Ă�
	character->SetMoving(true);
	// �ʉߋL�^
	character->SetEnteredTile(this);
}

void CrossTile::OnExit(MiniCharacter* character)
{
	// �t���O����������
	character->ResetEnteredTiles();
}

void CrossTile::OnCenterReached(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
	return;
}
