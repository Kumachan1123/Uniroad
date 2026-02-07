/*
*	@file FileCounter.h
*	@brief ファイルカウンタークラス
*/
#pragma once
// 標準ライブラリ
#include <string>
#include <Windows.h>

// ファイルカウンタークラス
class FileCounter
{
public:
	// 静的public関数
	// 指定したフォルダの中のファイル数をカウントする
	static int CountFilesInFolder(const std::string& folderPath, const std::string& extension);

};