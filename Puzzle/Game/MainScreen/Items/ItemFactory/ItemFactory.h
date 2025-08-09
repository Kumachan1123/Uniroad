/*
*	@file ItemFactory.h
*	@brief �A�C�e���𐶐����邽�߂̃t�@�N�g���[�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/Items/Medal/Medal.h"
#include "Game/MainScreen/Items/GoalLock/GoalLock.h"

class ItemFactory
{
public:
	// public�֐�
	// �A�C�e�����Ɋ�Â��ăA�C�e���𐶐�����
	static std::unique_ptr<ItemBase> CreateItemByName(const std::string& name);

};