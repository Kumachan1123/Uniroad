/*
*   @file ItemFactory.cpp
*   @brief アイテムを生成するためのファクトリークラス
*/
#include "pch.h"
#include "ItemFactory.h"
/*
*	@brief アイテム名に基づいてアイテムを生成する
*	@details アイテム名に応じて適切なアイテムを生成し、ポインタを返す。
*	@param name アイテムの名前
*	@return 生成されたアイテムのポインタ
*/
std::unique_ptr<ItemBase> ItemFactory::CreateItemByName(const std::string& name)
{
	// アイテム名に応じてアイテムを生成
	// メダル
	if (name == "Medal")
		return std::make_unique<Medal>();

	return nullptr;
}
