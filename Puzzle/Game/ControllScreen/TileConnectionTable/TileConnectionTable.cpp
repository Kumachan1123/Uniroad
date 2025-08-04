#include "pch.h"
#include "TileConnectionTable.h"
// タイル種ごとの進行可能タイル
static const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>> s_tileConnectionTable =
{
	{"DefaultStraightVertical",
	{
		{Direction::UP,{  "StraightVertical","RightDown", "LeftDown"}},
		{Direction::DOWN,{ "StraightVertical","RightUp", "LeftUp"}}
	}},
	{"DefaultStraightHorizontal",
	{
		{Direction::LEFT,{ "StraightHorizontal","RightDown", "RightUp"}},
		{Direction::RIGHT,{  "StraightHorizontal","LeftDown", "LeftUp"}}
	}},
	{"StraightVertical",
	{
		{Direction::UP,{  "StraightVertical","RightDown", "LeftDown"}},
		{Direction::DOWN,{ "StraightVertical","RightUp", "LeftUp"}}
	}},
	{"StraightHorizontal",
	{
		{Direction::LEFT,{ "StraightHorizontal","RightDown", "RightUp"}},
		{Direction::RIGHT,{ "StraightHorizontal","LeftDown", "LeftUp"}}
	}},
	{"RightDown",{
	{Direction::DOWN,{ "StraightVertical","RightUp", "LeftUp"}},
	{Direction::LEFT,{  "StraightVertical","RightUp", "LeftUp"}},
	{Direction::RIGHT,{ "StraightHorizontal","LeftDown", "LeftUp"}},
	{Direction::UP,{ "StraightHorizontal","LeftDown", "LeftUp"}},
	}},
	{"LeftDown",{
	{Direction::DOWN,{ "StraightVertical","RightUp", "LeftUp"}},
	{Direction::RIGHT,{"StraightVertical","RightUp", "LeftUp"}},
	{Direction::UP,{ "StraightHorizontal","LeftDown", "LeftUp"}},
	{Direction::LEFT,{"StraightHorizontal","RightDown", "RightUp"}}}},
	{"RightUp",{
	{Direction::UP,{  "StraightVertical","RightDown", "LeftDown"}},
	{Direction::LEFT,{  "StraightVertical","RightDown", "LeftDown"}},
	{Direction::RIGHT,{ "StraightHorizontal","LeftDown", "LeftUp"}},
	{Direction::RIGHT,{ "StraightHorizontal","LeftDown", "LeftUp"}}}},
	{"LeftUp",{
		{Direction::UP,{ "StraightVertical","RightDown", "LeftDown"}},
		{Direction::DOWN,{   "StraightVertical","RightDown", "RightUp"}},
		{Direction::RIGHT,{ "StraightHorizontal","RightDown", "LeftDown"}},
		{Direction::LEFT,{   "StraightHorizontal","RightDown", "RightUp"}}}},
	{"Cross",{
		{Direction::UP,{  "StraightVertical","RightDown", "LeftDown"}},
		{Direction::DOWN,{ "StraightVertical","RightUp", "LeftUp"}},
		{Direction::LEFT,{ "StraightHorizontal","RightDown", "RightUp"}},
		{Direction::RIGHT,{ "StraightHorizontal","LeftDown", "LeftUp"}}}}
};

static const std::vector<std::string> s_dammyArray = { "StraightVertical", "StraightHorizontal", "RightDown", "LeftDown", "RightUp", "LeftUp" };

const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>>& TileConnectionTable::GetTable()
{
	return s_tileConnectionTable;
}

const std::vector<std::string>& TileConnectionTable::GetDummyArray()
{
	return s_dammyArray;
}

const std::vector<std::string>& TileConnectionTable::GetConnectableTiles(const std::string& tileName, Direction dir)
{
	// 現在のタイル名が空の場合はエラー配列を返す
	if (tileName.empty()) return GetDummyArray();
	auto it = GetTable().find(tileName);
	if (it == GetTable().end()) return GetDummyArray();
	auto dit = it->second.find(dir);
	if (dit == it->second.end()) return GetDummyArray();
	return dit->second;
}
