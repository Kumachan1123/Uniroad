/*
*	@file TileFactory.h
*	@brief �^�C���𐶐����邽�߂̃t�@�N�g���[�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/Tiles/BlockTile/BlockTile.h"
#include "Game/MainScreen/Tiles/StraightVerticalTile/StraightVerticalTile.h"
#include "Game/MainScreen/Tiles/StraightHorizontalTile/StraightHorizontalTile.h"
#include "Game/MainScreen/Tiles/LeftUpTile/LeftUpTile.h"
#include "Game/MainScreen/Tiles/LeftDownTile/LeftDownTile.h"
#include "Game/MainScreen/Tiles/RightUpTile/RightUpTile.h"
#include "Game/MainScreen/Tiles/RightDownTile/RightDownTile.h"
#include "Game/MainScreen/Tiles/StartTile/StartTile.h"
#include "Game/MainScreen/Tiles/GoalTile/GoalTile.h"
#include "Game/MainScreen/Tiles/CrossTile/CrossTile.h"

// �^�C���t�@�N�g���[�N���X
class TileFactory
{
public:
	// public�֐�
	// �^�C�����Ɋ�Â��ă^�C���𐶐�����
	static std::unique_ptr<TileBase> CreateTileByName(const std::string& name);
};

