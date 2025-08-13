/*
*	@file TileFactory.cpp
*	@brief タイルを生成するためのファクトリークラス
*/
#include "pch.h"
#include "TileFactory.h"
/*
*	@brief タイル名に基づいてタイルを生成する
*	@details タイル名に応じて、対応するタイルオブジェクトを生成する。
*	@param name タイルの名前
*	@return 生成されたタイルオブジェクトのユニークポインタ
*/
std::unique_ptr<TileBase> TileFactory::CreateTileByName(const std::string& name)
{
	// タイル名に応じてタイルを生成
	// 直進（縦）タイル
	if (name == "DefaultStraightVerticalBlock" || name == "StraightVerticalBlock")
		return std::make_unique<StraightVerticalTile>();
	// 直進（横）タイル
	if (name == "DefaultStraightHorizontalBlock" || name == "StraightHorizontalBlock")
		return std::make_unique<StraightHorizontalTile>();
	// 左上タイル
	if (name == "LeftUpBlock")
		return std::make_unique<LeftUpTile>();
	// 左下タイル
	if (name == "LeftDownBlock")
		return std::make_unique<LeftDownTile>();
	// 右上タイル
	if (name == "RightUpBlock")
		return std::make_unique<RightUpTile>();
	// 右下タイル
	if (name == "RightDownBlock")
		return std::make_unique<RightDownTile>();
	// 十字タイル
	if (name == "CrossBlock")
		return std::make_unique<CrossTile>();
	// スタートタイル
	if (name == "StartBlock")
		return std::make_unique<StartTile>();
	// ゴールタイル
	if (name == "GoalBlock")
		return std::make_unique<GoalTile>();
	// 侵入不可タイル
	if (name == "Block")
		return std::make_unique<BlockTile>();

	return nullptr;
}
