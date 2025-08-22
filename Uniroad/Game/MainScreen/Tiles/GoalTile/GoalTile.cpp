/*
*	@file GoalTile.cpp
*	@brief �S�[���^�C���̏������`����N���X
*/
#include "pch.h"
#include "GoalTile.h"

/*
*	@brief �^�C���ɓ������Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C���ɓ������Ƃ��̏������`����B
*	@param character �^�C���ɓ������L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void GoalTile::OnEnter(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
	// 2��ڈȍ~�͉������Ȃ�
	if (character->HasEnteredTile(this)) return;

	// �S�[���ɓ��B�������Ƃ�ʒm

	// �ʉߋL�^
	character->SetEnteredTile(this);
}

/*
*	@brief �^�C������o���Ƃ��̏���
*	@details �L�����N�^�[�����̃^�C������o���Ƃ��̏������`����B
*	@param character �^�C������o���L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void GoalTile::OnExit(MiniCharacter* character)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
	// �t���O����������
	character->ResetEnteredTiles();
}

void GoalTile::OnCenterReached(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(character);
	// �L�����N�^�[�̑��x���[���ɂ���
	character->SetVelocity(Vector3::Zero);
	// �L�����N�^�[�𓮂����t���O���~�߂�
	character->SetMoving(false);
	// 5�b��ɃQ�[���N���A�̃t���O�𗧂Ă�
	if (character->GetGameClearSwitchTime() > 5.0f)
	{
		// �x�[�X�N���X�̃|�C���^�[���擾
		const auto& pMinicharacterBase = dynamic_cast<MiniCharacterBase*>(character->GetParent());
		// �x�[�X�N���X�̃|�C���^�[����ꂽ��Q�[���N���A�t���O�𗧂Ă�
		if (pMinicharacterBase)pMinicharacterBase->SetGameClear(true);
		// ���Ȃ�������G���[
		else assert(false && "MiniCharacterBase is not set correctly.");
	}
}
