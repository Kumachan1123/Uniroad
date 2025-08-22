/*
 *	@file StraightHorizontalTile.cpp
 *	@brief ���i�i���j�^�C���̏������`����N���X
 */
#include <pch.h>
#include "StraightHorizontalTile.h"
 /*
 *	@brief �^�C���ɓ������Ƃ��̏���
 *	@details �L�����N�^�[�����̃^�C���ɓ������Ƃ��̏������`����B
 *	@param character �^�C���ɓ������L�����N�^�[�ւ̃|�C���^
 *	@return �Ȃ�
 */
void StraightHorizontalTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this)) return;
	// �������ix���}�����j�̂ݐN���\
	Vector3 v = character->GetVelocity();
	if (v == Vector3::Left || v == Vector3::Right)
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
void StraightHorizontalTile::OnExit(MiniCharacter* character)
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
void StraightHorizontalTile::OnCenterReached(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
	return;
}
