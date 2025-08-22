/*
*   @file ItemFactory.cpp
*   @brief �A�C�e���𐶐����邽�߂̃t�@�N�g���[�N���X
*/
#include "pch.h"
#include "ItemFactory.h"
/*
*	@brief �A�C�e�����Ɋ�Â��ăA�C�e���𐶐�����
*	@details �A�C�e�����ɉ����ēK�؂ȃA�C�e���𐶐����A�|�C���^��Ԃ��B
*	@param name �A�C�e���̖��O
*	@return �������ꂽ�A�C�e���̃|�C���^
*/
std::unique_ptr<ItemBase> ItemFactory::CreateItemByName(const std::string& name)
{
	// �A�C�e�����ɉ����ăA�C�e���𐶐�
	// ���_��
	if (name == "Medal")
		return std::make_unique<Medal>();
	// �S�[�����b�N
	if (name == "GoalLock")
		return std::make_unique<GoalLock>();

	return nullptr;
}
