/*
*	@file CSVItem.h
*	@brief CSV形式のアイテム分布を読み込み、描画するクラス
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
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/MainScreen/Items/ItemDatas/ItemDatas.h"
#include "Game/MainScreen/Items/ItemFactory/ItemFactory.h"
// 前方宣言
class CommonResources;

// CSV形式のマップを読み込む
class CSVItem
{
public:
	// アクセサ
	// 指定位置のタイルのアイテム情報を取得する(行, 列)
	const MapItemData& GetItemData(int row, int col) const;
	// 指定座標のタイルのアイテム情報を取得する(ワールド座標)
	const MapItemData& GetItemData(const DirectX::SimpleMath::Vector3& pos) const;
	// 指定座標のタイルのアイテム情報を消す(獲得後に呼ぶ)(行, 列)
	void  RemoveItem(int row, int col);


	// マップの最大列数と行数を取得する
	const int GetMaxCol() const { return MAXCOL; }// 列数
	const int GetMaxRow() const { return MAXRAW; }// 行数
	// 獲得された枚数を加算する
	void CountMedals() { m_collectedMedals++; }
	// 獲得されたメダルの枚数を取得する
	int GetCollectedMedals() const { return m_collectedMedals; }
public:
	// public関数
	// コンストラクタ
	CSVItem(CommonResources* resources);
	// デストラクタ
	~CSVItem();
	// CSV形式のアイテム配置を読み込む
	void LoadItem(const std::string& filePath);
	// 更新処理
	void Update(float elapsedTime);
	// 当たり判定描画
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
private:
	// private関数
	// 辞書を初期化する
	void InitializeTileDictionary();

private:
	CommonResources* m_pCommonResources;
	//マップ
	// 最大行
	const int MAXRAW = 5;
	// 最大列
	const int MAXCOL = 5;
	// タイルの辞書
	std::unordered_map<std::string, ItemInfo> m_tileDictionary;
	// タイルのレンダリングデータ
	std::vector<ItemRenderData> m_tiles;
	// マップデータ
	std::vector<std::vector<MapItemData>> m_mapItemData;
	// アイテム配列
	std::vector<std::unique_ptr<ItemBase>> m_itemArray;
	// 時間
	float m_time;
	// 獲得されたメダル
	int m_collectedMedals;
};