/*
*	@file TileConnectionTable.h
*	@brief 接続可能なタイルのテーブルクラス
*/
#pragma once
// 標準ライブラリ
#include <unordered_map>
#include <string>
#include <vector>

// プレイヤーの進行方向を表す列挙型
enum class Direction
{
	UP,		// 上方向
	DOWN,	// 下方向
	LEFT,	// 左方向
	RIGHT	// 右方向
};

// タイル接続テーブルクラス
class TileConnectionTable
{
public:
	// 静的public関数
	// 接続可能なタイルのテーブルを取得する
	static const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>>& GetTable();
	// 仮の配列を返す関数
	static const std::vector<std::string>& GetDummyArray();
	// 指定されたタイル名と方向に基づいて接続可能なタイルのリストを取得する
	static const std::vector<std::string>& GetConnectableTiles(const std::string& tileName, Direction dir);
};