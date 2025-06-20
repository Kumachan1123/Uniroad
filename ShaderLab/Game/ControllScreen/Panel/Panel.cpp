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
	, m_pDR(nullptr) // デバイスリソース
	, m_hit(false) // UIにヒットしたかどうか
	, m_time(0.0f) // 経過時間
	, m_windowHeight(0) // ウィンドウの高さ
	, m_windowWidth(0) // ウィンドウの幅
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
	m_windowWidth = width * .3f;
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
			const CSVMap::MapTileData& tileData = m_pCSVMap->GetTileData(row, col);
			// グリッドのキーを決定
			std::string textureKey = ((row + col) % 2 == 0) ? "GridA" : "GridB";
			// タイルの種類に応じてテクスチャキーを変更
			textureKey = tileData.tileInfo.modelName.empty() ? textureKey : tileData.tileInfo.modelName;
			// 位置計算
			float posX = startX + col * tileSize - 350.0f;
			float posY = startY + row * tileSize + 420.0f;
			// タイルを並べる
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
	// マウストラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// マウスの現在の状態を取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 未選択状態にする
	m_hit = false;
	// マウス座標を2Dベクトルで取得する
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// UIの各要素に対してヒット判定を行う
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスの座標がUIに重なっているか確認する
		if (m_pUI[i]->IsHit(mousePos))
		{
			// ヒットフラグを立てる
			m_hit = true;
			//// 選択メニューが前回と違う場合、SE再生フラグをリセット
			//if ((int(m_menuIndex)) != i)m_isSEPlay = false;
			//// SEが未再生なら
			//if (!m_isSEPlay)
			//{
			//	// 効果音を再生する
			//	m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
			//	// SE再生フラグを立てる
			//	m_isSEPlay = true;
			//}
			//// 現在選択されているメニューのインデックスを更新する
			//m_menuIndex = i;
			// 1つでもヒットしたら他は無視する（選択は1つだけ）
			break;
		}
	}
	// 経過時間を加算する
	m_time += elapsedTime;
	// UI要素の状態を更新する 
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// 経過時間を渡してアニメ進行
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
