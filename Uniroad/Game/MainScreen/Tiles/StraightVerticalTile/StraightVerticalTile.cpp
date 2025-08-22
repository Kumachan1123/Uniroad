/*
*	@file StraightVerticalTile.cpp
*	@brief ���i�i�c�j�^�C���̏������`����N���X
*/
#include <pch.h>
#include "StraightVerticalTile.h"
/*
*	@brief �^�C���ɓ������Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C���ɓ������Ƃ��̏������`����B
*	@param character �^�C���ɓ������L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void StraightVerticalTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this)) return;

	// �c�����̂ݐN���\
	Vector3 v = character->GetVelocity();
	if (v == Vector3::Backward || v == Vector3::Forward)
	{
		// �N���\�Ȃ�ʉߋL�^�̂�
		character->SetEnteredTile(this);
		// ���x�x�N�g���͂��̂܂�
		// �L�����N�^�[�𓮂����t���O�𗧂Ă�
		character->SetMoving(true);
	}
	else
	{
		// �N���s�Ȃ��~
		character->SetVelocity(Vector3::Zero);
		// �ʉߋL�^�͂��Ȃ�
	}
}
/*
*	@brief �^�C������o���Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C������o���Ƃ��̏������`����B
*	@param character �^�C������o���L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void StraightVerticalTile::OnExit(MiniCharacter* character)
{
	// �t���O����������
	character->ResetEnteredTiles();
}
/*
*	@brief �^�C���̒��S�ɗ����Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C���̒��S�ɗ����Ƃ��̏������`����B
*	@param character �^�C���̒��S�ɗ����L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void StraightVerticalTile::OnCenterReached(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
	return;
}
