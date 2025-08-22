/*
*	@file LeftDownTile.cpp
*	@brief �����^�C���̏������`����N���X
*/
#include "pch.h"
#include "LeftDownTile.h"
/*
*	@brief �^�C���ɓ������Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C���ɓ������Ƃ��̏������`����B
*	@param character �^�C���ɓ������L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void LeftDownTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this))
		return;
	// ����������^�C���ɓ������牺�ɕ����]��
	if (character->GetVelocity().x < 0.0f || character->GetVelocity().z > 0.0f)
		character->SetVelocity(Vector3::Zero);
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
void LeftDownTile::OnExit(MiniCharacter* character)
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
void LeftDownTile::OnCenterReached(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// ����������^�C���ɓ������牺�ɕ����]��
	if (character->GetVelocity().x > 0.0f)
		character->SetVelocity(Vector3(0.0f, 0.0f, 1.0f));
	// ����������^�C���ɓ������獶�ɕ����]��
	else if (character->GetVelocity().z < 0.0f)
		character->SetVelocity(Vector3(-1.0f, 0.0f, 0.0f));
}
