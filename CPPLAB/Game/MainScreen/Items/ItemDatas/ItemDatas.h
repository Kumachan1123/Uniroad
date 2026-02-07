/*
*	@file	ItemDatas.h
*	@brief	アイテムの情報構造体をまとめたヘッダーファイル
*/
#pragma once
// 標準ライブラリ
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>

// 前方宣言
class ItemBase;

// アイテムの情報構造体
struct  ItemInfo
{
	// 使用するモデル名
	std::string modelName;
	// スケーリング
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };
};
// アイテムのレンダリングデータ構造体
struct  ItemRenderData
{
	// モデルデータ
	DirectX::Model* model;
	// ワールド行列
	DirectX::SimpleMath::Matrix world;
};
// マップの1マス分の情報
struct MapItemData
{
	// その場所のアイテムの種類（Block、Start、Goalなど）
	ItemInfo itemInfo;
	// そのマスのワールド座標
	DirectX::SimpleMath::Vector3 pos;
	// アイテムのベースクラスへのポインタ
	std::unique_ptr<ItemBase> itemBasePtr = nullptr;
};