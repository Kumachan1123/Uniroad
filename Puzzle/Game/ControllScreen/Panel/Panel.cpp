/*
*	@file Panel.cpp
*	@brief パネルクラス
*/
#include <pch.h>
#include "Panel.h"
// 1タイルの幅
const float Panel::TILE_SIZE = 90.0f;
// タイルの枚数(補正値も考慮）
const int Panel::TILE_COUNT = 5;
// タイルの座標の補正値
const float Panel::TILE_POSITION_CORRECTION = 0.6f;
// タイルの枚数（補正値考慮）
const float Panel::TILE_COUNT_CORRECTED = TILE_COUNT - TILE_POSITION_CORRECTION;
// 3D空間上のプレイヤーの座標の補正値
const float Panel::PLAYER_POSITION_CORRECTION = 4.0f;
// 3D空間上のタイルの幅（補正値考慮）
const float Panel::TILE_SIZE_3D = 8.75f;
// 描画オフセットX
const float Panel::DRAW_OFFSET_X = -350.0f;
// 描画オフセットY
const float Panel::DRAW_OFFSET_Y = 420.0f;

/*
*	@brief コンストラクタ
*	@details パネルクラスのコンストラクタ
*	@param mapSizeX マップのサイズX
*	@param mapSizeY マップのサイズY
*	@return なし
*/
Panel::Panel(int mapSizeX, int mapSizeY)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_viewPortControll() // ビューポートの制御
	, m_pDR(nullptr) // デバイスリソース
	, m_pCSVMap(nullptr) // CSVマップへのポインタ
	, m_pCSVItem(nullptr) // CSVアイテムへのポインタ
	, m_pMouse(nullptr) // マウスへのポインタ
	, m_hit(false) // UIにヒットしたかどうか
	, m_time(0.0f) // 経過時間
	, m_windowHeight(0) // ウィンドウの高さ
	, m_windowWidth(0) // ウィンドウの幅
	, m_menuIndex(0) // 現在選択されているメニューのインデックス
	, m_mapSizeX(mapSizeX) // マップのサイズX
	, m_mapSizeY(mapSizeY) // マップのサイズY
	, m_row(-1) // 行番号（保存用）
	, m_col(-1) // 列番号（保存用）
{
}
/*
*	@brief デストラクタ
*	@details パネルクラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
Panel::~Panel()
{
	/*do nothing*/
}
/*
*	@brief 初期化
*	@details パネルクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Panel::Initialize(CommonResources* resources, int width, int height)
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソース取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// ウィンドウ幅
	m_windowWidth = (int)(width * .3f);
	// ウィンドウ高さ
	m_windowHeight = height;
	// 左端の位置
	const float startX = Screen::CENTER_X - (TILE_SIZE * 2);
	// 上端の位置
	const float startY = Screen::CENTER_Y - (TILE_SIZE * 2);
	// グリッドのキーを決定するためのループ
	for (int row = 0; row < m_mapSizeX; ++row)
	{
		for (int col = 0; col < m_mapSizeY; ++col)
		{
			// CSVマップのタイル情報を取得
			const MapTileData& tileData = m_pCSVMap->GetTileData(row, col);
			// グリッドのキーを決定
			std::string textureKey = ((row + col) % 2 == 0) ? "GridA" : "GridB";
			// タイルの種類に応じてテクスチャキーを変更
			textureKey = tileData.tileInfo.modelName.empty() ? textureKey : tileData.tileInfo.modelName;
			// 位置計算
			float posX = startX + col * TILE_SIZE + DRAW_OFFSET_X;
			float posY = startY + row * TILE_SIZE + DRAW_OFFSET_Y;
			Vector2 pos(posX, posY);
			// タイルの種類に応じてテクスチャキーを変更して並べる
			Add(textureKey
				, pos
				, Vector2(TILE_POSITION_CORRECTION)
				, KumachiLib::ANCHOR::MIDDLE_CENTER
				, UIType::TILE);
			// アイテムを配置する
			PlaceItems(m_pCSVItem->GetItemData(row, col), row, col, pos);
			// プレイヤーを配置する
			PlacePlayer(tileData, row, col, pos);

		}
	}
}
/*
*	@brief 更新
*	@details パネルの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Panel::Update(const float elapsedTime)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// マウスのヒットフラグをリセット
	m_pMouse->SetHit(false);
	// マウスのヒットパネルインデックスをリセット
	m_pMouse->SetHitPanelIndex(-1);
	// UI要素ごとにヒット判定を行う
	for (int i = 0; i < m_pTiles.size(); i++)
	{
		// マウスがビューポート外ならスキップ
		if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 ||
			m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() ||
			m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())
			continue;
		// ヒット判定（UI要素ごと）
		if (m_pTiles[i]->IsHit(m_pMouse->GetPosition()))
		{
			m_pMouse->SetHit(true); // マウスのヒットフラグをセット
			m_pMouse->SetHitPanelIndex(i);// 当たったパネルのインデックスをセット
			m_pMouse->SetPanelPosition(m_pTiles[i]->GetPosition()); // 当たったパネルの位置をセット
			// 当たったパネルの行番号を設定
			m_pMouse->SetHitPanelRowIndex(i / m_mapSizeX);
			// 当たったパネルの列番号を設定
			m_pMouse->SetHitPanelColIndex(i % m_mapSizeX);
			break;
		}
	}
	// 経過時間を加算
	m_time += elapsedTime;
	// 全UI要素の経過時間を更新
	// タイルの経過時間を更新
	for (int i = 0; i < m_pTiles.size(); i++)m_pTiles[i]->SetTime(m_pTiles[i]->GetTime() + elapsedTime);
	// アイテムの経過時間を更新
	for (int i = 0; i < m_pItems.size(); i++)
	{
		// アイテムのデータが存在する場合のみ更新
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
			// アイテムの経過時間を更新
			m_pItems[i].first->SetTime(m_pItems[i].first->GetTime() + elapsedTime);

	}
	// プレイヤーアイコンの更新
	UpdatePlayerIcons(elapsedTime);
}
/*
*	@brief プレイヤーアイコンの更新
*	@details プレイヤーアイコンの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Panel::UpdatePlayerIcons(const float elapsedTime)
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 左端の位置
	const float startX = Screen::CENTER_X - (TILE_SIZE * 2);
	// 上端の位置
	const float startY = Screen::CENTER_Y - (TILE_SIZE * 2);
	// 3D空間上の座標（-4～4）をタイル座標（0～5）に線形変換
	float tileCoordX = ((m_playerPosition.x) + PLAYER_POSITION_CORRECTION) * TILE_COUNT_CORRECTED / TILE_SIZE_3D;
	float tileCoordZ = ((m_playerPosition.z) + PLAYER_POSITION_CORRECTION) * TILE_COUNT_CORRECTED / TILE_SIZE_3D;
	// 位置計算
	float posX = startX + (tileCoordX)*TILE_SIZE + DRAW_OFFSET_X;
	float posY = startY + (tileCoordZ)*TILE_SIZE + DRAW_OFFSET_Y;
	Vector2 position(posX, posY);
	// プレイヤーの位置を更新
	for (unsigned int i = 0; i < m_pPlayerIcons.size(); i++)
	{
		// プレイヤーアイコンの経過時間を更新
		m_pPlayerIcons[i]->SetTime(m_pPlayerIcons[i]->GetTime() + elapsedTime);
		// プレイヤーアイコンの位置を更新
		m_pPlayerIcons[i]->SetPosition(position);
	}
}
/*
*	@brief 描画
*	@details ここではプレイヤーのアイコンを描画する
*	@param なし
*	@return なし
*/
void Panel::Render()
{
	// プレイヤーアイコンの描画
	for (unsigned int i = 0; i < m_pPlayerIcons.size(); i++)m_pPlayerIcons[i]->Render();
}

