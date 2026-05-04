#pragma once
#ifndef OVER_WORLD_MAP_H
#define OVER_WORLD_MAP_H
#include <Game\GameObject\IGameObject.h>
#include <Game\Map\MapTile.h>
#include <Game\CollisionTile\CollisionTile.h>
#include <Game\MyResources\MyResources.h>

class OverWorldMap : public IGameObject
{
public:
	// 任意のタイル番号のタイルを取得する関数
public:
	OverWorldMap();
	~OverWorldMap() = default;
	void Initialize()override;
	void Update(float elapsedTime)override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;
	void Finalize()override;

	bool IsWalkable(int row, int col) const;
private:
	void CSVMapLoad(const std::string& filename, std::vector<std::unique_ptr<MapTile>>& pMap); // CSVファイルからマップデータを読み込む関数
	void CSVCollisionLoad(const std::string& filename); // CSVファイルから当たり判定データを読み込む関数
private:
	std::vector<std::unique_ptr<MapTile>> m_mapLayer1; // マップタイルのコンテナ
	std::vector<std::unique_ptr<MapTile>> m_mapLayer2; // マップのレイヤー2（必要に応じて使用）
	std::vector<std::unique_ptr<CollisionTile>> m_collisionLayer; // 当たり判定タイルのコンテナ
};
#endif // !OVER_WORLD_MAP_H
