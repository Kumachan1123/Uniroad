#pragma once
#ifndef OVER_WORLD_MAP_H
#define OVER_WORLD_MAP_H
#include <Game\GameObject\IGameObject.h>
#include <Game\Map\MapTile.h>
#include <Game\MyResources\MyResources.h>

class OverWorldMap : public IGameObject
{
public:
	// 任意のタイル番号のタイルを取得する関数
	MapTile* GetTile(int row, int col)const;
public:
	OverWorldMap();
	~OverWorldMap() = default;
	void Initialize()override;
	void Update(float elapsedTime)override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;
	void Finalize()override;
private:
	void CSVMapLoad(const std::string& filename); // CSVファイルからマップデータを読み込む関数
private:
	std::vector<std::unique_ptr<MapTile>> m_mapTiles; // マップタイルのコンテナ
};
#endif // !OVER_WORLD_MAP_H
