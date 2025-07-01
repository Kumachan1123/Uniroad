/*
*	@file NextTiles.cpp
*	@brief 次のタイルクラス
*/
#include <pch.h>
#include "NextTiles.h"
#include <random>
/*
*	@brief コンストラクタ
*	@details 次のタイルクラスのコンストラクタ
*	@param なし
*	@return なし
*/
NextTiles::NextTiles()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_viewPortControll() // ビューポートの制御
	, m_pDR(nullptr) // デバイスリソース
	, m_hit(false) // UIにヒットしたかどうか
	, m_time(0.0f) // 経過時間
	, m_windowHeight(0) // ウィンドウの高さ
	, m_windowWidth(0) // ウィンドウの幅
	, m_menuIndex(0) // 現在選択されているメニューのインデックス
{
}
/*
*	@brief デストラクタ
*	@details 次のタイルクラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
NextTiles::~NextTiles()
{
	// 何もしない
}
/*
*	@brief 初期化
*	@details 次のタイルクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void NextTiles::Initialize(CommonResources* resources, int width, int height)
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
	// UI追加
	Add("NextTilesBack"
		, Vector2(290.0f, 300.0f)
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// 使うタイルを宣言
	// 直進(横)
	m_tilesDictionary.push_back("StraightHorizontal");
	// 直進(縦)
	m_tilesDictionary.push_back("StraightVertical");
	// 十字
	m_tilesDictionary.push_back("Cross");
	// 右下カーブ
	m_tilesDictionary.push_back("RightDown");
	// 左下カーブ
	m_tilesDictionary.push_back("LeftDown");
	// 右上カーブ
	m_tilesDictionary.push_back("RightUp");
	// 左上カーブ
	m_tilesDictionary.push_back("LeftUp");
}
/*
*	@brief 更新
*	@details 次のタイルクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void NextTiles::Update(const float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	m_hit = false;
	m_menuIndex = -1;
	//// ウィンドウハンドルを取得
	//const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	//// ウィンドウサイズ取得
	//RECT rect;
	//// クライアント領域サイズを取得
	//GetClientRect(hwnd, &rect);
	//// ウィンドウの幅（ピクセル単位）
	//float renderWidth = static_cast<float>(rect.right);
	//// ウィンドウの高さ（ピクセル単位）
	//float renderHeight = static_cast<float>(rect.bottom);
	//// ビューポート設定 
	//D3D11_VIEWPORT viewportRight = {};
	//// ビューポートの左上X座標（画面幅の70%位置）
	//viewportRight.TopLeftX = renderWidth * 0.7f;
	//// ビューポートの左上Y座標（最上部）
	//viewportRight.TopLeftY = 0;
	//// ビューポートの幅（画面幅の30%）
	//viewportRight.Width = renderWidth * 0.3f;
	//// ビューポートの高さ（画面高さ全体）
	//viewportRight.Height = renderHeight;
	//// 最小深度
	//viewportRight.MinDepth = 0.0f;
	//// 最大深度
	//viewportRight.MaxDepth = 1.0f;
	//// ビューポート情報をメンバ変数に保存
	//m_viewPortControll = viewportRight;
	//// ビューポート左上X
	//float vp_left = m_viewPortControll.TopLeftX;
	//// ビューポート左上Y
	//float vp_top = m_viewPortControll.TopLeftY;
	//// ビューポート幅
	//float vp_width = m_viewPortControll.Width;
	//// ビューポート高さ
	//float vp_height = m_viewPortControll.Height;

	//// UIの論理解像度 
	//// 論理解像度の幅
	//constexpr float logicalWidth = 1920.0f;
	//// 論理解像度の高さ
	//constexpr float logicalHeight = 1080.0f;
	//// マウス座標を論理解像度基準にスケーリング
	//// 論理解像度基準のマウスX座標
	//float mouseX_UI = mouseState.x * (logicalWidth / renderWidth);
	//// 論理解像度基準のマウスY座標
	//float mouseY_UI = mouseState.y * (logicalHeight / renderHeight);
	//// ビューポートの論理解像度基準での座標・サイズを計算
	//// 論理解像度基準のビューポート左上X
	//float vp_left_UI = vp_left * (logicalWidth / renderWidth);
	//// 論理解像度基準のビューポート左上Y
	//float vp_top_UI = vp_top * (logicalHeight / renderHeight);
	//// 論理解像度基準のビューポート幅
	//float vp_width_UI = vp_width * (logicalWidth / renderWidth);
	//// 論理解像度基準のビューポート高さ
	//float vp_height_UI = vp_height * (logicalHeight / renderHeight);
	//// マウス座標をビューポート内ローカル座標に変換
	//Vector2 mousePos = Vector2(mouseX_UI - vp_left_UI, mouseY_UI - vp_top_UI);
	// UI要素ごとにヒット判定を行う
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスがビューポート外ならスキップ
		if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 || m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() || m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())
			continue;
		// ヒット判定（UI要素ごと）
		if (m_pUI[i]->IsHit(m_pMouse->GetPosition()) && mouseState.leftButton)
		{
			m_hit = true;         // 当たり判定フラグ
			m_menuIndex = i;      // 当たったUIのインデックス
			const auto debugString = m_pCommonResources->GetDebugString();
			debugString->AddString("hitNextTile:%i", m_menuIndex);
			break;
		}
	}
	// 経過時間を加算
	m_time += elapsedTime;
	// 10秒ごとにUIを追加する
	if (m_time >= 10.0f)
	{
		// UIを追加
		AddNextTiles();
		// 時間をリセット
		m_time = 0.0f;
	}
	// 全背景UIの経過時間を更新
	for (int i = 0; i < m_pBackUI.size(); i++)
	{
		// 背景UIの時間を更新
		m_pBackUI[i]->SetTime(m_pBackUI[i]->GetTime() + elapsedTime);
	}
	// 全UI要素の経過時間を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// UI要素の時間を更新
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// 選択中のUIがあるなら座標を変更する
	if (m_menuIndex >= 0 && mouseState.leftButton)m_pUI[m_menuIndex]->SetPosition(m_pMouse->GetPosition());
}
/*
*	@brief 描画
*	@details 次のタイルクラスの描画を行う
*	@param なし
*	@return なし
*/
void NextTiles::Render()
{
	// 背景UIの数だけ繰り返す
	for (unsigned int i = 0; i < m_pBackUI.size(); i++)
	{
		// 背景UIの描画
		m_pBackUI[i]->Render();
	}
	// UIの数だけ繰り返す
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// 選択可能UIの描画
		m_pUI[i]->Render();
	}
}
/*
*	@brief UI追加
*	@details 次のタイルクラスにUIを追加する
*	@param key テクスチャのキー
*	@param position 位置
*	@param scale スケール
*	@param anchor アンカー
*	@param type UIの種類
*	@return なし
*/
void NextTiles::Add(const std::string& key, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor, UIType type)
{
	// UIオブジェクトの生成
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// 指定画像でUI作成
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// アイテムを新しく追加		
	if (type == UIType::SELECT)m_pUI.push_back(std::move(userInterface));
	else m_pBackUI.push_back(std::move(userInterface));
}

/*
*	@brief 定期的にUIを追加する
*	@details 更新中に10秒に一個UIを追加する
*	@param なし
*	@return なし
*/
void NextTiles::AddNextTiles()
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// UIの数が5個以上なら追加しない
	if (m_pUI.size() == 5)return;
	// 乱数の設定
	std::random_device seed;
	// メルセンヌ・ツイスタ法
	std::default_random_engine engine(seed());
	// ランダムな範囲を設定
	std::uniform_int_distribution<int> rand(0, (int)m_tilesDictionary.size() - 1);
	// ランダムなインデックスを取得
	int randomIndex = rand(engine);
	// Y座標を調整
	float positionY = 480.0f - (float(m_pUI.size()) * 90.0f);
	// UI追加
	Add(m_tilesDictionary[randomIndex]
		, Vector2(290.0f, positionY)
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);

}

