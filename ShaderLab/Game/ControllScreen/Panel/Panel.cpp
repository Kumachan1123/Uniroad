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
	, m_pCSVMap(nullptr) // CSVマップへのポインタ
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
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	m_hit = false;
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズ取得
	RECT rect;
	// クライアント領域サイズを取得
	GetClientRect(hwnd, &rect);
	// ウィンドウの幅（ピクセル単位）
	float renderWidth = static_cast<float>(rect.right);
	// ウィンドウの高さ（ピクセル単位）
	float renderHeight = static_cast<float>(rect.bottom);
	// ビューポート設定 
	D3D11_VIEWPORT viewportRight = {};
	// ビューポートの左上X座標（画面幅の70%位置）
	viewportRight.TopLeftX = renderWidth * 0.7f;
	// ビューポートの左上Y座標（最上部）
	viewportRight.TopLeftY = 0;
	// ビューポートの幅（画面幅の30%）
	viewportRight.Width = renderWidth * 0.3f;
	// ビューポートの高さ（画面高さ全体）
	viewportRight.Height = renderHeight;
	// 最小深度
	viewportRight.MinDepth = 0.0f;
	// 最大深度
	viewportRight.MaxDepth = 1.0f;
	// ビューポート情報をメンバ変数に保存
	m_viewPortControll = viewportRight;
	// ビューポート左上X
	float vp_left = m_viewPortControll.TopLeftX;
	// ビューポート左上Y
	float vp_top = m_viewPortControll.TopLeftY;
	// ビューポート幅
	float vp_width = m_viewPortControll.Width;
	// ビューポート高さ
	float vp_height = m_viewPortControll.Height;

	// UIの論理解像度 
	// 論理解像度の幅
	constexpr float logicalWidth = 1920.0f;
	// 論理解像度の高さ
	constexpr float logicalHeight = 1080.0f;
	// マウス座標を論理解像度基準にスケーリング
	// 論理解像度基準のマウスX座標
	float mouseX_UI = mouseState.x * (logicalWidth / renderWidth);
	// 論理解像度基準のマウスY座標
	float mouseY_UI = mouseState.y * (logicalHeight / renderHeight);
	// ビューポートの論理解像度基準での座標・サイズを計算
	// 論理解像度基準のビューポート左上X
	float vp_left_UI = vp_left * (logicalWidth / renderWidth);
	// 論理解像度基準のビューポート左上Y
	float vp_top_UI = vp_top * (logicalHeight / renderHeight);
	// 論理解像度基準のビューポート幅
	float vp_width_UI = vp_width * (logicalWidth / renderWidth);
	// 論理解像度基準のビューポート高さ
	float vp_height_UI = vp_height * (logicalHeight / renderHeight);
	// マウス座標をビューポート内ローカル座標に変換
	Vector2 mousePos = Vector2(mouseX_UI - vp_left_UI, mouseY_UI - vp_top_UI);
	// デバッグ表示
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("isInside: %s",
		(mouseX_UI >= vp_left_UI) && (mouseX_UI < vp_left_UI + vp_width_UI)
		&& (mouseY_UI >= vp_top_UI) && (mouseY_UI < vp_top_UI + vp_height_UI)
		? "true" : "false"); // マウスがビューポート内にあるか
	debugString->AddString("Inside ViewPort Mouse Position: (%f, %f)", mousePos.x, mousePos.y); // ビューポート内マウス座標

	// 6. UI要素ごとにヒット判定を行う
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスがビューポート外ならスキップ
		if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x >= vp_width_UI || mousePos.y >= vp_height_UI)
			continue;

		// ヒット判定（UI要素ごと）
		if (m_pUI[i]->IsHit(mousePos))
		{
			m_hit = true;         // 当たり判定フラグ
			m_menuIndex = i;      // 当たったUIのインデックス
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
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("hitPanel:%i", m_menuIndex);
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
