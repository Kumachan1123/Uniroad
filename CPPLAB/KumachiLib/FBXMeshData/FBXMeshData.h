#pragma once

#include <vector>
#include <string>

#include <SimpleMath.h>


/**
 * @brief FBX頂点
 */
struct ModelVertex
{
	DirectX::SimpleMath::Vector3 Position;

	DirectX::SimpleMath::Vector3 Normal;

	DirectX::SimpleMath::Vector2 UV;
};


/**
 * @brief FBXメッシュデータ
 */
struct MeshData
{
	std::vector<ModelVertex> Vertices;

	std::vector<uint32_t> Indices;

	uint32_t MaterialIndex = 0;
};