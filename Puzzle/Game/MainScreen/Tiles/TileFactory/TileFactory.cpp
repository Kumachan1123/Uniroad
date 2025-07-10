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
	if (name == "DefaultStraightVertical" || name == "StraightVertical")
		return std::make_unique<StraightVerticalTile>();
	// 直進（横）タイル
	if (name == "DefaultStraightHorizontal" || name == "StraightHorizontal")
		return std::make_unique<StraightHorizontalTile>();
	// 左上タイル
	if (name == "LeftUp")
		return std::make_unique<LeftUpTile>();
	// 左下タイル
	if (name == "LeftDown")
		return std::make_unique<LeftDownTile>();
	// 右上タイル
	if (name == "RightUp")
		return std::make_unique<RightUpTile>();
	// 右下タイル
	if (name == "RightDown")
		return std::make_unique<RightDownTile>();
	// スタートタイル
	if (name == "Start")
		return std::make_unique<StartTile>();
	// ゴールタイル
	if (name == "Goal")
		return std::make_unique<GoalTile>();

	return nullptr;
}
