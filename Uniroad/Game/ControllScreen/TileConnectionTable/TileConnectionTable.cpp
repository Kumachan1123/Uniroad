/*
*	@file		TileConnectionTable.cpp
*	@brief		接続可能なタイルのテーブルクラス
*/
#include "pch.h"
#include "TileConnectionTable.h"
// タイル種ごとの進行可能タイル
static const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>> s_tileConnectionTable =
{
	// タイル名、進行方向、接続可能なタイル名のリスト
	// デフォルトの縦方向ブロック
	{"DefaultStraightVerticalBlock",
	{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},// 上方向に進む場合
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}}// 下方向に進む場合
	}},
	// デフォルトの横方向ブロック
	{"DefaultStraightHorizontalBlock",
	{
		{Direction::LEFT,{ "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},// 左方向に進む場合
		{Direction::RIGHT,{  "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}}// 右方向に進む場合
	}},
	// 縦方向ブロック
	{"StraightVerticalBlock",
	{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},// 上方向に進む場合
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}}// 下方向に進む場合
	}},
	// 横方向ブロック
	{"StraightHorizontalBlock",
	{
		{Direction::LEFT,{ "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},// 左方向に進む場合
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}}// 右方向に進む場合
	}},
	// 右下カーブブロック
	{"RightDownBlock",
	{
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},// 下方向に進む場合
		{Direction::LEFT,{  "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},// 左方向に進む場合
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},// 右方向に進む場合
		{Direction::UP,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},// 上方向に進む場合
	}},
	// 左下カーブブロック
	{"LeftDownBlock",
	{
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},// 下方向に進む場合
		{Direction::RIGHT,{"StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},// 右方向に進む場合
		{Direction::UP,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},// 上方向に進む場合
		{Direction::LEFT,{"StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},// 左方向に進む場合
	}},
	// 右上カーブブロック
	{"RightUpBlock",
	{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},// 上方向に進む場合
		{Direction::LEFT,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},// 左方向に進む場合
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},// 右方向に進む場合
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},// 右方向に進む場合
	}},
	// 左上カーブブロック
	{"LeftUpBlock",
	{
		{Direction::UP,{ "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},// 上方向に進む場合
		{Direction::DOWN,{   "StraightVerticalBlock","RightDownBlock", "RightUpBlock"}},// 下方向に進む場合
		{Direction::RIGHT,{ "StraightHorizontalBlock","RightDownBlock", "LeftDownBlock"}},// 右方向に進む場合
		{Direction::LEFT,{   "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},// 左方向に進む場合
	}},
	// 十字ブロック
	{"CrossBlock",
	{
		{Direction::UP,{  "StraightVerticalBlock","RightDownBlock", "LeftDownBlock"}},// 上方向に進む場合
		{Direction::DOWN,{ "StraightVerticalBlock","RightUpBlock", "LeftUpBlock"}},// 下方向に進む場合
		{Direction::LEFT,{ "StraightHorizontalBlock","RightDownBlock", "RightUpBlock"}},// 左方向に進む場合
		{Direction::RIGHT,{ "StraightHorizontalBlock","LeftDownBlock", "LeftUpBlock"}},// 右方向に進む場合
	}}
};
// 仮の配列(エラー用)
static const std::vector<std::string> s_dammyArray =
{ "StraightVerticalBlock", "StraightHorizontalBlock", "RightDownBlock", "LeftDownBlock", "RightUpBlock", "LeftUpBlock" };
/*
*	@brief 接続可能なタイルのテーブルを取得する
*	@details 接続可能なタイルのテーブルを取得する
*	@param なし
*	@return 接続可能なタイルのテーブル
*/
const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>>& TileConnectionTable::GetTable() { return s_tileConnectionTable; }
/*
*	@brief 仮の配列を返す関数
*	@details 接続可能なタイルがない、またはエラーが発生した場合に返す仮の配列を返す
*	@param なし
*	@return 仮の配列
*/
const std::vector<std::string>& TileConnectionTable::GetDummyArray() { return s_dammyArray; }
/*
*	@brief 指定されたタイル名と方向に基づいて接続可能なタイルのリストを取得する
*	@details 指定されたタイル名と方向に基づいて接続可能なタイルのリストを取得する
*	@param tileName タイル名
*	@param dir 進行方向
*	@return 接続可能なタイルのリスト
*/
const std::vector<std::string>& TileConnectionTable::GetConnectableTiles(const std::string& tileName, Direction dir)
{
	// 現在のタイル名が空の場合はエラー配列を返す
	if (tileName.empty()) return GetDummyArray();
	// テーブルからタイル名をキーにして進行方向に対応する接続可能なタイルのリストを取得する
	auto it = GetTable().find(tileName);
	// 見つからなかった場合はエラー配列を返す
	if (it == GetTable().end()) return GetDummyArray();
	// 進行方向に対応する接続可能なタイルのリストを取得する
	auto dit = it->second.find(dir);
	// 見つからなかった場合はエラー配列を返す
	if (dit == it->second.end()) return GetDummyArray();
	// 接続可能なタイルのリストを返す
	return dit->second;
}
