/*
*   @file FileCounter.cpp
*   @brief ファイルカウンタークラスの実装
*/
#include "pch.h"
#include "FileCounter.h"
/*
*   @brief 指定したフォルダの中のファイル数をカウントする
*   @details 指定されたフォルダパス内のファイル数をカウントする関数
*   @param folderPath カウントするフォルダのパス
*	@param extension ファイルの拡張子（例: ".csv"）
*   @return ファイル数
*/
int FileCounter::CountFilesInFolder(const std::string& folderPath, const std::string& extension)
{
	// カウント用変数
	int count = 0;
	// 検索用のパス
	std::string searchPath = folderPath + "\\*" + extension;
	// WIN32_FIND_DATA構造体のインスタンスを作成
	WIN32_FIND_DATAA findFileData{};
	// FindFirstFileA関数を使用してファイル検索を開始
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);
	// ファイルが見つかった場合
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// ファイルがディレクトリでない場合、カウントを増やす
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				count++;
			}
		} while (FindNextFileA(hFind, &findFileData) != 0); // 次のファイルを検索
		// 検索ハンドルを閉じる
		FindClose(hFind);
	}
	// ファイル数を返す
	return count;
}
