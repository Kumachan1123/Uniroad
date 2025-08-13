/*
*	@file TileFactory.cpp
*	@brief �^�C���𐶐����邽�߂̃t�@�N�g���[�N���X
*/
#include "pch.h"
#include "TileFactory.h"
/*
*	@brief �^�C�����Ɋ�Â��ă^�C���𐶐�����
*	@details �^�C�����ɉ����āA�Ή�����^�C���I�u�W�F�N�g�𐶐�����B
*	@param name �^�C���̖��O
*	@return �������ꂽ�^�C���I�u�W�F�N�g�̃��j�[�N�|�C���^
*/
std::unique_ptr<TileBase> TileFactory::CreateTileByName(const std::string& name)
{
	// �^�C�����ɉ����ă^�C���𐶐�
	// ���i�i�c�j�^�C��
	if (name == "DefaultStraightVerticalBlock" || name == "StraightVerticalBlock")
		return std::make_unique<StraightVerticalTile>();
	// ���i�i���j�^�C��
	if (name == "DefaultStraightHorizontalBlock" || name == "StraightHorizontalBlock")
		return std::make_unique<StraightHorizontalTile>();
	// ����^�C��
	if (name == "LeftUpBlock")
		return std::make_unique<LeftUpTile>();
	// �����^�C��
	if (name == "LeftDownBlock")
		return std::make_unique<LeftDownTile>();
	// �E��^�C��
	if (name == "RightUpBlock")
		return std::make_unique<RightUpTile>();
	// �E���^�C��
	if (name == "RightDownBlock")
		return std::make_unique<RightDownTile>();
	// �\���^�C��
	if (name == "CrossBlock")
		return std::make_unique<CrossTile>();
	// �X�^�[�g�^�C��
	if (name == "StartBlock")
		return std::make_unique<StartTile>();
	// �S�[���^�C��
	if (name == "GoalBlock")
		return std::make_unique<GoalTile>();
	// �N���s�^�C��
	if (name == "Block")
		return std::make_unique<BlockTile>();

	return nullptr;
}
