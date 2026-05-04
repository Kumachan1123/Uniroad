#include <pch.h>
#include "OverWorldMap.h"
#include <fstream>
#include <sstream>
#include <string>
/*
* 	@brief コンストラクタ
* 	@details OverWorldMapクラスのコンストラクタ
* 	@param なし
*/
OverWorldMap::OverWorldMap()
	: m_mapTiles()// マップタイルのコンテナ
{
	Initialize();
}
/*
* 	@brief 初期化
* 	@details OverWorldMapクラスの初期化
* 	@param なし
* 	@return なし
*/
void OverWorldMap::Initialize()
{
	CSVMapLoad("Resources/Map/Map.csv"); // CSVファイルからマップデータを読み込む

}

void OverWorldMap::Update(float elapsedTime)
{
	// 各マップタイルの更新
	for (auto& tile : m_mapTiles)
	{
		tile->Update(elapsedTime);
	}
}

void OverWorldMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// 各マップタイルの描画
	for (auto& tile : m_mapTiles)
	{
		tile->Render(view, projection);
	}
}

void OverWorldMap::Finalize()
{
	// 各マップタイルの終了処理
	for (auto& tile : m_mapTiles)
	{
		tile->Finalize();
	}
}
/*
*	@brief CSVマップの読み込み
*	@details CSVファイルからマップデータを読み込む関数
*	@param filename CSVファイルのパス
*	@return なし
*/
void OverWorldMap::CSVMapLoad(const std::string& filename)
{
	const float tileInterval = 1.0f;
	const float startX = -1.0f;
	const float startZ = -1.0f;

	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		return;
	}

	std::string line;
	int row = 0;
	while (std::getline(ifs, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::stringstream ss(line);
		std::string cell;
		int col = 0;
		while (std::getline(ss, cell, ','))
		{
			if (cell.empty())
			{
				++col;
				continue;
			}

			auto tile = std::make_unique<MapTile>();
			tile->SetFrame(36, 40); // マップチップの行数と列数を設定
			tile->SetChipNum(std::stoi(cell)); // CSVの数値をチップ番号に設定
			tile->SetMapPosition(row, col); // マップ上の位置を設定
			m_mapTiles.push_back(std::move(tile)); // タイルをコンテナに追加
			++col;
		}
		++row;
	}
}
