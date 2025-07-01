#pragma once
// // 標準ライブラリ
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
// タイルの情報構造体
struct TileInfo
{
	std::string modelName;                   // 使用するモデル名
	bool hasCollision = false;              // 当たり判定を持つか
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f }; // スケーリング
};
// タイルのレンダリングデータ構造体
struct TileRenderData
{
	DirectX::Model* model;
	DirectX::SimpleMath::Matrix world;
};
// マップの1マス分の情報
struct MapTileData
{
	TileInfo tileInfo;               // その場所のタイル種類（Block、Start、Goalなど）
	DirectX::SimpleMath::Vector3 pos; // そのマスのワールド座標
	bool hasCollision = false;               // 当たり判定あるか
};