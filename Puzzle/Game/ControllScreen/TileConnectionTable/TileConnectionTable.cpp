#include "pch.h"
#include "TileConnectionTable.h"
// タイル種ごとの進行可能タイル
static const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>> s_tileConnectionTable =
{
	{"DefaultStraightVerticalBlock",
	{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}}
	}},
	{"DefaultStraightHorizontalBlock",
	{
		{Direction::LEFT,{ "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},
		{Direction::RIGHT,{  "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}}
	}},
	{"StraightVerticalBlock",
	{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}}
	}},
	{"StraightHorizontalBlock",
	{
		{Direction::LEFT,{ "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}}
	}},
	{"RightDownBlock",{
	{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},
	{Direction::LEFT,{  "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},
	{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},
	{Direction::UP,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},
	}},
	{"LeftDownBlock",{
	{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},
	{Direction::RIGHT,{"StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},
	{Direction::UP,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},
	{Direction::LEFT,{"StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}}}},
	{"RightUpBlock",{
	{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},
	{Direction::LEFT,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},
	{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},
	{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}}}},
	{"LeftUpBlock",{
		{Direction::UP,{ "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},
		{Direction::DOWN,{   "StraightVerticalBlock","RightDownBlock", "RightUpBlock"}},
		{Direction::RIGHT,{ "StraightHorizontalBlock","RightDownBlock", "LeftDownBlock"}},
		{Direction::LEFT,{   "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}}}},
	{"CrossBlock",{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},
		{Direction::LEFT,{ "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}}}}
};

static const std::vector<std::string> s_dammyArray = { "StraightVerticalBlock", "StraightHorizontalBlock", "RightDownBlock", "LeftDownBlock", "RightUpBlock", "LeftUpBlock" };

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
