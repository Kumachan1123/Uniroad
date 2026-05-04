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
	: m_mapLayer1() // マップタイルのコンテナ
	, m_mapLayer2() // マップのレイヤー2（必要に応じて使用）
	, m_collisionLayer() // 当たり判定タイルのコンテナ
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
	CSVMapLoad("Resources/Map/Map.csv", m_mapLayer1); // CSVファイルからマップデータを読み込む
	//CSVMapLoad("Resources/Map/Map.csv", m_mapLayer2); // CSVファイルからマップデータを読み込む
	CSVCollisionLoad("Resources/Map/CollisonMap.csv"); // CSVファイルから当たり判定データを読み込む
}

bool OverWorldMap::IsWalkable(int row, int col) const
{
	for (const auto& tile : m_collisionLayer)
	{
		if (tile != nullptr && tile->GetRow() == row && tile->GetCol() == col)
		{
			return !tile->HasCollision();
		}
	}
	return true;
}

void OverWorldMap::Update(float elapsedTime)
{
	// 各マップタイルの更新
	for (auto& tile : m_mapLayer1)
	{
		if (tile != nullptr)
			tile->Update(elapsedTime);
	}
	for (auto& tile : m_mapLayer2)
	{
		if (tile != nullptr)
			tile->Update(elapsedTime);
	}

}
void OverWorldMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// 各マップタイルの描画
	for (auto& tile : m_mapLayer1)
	{
		tile->Render(view, projection);
	}
	for (auto& tile : m_mapLayer2)
	{
		tile->Render(view, projection);
	}

}
void OverWorldMap::Finalize()
{
	// 各マップタイルの終了処理
	for (auto& tile : m_mapLayer1)
	{
		tile->Finalize();
	}
	for (auto& tile : m_mapLayer2)
	{
		tile->Finalize();
	}
	for (auto& tile : m_collisionLayer)
	{
		tile->Finalize();
	}
}
/*
* 	@brief CSVマップの読み込み
* 	@details CSVファイルからマップデータを読み込む関数
* 	@param filename CSVファイルのパス
* 	@param pMap マップタイルのコンテナ
* 	@return なし
*/
void OverWorldMap::CSVMapLoad(const std::string& filename, std::vector<std::unique_ptr<MapTile>>& pMap)
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
			tile->SetPosition(DirectX::SimpleMath::Vector3(startX + col * tileInterval, -1.0f, startZ + row * tileInterval));
			pMap.push_back(std::move(tile)); // タイルをコンテナに追加
			++col;
		}
		++row;
	}
}

void OverWorldMap::CSVCollisionLoad(const std::string& filename)
{
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
			auto tile = std::make_unique<CollisionTile>();
			bool hasCollision = std::stoi(cell) == 1; // CSVの数値が1なら当たり判定あり、0ならなし
			tile->SetHasCollision(hasCollision); // CSVの数値が1なら当たり判定あり、0ならなし
			tile->SetMapPosition(row, col); // マップ上の位置を設定
			m_collisionLayer.push_back(std::move(tile)); // タイルをコンテナに追加
			++col;
		}
		++row;
	}
}

