#pragma once
// // 標準ライブラリ
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>

// 前方宣言
class TileBase;

// タイルの情報構造体
struct TileInfo
{
	// 使用するモデル名
	std::string modelName;
	// スケーリング
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };
};
// タイルのレンダリングデータ構造体
struct TileRenderData
{
	// 使用するモデル
	DirectX::Model* model;
	// ワールド行列
	DirectX::SimpleMath::Matrix world;
};
// マップの1マス分の情報
struct MapTileData
{
	// その場所のタイル種類（Block、Start、Goalなど）
	TileInfo tileInfo;
	// そのマスのワールド座標
	DirectX::SimpleMath::Vector3 pos;
	// タイルのベースクラスへのポインタ
	std::unique_ptr<TileBase> tileBasePtr = nullptr;

};