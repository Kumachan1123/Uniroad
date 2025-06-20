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
	// タイルの情報構造体
	struct TileInfo
	{
		std::string modelName;                   // 使用するモデル名
		bool hasCollision = false;              // 当たり判定を持つか
		DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f }; // スケーリング
	};
	struct TileRenderData
	{
		DirectX::Model* model;
		DirectX::SimpleMath::Matrix world;
	};
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
	static const int MAXCOL = 5;
	static const int MAXRAW = 5;
	char m_map[MAXCOL][MAXRAW];
	// CSVMap のメンバに追加
	std::unordered_map<std::string, TileInfo> m_tileDictionary;
	std::vector<TileRenderData> m_tiles;

	std::vector<DirectX::BoundingBox> m_wallBox;
	//std::vector<DirectX::SimpleMath::Matrix> worldMatrices;
	DirectX::Model* m_pModel;
	// モデルパス
	wchar_t m_modelPath[256];


};