/*
*	@brief タイル情報の描画
*	@details パネルに配置されたタイル情報を描画する
*	@param なし
*	@return なし
*/
void Panel::DrawTiles()
{
	// タイルの描画
	for (unsigned int i = 0; i < m_pTiles.size(); i++)m_pTiles[i]->Render();
}
/*
*	@brief アイテム情報の描画
*	@details パネルに配置されたアイテム情報を描画する
*	@param なし
*	@return なし
*/
void Panel::DrawItems()
{
	// アイテムの描画
	for (int i = 0; i < m_pItems.size(); i++)
	{
		// アイテムのデータが存在する場合のみ描画
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
			m_pItems[i].first->Render();
	}
}
/*
*	@brief アイテムの配置
*	@details パネルにアイテムを配置する（Initializeから独立)
*	@param itemData アイテムのデータ
*	@param row アイテムの行番号
*	@param col アイテムの列番号
*/
void Panel::PlaceItems(const MapItemData& itemData, int row, int col, const DirectX::SimpleMath::Vector2& pos)
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// アイテムがない場合はスキップ
	if (itemData.itemInfo.modelName.empty())return;
	// アイテムの位置を計算
	Vector2 itemPos = pos;
	// アイテムのテクスチャキーを取得
	std::string itemTextureKey = itemData.itemInfo.modelName;
	// 行と列を保存
	m_row = row;
	m_col = col;
	// アイテムを追加
	Add(itemTextureKey
		, itemPos
		, Vector2(TILE_POSITION_CORRECTION, TILE_POSITION_CORRECTION)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::ITEM);
}
/*
*	@brief プレイヤーの配置
*	@details 配置したタイルがスタート地点の時、パネルにプレイヤーを配置する（Initializeから独立)
*/
void Panel::PlacePlayer(const MapTileData& tileData, int row, int col, const DirectX::SimpleMath::Vector2& pos)
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// スタート地点のタイルかどうかを確認
	if (tileData.tileInfo.modelName != "StartBlock")return;
	// 行と列を保存
	m_row = row;
	m_col = col;
	// プレイヤーを追加
	Add("PlayerIcon"
		, pos
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::PLAYERICON);
}
/*
*	@brief パネルの追加
*	@details パネルにUI要素を追加する
*	@param key 画像のキー
*	@param position 位置
*	@param scale スケール
*	@param anchor アンカー
*	@param type UIの種類
*	@return なし
*/
void Panel::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	// UIオブジェクトの生成
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// 指定画像でUI作成
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UIの種類に応じて処理を分岐
	// 選択可能なアイテムなら
	if (type == UIType::TILE)m_pTiles.push_back(std::move(userInterface));
	// アイテムなら
	else if (type == UIType::ITEM)
	{
		// アイテム情報を設定
		ItemInfo itemInfo{};
		// 収集済みフラグを初期化
		itemInfo.isCollected = false;
		// 行番号を設定
		itemInfo.row = m_row;
		// 列番号を設定
		itemInfo.col = m_col;
		// アイテムのペアを定義
		std::pair<std::unique_ptr<Canvas>, ItemInfo> item;
		// UIオブジェクトをセット
		item.first = std::move(userInterface);
		// アイテム情報をセット
		item.second = itemInfo;
		// アイテムをパネルに追加
		m_pItems.push_back(std::move(item));
	}
	// プレイヤーアイコンなら
	else if (type == UIType::PLAYERICON)
	{
		// プレイヤーアイコンをパネルに追加
		m_pPlayerIcons.push_back(std::move(userInterface));
	}
	// 未知のUIタイプ
	else throw std::runtime_error("Unknown UIType in Panel::Add");
}
