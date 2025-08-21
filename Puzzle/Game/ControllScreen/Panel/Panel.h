/*
*	@file Panel.h
*	@brief パネルクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "Game/ControllScreen/Canvas/Canvas.h"
#include "Game/Interface/IGameUI.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/CSVItem/CSVItem.h"
#include "Game/MainScreen/Tiles/TileDatas/TileDatas.h"
#include "Game/MainScreen/Items/ItemDatas/ItemDatas.h"
#include "Game/Mouse/Mouse.h"

// 前方宣言
class CommonResources;
class CSVItem;

// パネルクラス
class Panel : public IGameUI
{
public:
	// 構造体
	// アイテムの情報
	struct ItemInfo
	{
		int row;		// 行番号
		int col;		// 列番号
		bool isCollected;	// 収集済みかどうか
	};
public:
	// アクセサ
	// UIにヒットしたかどうか取得
	bool GetIsHit()const { return m_hit; }
	// UIにヒットしたかどうか設定
	void SetIsHit(bool hit) { m_hit = hit; }
	// CSVマップを設定
	void SetCSVMap(CSVMap* pCSVMap) { m_pCSVMap = pCSVMap; }
	// CSVアイテムを設定
	void SetCSVItem(CSVItem* pCSVItem) { m_pCSVItem = pCSVItem; }
	// マウスポインターを設定
	void SetMouse(MyMouse* pMouse) { m_pMouse = pMouse; }
	// ビューポートを設定
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }
	// プレイヤーの座標を設定
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position) { m_playerPosition = position; }

public:
	// public関数
	// コンストラクタ
	Panel(int mapSizeX, int mapSizeY);
	// デストラクタ
	~Panel();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height) override;
	// 更新
	void Update(const float elapsedTime) override;
	// プレイヤーのアイコンの更新
	void UpdatePlayerIcons(const float elapsedTime);
	// 描画
	void Render() override;
	// UI追加
	void Add(
		const std::string& key,						// テクスチャのキー
		const DirectX::SimpleMath::Vector2& position,	// 位置
		const DirectX::SimpleMath::Vector2& scale,	// スケール
		KumachiLib::ANCHOR anchor,					// アンカー
		UIType type) override;						// UIの種類

	// タイル情報の描画
	void DrawTiles();
	// アイテム情報の描画
	void DrawItems();

private:
	// private関数
	// アイテムの配置
	void PlaceItems(const MapItemData& itemData, int row, int col, const DirectX::SimpleMath::Vector2& pos);
	// プレイヤーの配置
	void PlacePlayer(const MapTileData& tileData, int row, int col, const DirectX::SimpleMath::Vector2& pos);
private:
	// private定数
	// 1タイルの幅
	static const float TILE_SIZE;
	// タイルの数（0～5）
	static const int TILE_COUNT;
	// タイルの座標の補正値
	static const float TILE_POSITION_CORRECTION;
	// タイルの数（補正値考慮）
	static const float TILE_COUNT_CORRECTED;
	// 3D空間上のプレイヤーの座標の補正値
	static const float PLAYER_POSITION_CORRECTION;
	// 3D空間上のタイルの幅（補正値考慮）
	static const float TILE_SIZE_3D;
	// 描画オフセットX
	static const float DRAW_OFFSET_X;
	// 描画オフセットY
	static const float DRAW_OFFSET_Y;

private:
	// メンバ変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// タイル
	std::vector<std::unique_ptr<Canvas>> m_pTiles;
	// アイテム
	std::vector<std::pair<std::unique_ptr<Canvas>, ItemInfo>> m_pItems;
	// プレイヤーのアイコン
	std::vector<std::unique_ptr<Canvas>> m_pPlayerIcons;
	// マウスのポインター
	MyMouse* m_pMouse;
	// プレイヤーの座標
	DirectX::SimpleMath::Vector3 m_playerPosition;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 時間
	float m_time;
	// ヒットフラグ
	bool m_hit;
	// ヒットフラグが立ったUIのインデックス
	int m_menuIndex;
	// マップサイズ
	int m_mapSizeX, m_mapSizeY;
	// CSVマップ
	CSVMap* m_pCSVMap;
	// CSVアイテム
	CSVItem* m_pCSVItem;
	// 操作用ビューポート
	D3D11_VIEWPORT m_viewPortControll;
	// 行（保存用）
	int m_row;
	// 列（保存用）
	int m_col;
};