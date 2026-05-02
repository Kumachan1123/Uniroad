#include "pch.h"
#include "TileMap.h"

#include <fstream>
#include <sstream>

namespace
{
	TileMap::MapData BuildFallbackMap()
	{
		TileMap::MapData map(12, std::vector<TileMap::TileId>(20, 0));
		for (auto& row : map)
		{
			for (std::size_t x = 0; x < row.size(); ++x)
			{
				row[x] = 0;
			}
		}
		return map;
	}
}

TileMap::TileMap()
	: m_tileDefinitions()
	, m_tilePrimitive(nullptr)
	, m_map(BuildFallbackMap())
	, m_chunks()
	, m_mapWidth(static_cast<int>(m_map.empty() ? 0 : m_map.front().size()))
	, m_mapHeight(static_cast<int>(m_map.size()))
	, m_chunkWidth(0)
	, m_chunkHeight(0)
	, m_mapOffset(0.0f, 0.0f)
	, m_renderCenter(0.0f, 0.0f, 0.0f)
{
	RegisterDefaultTileDefinitions();
}

TileMap::~TileMap()
{
	Finalize();
}

void TileMap::RegisterDefaultTileDefinitions()
{
	RegisterTileDefinition(0, "grass", DirectX::XMFLOAT4(0.30f, 0.62f, 0.24f, 1.0f), true);
	RegisterTileDefinition(1, "road", DirectX::XMFLOAT4(0.78f, 0.66f, 0.38f, 1.0f), true);
	RegisterTileDefinition(10, "wall", DirectX::XMFLOAT4(0.45f, 0.45f, 0.48f, 1.0f), false);
}

void TileMap::RegisterTileDefinition(TileId id, const std::string& name, const DirectX::XMFLOAT4& color, bool walkable)
{
	m_tileDefinitions[id] = TileDefinition{ name, color, walkable };
}

void TileMap::Initialize()
{
	auto context = MyResourecs::Get().GetDeviceResources()->GetD3DDeviceContext();
	m_tilePrimitive = DirectX::GeometricPrimitive::CreateBox(context, DirectX::SimpleMath::Vector3(TILE_SIZE, TILE_HEIGHT, TILE_SIZE));

	const auto map = LoadMapFromCSV(L"Resources/MapData/MapData.csv");
	if (!map.empty())
	{
		m_map = map;
	}
	m_mapHeight = static_cast<int>(m_map.size());
	m_mapWidth = m_mapHeight > 0 ? static_cast<int>(m_map.front().size()) : 0;
	m_chunkWidth = (m_mapWidth + CHUNK_SIZE - 1) / CHUNK_SIZE;
	m_chunkHeight = (m_mapHeight + CHUNK_SIZE - 1) / CHUNK_SIZE;
	BuildChunks();
}

void TileMap::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void TileMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	if (!m_tilePrimitive)
	{
		return;
	}

	int minChunkX = 0;
	int maxChunkX = m_chunkWidth - 1;
	int minChunkY = 0;
	int maxChunkY = m_chunkHeight - 1;
	GetVisibleChunkRange(minChunkX, maxChunkX, minChunkY, maxChunkY);

	const bool hasVisibleChunks = (minChunkX <= maxChunkX) && (minChunkY <= maxChunkY);
	for (const auto& chunk : m_chunks)
	{
		if (hasVisibleChunks)
		{
			if (chunk.chunkX < minChunkX || chunk.chunkX > maxChunkX || chunk.chunkY < minChunkY || chunk.chunkY > maxChunkY)
			{
				continue;
			}
		}

		DrawChunk(chunk, view, projection);
	}
}

void TileMap::Finalize()
{
	m_chunks.clear();
	m_tilePrimitive.reset();
}

bool TileMap::IsWalkableAtWorld(const DirectX::SimpleMath::Vector3& worldPosition) const
{
	const auto mapPoint = MapWorldToMap(worldPosition);
	return IsWalkableAt(mapPoint.first, mapPoint.second);
}

std::pair<int, int> TileMap::MapWorldToMap(const DirectX::SimpleMath::Vector3& worldPosition) const
{
	const int mapX = static_cast<int>(std::round((static_cast<float>(m_mapWidth - 1) - worldPosition.x) - m_mapOffset.x));
	const int mapY = static_cast<int>(std::round((static_cast<float>(m_mapHeight - 1) - worldPosition.z) - m_mapOffset.y));
	return { mapX, mapY };
}

bool TileMap::IsInsideMap(int x, int y) const
{
	return x >= 0 && y >= 0 && x < m_mapWidth && y < m_mapHeight;
}

