#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <DeviceResources.h>
#include <SimpleMath.h>
#include <GeometricPrimitive.h>

#include <Game\GameObject\IGameObject.h>
#include <Game\MyResources\MyResources.h>

class TileMap : public IGameObject
{
public:
	static constexpr float TILE_SIZE = 1.0f;
	static constexpr float TILE_HEIGHT = 0.1f;
	static constexpr int CHUNK_SIZE = 6;
	static constexpr int RENDER_RADIUS = 10;

	using TileId = int;

	struct TileDefinition
	{
		std::string name;
		DirectX::XMFLOAT4 color;
		bool walkable;
	};

	struct Chunk
	{
		int chunkX = 0;
		int chunkY = 0;
		std::vector<std::pair<int, int>> tiles;
	};

	using MapData = std::vector<std::vector<TileId>>;

public:
	TileMap();
	~TileMap() override;

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) override;
	void Finalize() override;

	void SetMapOffset(const DirectX::SimpleMath::Vector2& offset) { m_mapOffset = offset; }
	void RegisterTileDefinition(TileId id, const std::string& name, const DirectX::XMFLOAT4& color, bool walkable);
	void SetRenderCenter(const DirectX::SimpleMath::Vector3& center) { m_renderCenter = center; }
	bool IsWalkableAt(int x, int y) const;
	bool IsInsideMap(int x, int y) const;
	bool IsWalkableAtWorld(const DirectX::SimpleMath::Vector3& worldPosition) const;

private:
	void DrawChunk(const Chunk& chunk, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	void BuildChunks();
	void DrawTile(int x, int y, TileId id, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	const TileDefinition& GetTileDefinition(TileId id) const;
	MapData LoadMapFromCSV(const std::wstring& filePath) const;
	TileId ParseTileToken(const std::string& token) const;
	void RegisterDefaultTileDefinitions();
	void GetVisibleChunkRange(int& minChunkX, int& maxChunkX, int& minChunkY, int& maxChunkY) const;
	std::pair<int, int> MapWorldToMap(const DirectX::SimpleMath::Vector3& worldPosition) const;

private:
	std::unordered_map<TileId, TileDefinition> m_tileDefinitions;
	std::unique_ptr<DirectX::GeometricPrimitive> m_tilePrimitive;
	MapData m_map;
	std::vector<Chunk> m_chunks;
	int m_mapWidth;
	int m_mapHeight;
	int m_chunkWidth;
	int m_chunkHeight;
	DirectX::SimpleMath::Vector2 m_mapOffset;
	DirectX::SimpleMath::Vector3 m_renderCenter;
};

#endif // TILEMAP_H
