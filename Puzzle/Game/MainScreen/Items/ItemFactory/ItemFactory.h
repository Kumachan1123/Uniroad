/*
*	@file ItemFactory.h
*	@brief アイテムを生成するためのファクトリークラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/Items/Medal/Medal.h"
#include "Game/MainScreen/Items/GoalLock/GoalLock.h"

class ItemFactory
{
public:
	// public関数
	// アイテム名に基づいてアイテムを生成する
	static std::unique_ptr<ItemBase> CreateItemByName(const std::string& name);

};