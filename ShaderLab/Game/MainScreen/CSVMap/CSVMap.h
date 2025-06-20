/*
*	@file CSVMap.h
*	@brief CSV形式のマップを読み込み、描画するクラス
*/
#pragma once
// 標準ライブラリ
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cassert>
// DirectX関連
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/DrawCollision/DrawCollision.h"

// 前方宣言
class CommonResources;

// CSV形式のマップを読み込む
class CSVMap
{
public:
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
public:
	// アクセサ
	// 指定位置のタイル情報を取得する
	const MapTileData& GetTileData(int col, int row) const;
	// マップの最大列数と行数を取得する
	const int GetMaxCol() const { return MAXCOL; }// 列数
	const int GetMaxRow() const { return MAXRAW; }// 行数
public:
	CSVMap(CommonResources* resources);
	~CSVMap();
	void LoadModel();
	void LoadMap(const std::string& filePath);
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
private:
	void InitializeTileDictionary();

private:
	CommonResources* m_commonResources;
	//マップ
	const int MAXCOL = 5;
	const int MAXRAW = 5;
	// タイルの辞書
	std::unordered_map<std::string, TileInfo> m_tileDictionary;
	// タイルのレンダリングデータ
	std::vector<TileRenderData> m_tiles;
	// マップデータ
	std::vector<std::vector<MapTileData>> m_mapData;

};