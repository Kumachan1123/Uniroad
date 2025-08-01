/*
*	@file BlockTile.cpp
*	@brief �N���s�̃^�C���̏������`����N���X
*/
#include "pch.h"
#include "BlockTile.h"

/*
*	@brief �v���C���[�����̃^�C���ɓ������Ƃ��̏���
*	@details �N���s�̃^�C���Ȃ̂ŁA�������Ȃ��B
*	@param character �^�C���ɓ������L�����N�^�[
*	@return �Ȃ�
*/
void BlockTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this)) return;
	// �L�����N�^�[�𓮂����t���O��܂�
	character->SetMoving(false);
	// �L�����N�^�[�̑��x�x�N�g�����[���ɂ���
	character->SetVelocity(Vector3::Zero);
	// �ʉߋL�^
	character->SetEnteredTile(this);
}
/*
*	@brief �v���C���[�����̃^�C������o���Ƃ��̏���
*	@details �N���s�̃^�C���Ȃ̂ŁA�������Ȃ��B
*	@param character �^�C������o���L�����N�^�[
*	@return �Ȃ�
*/
void BlockTile::OnExit(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
}
/*
*	@brief �v���C���[�����̃^�C���̒��S�ɗ����Ƃ��̏���
*	@details �N���s�̃^�C���Ȃ̂ŁA�������Ȃ��B
*	@param character �^�C���̒��S�ɗ����L�����N�^�[
*	@return �Ȃ�
*/
void BlockTile::OnCenterReached(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
}
