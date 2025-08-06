/*
*	@file Panel.cpp
*	@brief パネルクラス
*/
#include <pch.h>
#include "Panel.h"
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
	// 一枚当たりの幅
	const float tileSize = 90.0f;
	// 左端の位置
	const float startX = Screen::CENTER_X - (tileSize * 2);
	// 上端の位置
	const float startY = Screen::CENTER_Y - (tileSize * 2);
	// グリッドのキーを決定するためのループ
	for (int row = 0; row < 5; ++row)
	{
		for (int col = 0; col < 5; ++col)
		{
			// CSVマップのタイル情報を取得
			const MapTileData& tileData = m_pCSVMap->GetTileData(row, col);
			// グリッドのキーを決定
			std::string textureKey = ((row + col) % 2 == 0) ? "GridA" : "GridB";
			// タイルの種類に応じてテクスチャキーを変更
			textureKey = tileData.tileInfo.modelName.empty() ? textureKey : tileData.tileInfo.modelName;
			// 位置計算
			float posX = startX + col * tileSize - 350.0f;
			float posY = startY + row * tileSize + 420.0f;

			// タイルの種類に応じてテクスチャキーを変更して並べる
			Add(textureKey
				, Vector2(posX, posY)
				, Vector2(0.6f, 0.6f)
				, KumachiLib::ANCHOR::MIDDLE_CENTER
				, UIType::TILE);
			// アイテムを配置する
			const MapItemData& itemData = m_pCSVItem->GetItemData(row, col);
			// アイテムがない場合はスキップ
			if (itemData.itemInfo.modelName.empty())continue;
			// アイテムの位置を計算
			Vector2 itemPos = Vector2(posX, posY);
			// アイテムのテクスチャキーを取得
			std::string itemTextureKey = itemData.itemInfo.modelName;
			// 行と列を保存
			m_row = row;
			m_col = col;
			// アイテムを追加
			Add(itemTextureKey
				, itemPos
				, Vector2(0.6f, 0.6f)
				, KumachiLib::ANCHOR::MIDDLE_CENTER
				, UIType::ITEM);

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
	for (int i = 0; i < m_pTiles.size(); i++)
	{
		m_pTiles[i]->SetTime(m_pTiles[i]->GetTime() + elapsedTime);

	}
	for (int i = 0; i < m_pItems.size(); i++)
	{
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
			m_pItems[i].first->SetTime(m_pItems[i].first->GetTime() + elapsedTime);
	}
}
/*
*	@brief 描画
*	@details 継承したことで生まれた空の関数
*	@param なし
*	@return なし
*/
void Panel::Render()
{
	// ここでは何もしない
	return;
}

/*
*	@brief タイル情報の描画
*	@details パネルに配置されたタイル情報を描画する
*	@param なし
*	@return なし
*/
void Panel::DrawTiles()
{
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
	for (int i = 0; i < m_pItems.size(); i++)
	{
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
		{
			m_pItems[i].first->Render();
		}
	}
}


void Panel::Add(const std::string& key, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor, UIType type)
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
		itemInfo.isCollected = false; // 収集済みフラグを初期化
		itemInfo.row = m_row; // 行番号を設定
		itemInfo.col = m_col; // 列番号を設定
		std::pair<std::unique_ptr<Canvas>, ItemInfo> item;
		item.first = std::move(userInterface); // UIオブジェクトをセット
		item.second = itemInfo; // アイテム情報をセット
		m_pItems.push_back(std::move(item));
	}
	// 未知のUIタイプ
	else throw std::runtime_error("Unknown UIType in Panel::Add");
}
