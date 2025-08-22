/*
*	@file StartTile.cpp
*	@brief �X�^�[�g�^�C���̏������`����N���X
*/
#include "pch.h"
#include "StartTile.h"
/*
*	@brief �^�C���ɓ������Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C���ɓ������Ƃ��̏������`����B
*	@param character �^�C���ɓ������L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void StartTile::OnEnter(MiniCharacter* character)
{
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this)) return;
	// �c�����ɐi��
	character->SetVelocity(character->GetVelocity());
	// �L�����N�^�[�𓮂����t���O�𗧂Ă�
	character->SetMoving(true);
	// �ʉߋL�^
	character->SetEnteredTile(this);
}
/*
*	@brief �^�C������o���Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C������o���Ƃ��̏������`����B
*	@param character �^�C������o���L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void StartTile::OnExit(MiniCharacter* character)
{

	// �t���O����������
	character->ResetEnteredTiles();
}

void StartTile::OnCenterReached(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
}
