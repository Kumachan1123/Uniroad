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
	, m_pMouse(nullptr) // マウスへのポインタ
	, m_time(7.0f) // 経過時間
	, m_windowHeight(0) // ウィンドウの高さ
	, m_windowWidth(0) // ウィンドウの幅
	, m_draggingIndex(-1) // ドラッグ中のUIインデックス
	, m_menuIndex(0) // 現在選択されているメニューのインデックス
	, m_initialPositions{} // 各選択可能UIの初期位置リスト
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
	const auto debugString = m_pCommonResources->GetDebugString();
	m_pMouse->SetHitNewTile(false); // マウスのヒットフラグをリセット
	m_pMouse->SetHitNewTileIndex(-1);// 当たっている新しく出てきたタイルの番号をリセット
	// --- ドラッグ中は他UIのヒット判定をしない ---
	if (m_pMouse->IsMouseDrag() && m_draggingIndex >= 0)
	{
		// ドラッグ中の処理だけやる
		if (mouseState.leftButton)
		{
			m_pUI[m_draggingIndex]->SetPosition(m_pMouse->GetPosition());
			m_pMouse->SetNewTilePosition(m_pUI[m_draggingIndex]->GetPosition()); // 当たった新しいタイルの位置をセット
		}
		else// 左ボタン離した瞬間だけリセット
		{
			// パネルに当たっていなければ元の位置に戻す
			if (m_pMouse->GetHitPanelIndex() == -1)	ResetTilePosition();
			else AddToPanel();
			m_pMouse->SetMouseDrag(false);
			m_draggingIndex = -1;
		}
	}
	else
	{
		// ドラッグしていない状態のみヒット判定&ドラッグ開始
		for (int i = 0; i < m_pUI.size(); i++)
		{
			if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 ||
				m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() ||
				m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())
				continue;

			if (m_pUI[i]->IsHit(m_pMouse->GetPosition()))
			{
				m_pMouse->SetHitNewTile(true); // マウスのヒットフラグをセット
				m_pMouse->SetHitNewTileIndex(i); // 当たった新しいタイルのインデックスをセット
				m_pMouse->SetNewTilePosition(m_pUI[i]->GetPosition()); // 当たった新しいタイルの位置をセット


				// ドラッグ開始（ドラッグしていない時のみ）
				if (mouseState.leftButton && !m_pMouse->IsMouseDrag())
				{
					m_draggingIndex = i;
					m_pMouse->SetMouseDrag(true);
				}
				break;
			}
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
	if (m_pMouse->GetHitNewTileIndex() >= 0 && mouseState.leftButton)m_pUI[m_pMouse->GetHitNewTileIndex()]->SetPosition(m_pMouse->GetPosition());

	debugString->AddString("DragNextTile:%i", m_draggingIndex);
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
	// X座標は固定
	const float positionX = 290.0f;
	// 位置を設定
	Vector2 position(positionX, positionY);
	// UI追加
	Add(m_tilesDictionary[randomIndex]
		, position
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 初期位置を保存
	m_initialPositions.push_back(position);

}
/*
*	@brief 新しいパネルを配置
*	@details 新しいパネルを配置する
*	@param なし
*	@return なし
*/
void NextTiles::AddToPanel()
{
	m_pUI[m_draggingIndex]->SetPosition(m_pMouse->GetPanelPosition());

}
/*
*	@brief 元の位置にタイルを戻す
*	@details 元の位置にタイルを戻す
*	@param なし
*	@return なし
*/
void NextTiles::ResetTilePosition()
{
	m_pUI[m_draggingIndex]->SetPosition(m_initialPositions[m_draggingIndex]);
}