bool TileMap::IsWalkableAt(int x, int y) const
{
	if (!IsInsideMap(x, y))
	{
		return false;
	}

	const auto id = m_map[y][x];
	const auto& def = GetTileDefinition(id);
	return def.walkable;
}

void TileMap::BuildChunks()
{
	m_chunks.clear();
	m_chunks.resize(static_cast<std::size_t>(m_chunkWidth * m_chunkHeight));

	for (int cy = 0; cy < m_chunkHeight; ++cy)
	{
		for (int cx = 0; cx < m_chunkWidth; ++cx)
		{
			auto& chunk = m_chunks[static_cast<std::size_t>(cy * m_chunkWidth + cx)];
			chunk.chunkX = cx;
			chunk.chunkY = cy;
			chunk.tiles.clear();

			const int startX = cx * CHUNK_SIZE;
			const int startY = cy * CHUNK_SIZE;
			const int endX = std::min(startX + CHUNK_SIZE, m_mapWidth);
			const int endY = std::min(startY + CHUNK_SIZE, m_mapHeight);

			for (int y = startY; y < endY; ++y)
			{
				for (int x = startX; x < endX; ++x)
				{
					const auto id = m_map[y][x];
					if (id < 0)
					{
						continue;
					}

					chunk.tiles.emplace_back(x, y);
				}
			}
		}
	}
}

void TileMap::DrawChunk(const Chunk& chunk, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	for (const auto& tile : chunk.tiles)
	{
		const int x = tile.first;
		const int y = tile.second;
		DrawTile(x, y, m_map[y][x], view, projection);
	}
}

void TileMap::DrawTile(int x, int y, TileId id, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	const auto& def = GetTileDefinition(id);
	const float worldX = static_cast<float>(m_mapWidth - 1 - x) + 0.5f + m_mapOffset.x;
	const float worldZ = static_cast<float>(m_mapHeight - 1 - y) + 0.5f + m_mapOffset.y;
	const auto world = DirectX::SimpleMath::Matrix::CreateTranslation(
		worldX,
		-TILE_HEIGHT * 0.5f,
		worldZ);

	m_tilePrimitive->Draw(world, view, projection, DirectX::XMLoadFloat4(&def.color));
}

const TileMap::TileDefinition& TileMap::GetTileDefinition(TileId id) const
{
	auto it = m_tileDefinitions.find(id);
	if (it == m_tileDefinitions.end())
	{
		return m_tileDefinitions.at(0);
	}
	return it->second;
}

void TileMap::GetVisibleChunkRange(int& minChunkX, int& maxChunkX, int& minChunkY, int& maxChunkY) const
{
	const int mapSpaceX = static_cast<int>(std::floor((static_cast<float>(m_mapWidth - 1) - m_renderCenter.x) - m_mapOffset.x));
	const int mapSpaceY = static_cast<int>(std::floor((static_cast<float>(m_mapHeight - 1) - m_renderCenter.z) - m_mapOffset.y));
	const int minX = std::max(0, mapSpaceX - RENDER_RADIUS);
	const int maxX = std::min(m_mapWidth - 1, mapSpaceX + RENDER_RADIUS);
	const int minY = std::max(0, mapSpaceY - RENDER_RADIUS);
	const int maxY = std::min(m_mapHeight - 1, mapSpaceY + RENDER_RADIUS);

	minChunkX = std::max(0, minX / CHUNK_SIZE - 2);
	maxChunkX = std::min(m_chunkWidth - 1, maxX / CHUNK_SIZE + 2);
	minChunkY = std::max(0, minY / CHUNK_SIZE - 2);
	maxChunkY = std::min(m_chunkHeight - 1, maxY / CHUNK_SIZE + 2);

	if (m_chunkWidth <= 0 || m_chunkHeight <= 0)
	{
		minChunkX = 0;
		maxChunkX = -1;
		minChunkY = 0;
		maxChunkY = -1;
	}
}

TileMap::MapData TileMap::LoadMapFromCSV(const std::wstring& filePath) const
{
	std::string narrowPath(filePath.begin(), filePath.end());
	std::ifstream file(narrowPath);
	if (!file)
	{
		return {};
	}

	MapData map;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::vector<TileId> row;
		std::stringstream ss(line);
		std::string token;
		while (std::getline(ss, token, ','))
		{
		row.push_back(ParseTileToken(token));
		}

		if (!row.empty())
		{
			map.push_back(std::move(row));
		}
	}

	if (map.empty())
	{
		return {};
	}

	const auto width = map.front().size();
	for (const auto& row : map)
	{
		if (row.size() != width)
		{
			return {};
		}
	}

	return map;
}

TileMap::TileId TileMap::ParseTileToken(const std::string& token) const
{
	try
	{
		return std::stoi(token);
	}
	catch (...)
	{
		return 0;
	}
}
