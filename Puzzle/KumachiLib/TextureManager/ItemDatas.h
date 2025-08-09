#pragma once
// // 標準ライブラリ
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>

// 前方宣言
class ItemBase;
// アイテムの情報構造体
struct  ItemInfo
{
	std::string modelName;                   // 使用するモデル名
	bool hasCollision = false;              // 当たり判定を持つか
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f }; // スケーリング
};
// アイテムのレンダリングデータ構造体
struct  ItemRenderData
{
	DirectX::Model* model;
	DirectX::SimpleMath::Matrix world;
};
// マップの1マス分の情報
struct MapItemData
{
	ItemInfo itemInfo;               // その場所のアイテムの種類（Block、Start、Goalなど）
	DirectX::SimpleMath::Vector3 pos; // そのマスのワールド座標
	std::unique_ptr<ItemBase> itemBasePtr = nullptr; // アイテムのベースクラスへのポインタ
};