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
	, m_pMouse(nullptr) // マウスへのポインタ
	, m_hit(false) // UIにヒットしたかどうか
	, m_time(0.0f) // 経過時間
	, m_windowHeight(0) // ウィンドウの高さ
	, m_windowWidth(0) // ウィンドウの幅
	, m_menuIndex(0) // 現在選択されているメニューのインデックス
	, m_mapSizeX(mapSizeX) // マップのサイズX
	, m_mapSizeY(mapSizeY) // マップのサイズY
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
				, UIType::SELECT);
		}
	}
}

void Panel::Update(const float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	//m_hit = false;
	m_pMouse->SetHit(false); // マウスのヒットフラグをリセット
	//m_menuIndex = -1;
	m_pMouse->SetHitPanelIndex(-1);
	// UI要素ごとにヒット判定を行う
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスがビューポート外ならスキップ
		if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 ||
			m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() ||
			m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())
			continue;
		// ヒット判定（UI要素ごと）
		if (m_pUI[i]->IsHit(m_pMouse->GetPosition()))
		{
			m_pMouse->SetHit(true); // マウスのヒットフラグをセット
			m_pMouse->SetHitPanelIndex(i);// 当たったパネルのインデックスをセット
			m_pMouse->SetPanelPosition(m_pUI[i]->GetPosition()); // 当たったパネルの位置をセット
			break;
		}
	}
	// 経過時間を加算
	m_time += elapsedTime;

	// 全UI要素の経過時間を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
}


void Panel::Render()
{
	// UIの数だけ繰り返す
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// 描画
		m_pUI[i]->Render();
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
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		// アイテムを新しく追加		
		m_pUI.push_back(std::move(userInterface));
	}
}