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
		// 直進（縦）タイルを生成して返す
		return std::make_unique<StraightVerticalTile>();
	// 直進（横）タイル
	if (name == "DefaultStraightHorizontalBlock" || name == "StraightHorizontalBlock")
		// 直進（横）タイルを生成して返す
		return std::make_unique<StraightHorizontalTile>();
	// 左上タイル
	if (name == "LeftUpBlock")
		// 左上タイルを生成して返す
		return std::make_unique<LeftUpTile>();
	// 左下タイル
	if (name == "LeftDownBlock")
		// 左下タイルを生成して返す
		return std::make_unique<LeftDownTile>();
	// 右上タイル
	if (name == "RightUpBlock")
		// 右上タイルを生成して返す
		return std::make_unique<RightUpTile>();
	// 右下タイル
	if (name == "RightDownBlock")
		// 右下タイルを生成して返す
		return std::make_unique<RightDownTile>();
	// 十字タイル
	if (name == "CrossBlock")
		// 十字タイルを生成して返す
		return std::make_unique<CrossTile>();
	// スタートタイル
	if (name == "StartBlock")
		// スタートタイルを生成して返す
		return std::make_unique<StartTile>();
	// ゴールタイル
	if (name == "GoalBlock")
		// ゴールタイルを生成して返す
		return std::make_unique<GoalTile>();
	// 侵入不可タイル
	if (name == "Block")
		// 侵入不可タイルを生成して返す
		return std::make_unique<BlockTile>();
	// 対応するタイルがない場合はnullptrを返す
	return nullptr;
}
