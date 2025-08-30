/*
*	@file NextTiles.cpp
*	@brief 次のタイルクラス
*/
#include <pch.h>
#include "NextTiles.h"

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
	, m_pCSVMap(nullptr) // CSVマップへのポインタ
	, m_hit(false) // UIにヒットしたかどうか
	, m_pMouse(nullptr) // マウスへのポインタ
	, m_time(0.0f) // 経過時間
	, m_windowHeight(0) // ウィンドウの高さ
	, m_windowWidth(0) // ウィンドウの幅
	, m_draggingIndex(-1) // ドラッグ中のUIインデックス
	, m_menuIndex(0) // 現在選択されているメニューのインデックス
	, m_initialPositions{} // 各選択可能UIの初期位置リスト
	, m_lastPlacedTileName("") // 最後に置いたタイルの名前
	, m_previousTileName("") // 前回生成したタイル名
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
	m_windowWidth = (int)(width * Display::RATIO_CONTROLL_SCREEN_WIDTH);
	// ウィンドウ高さ
	m_windowHeight = height;
	// UI追加
	Add("NextTilesBack",
		NEXT_TILES_BACK_POS,
		NEXT_TILES_BACK_SIZE,
		KumachiLib::ANCHOR::MIDDLE_CENTER,
		UIType::ITEM);
	// 使うタイルを宣言
	// 直進(横)
	m_tilesDictionary.push_back("StraightHorizontalBlock");
	// 直進(縦)
	m_tilesDictionary.push_back("StraightVerticalBlock");
	// 十字
	m_tilesDictionary.push_back("CrossBlock");
	// 右下カーブ
	m_tilesDictionary.push_back("RightDownBlock");
	// 左下カーブ
	m_tilesDictionary.push_back("LeftDownBlock");
	// 右上カーブ
	m_tilesDictionary.push_back("RightUpBlock");
	// 左上カーブ
	m_tilesDictionary.push_back("LeftUpBlock");
}
/*
*	@brief 更新
*	@details 次のタイルクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void NextTiles::Update(const float elapsedTime)
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスのヒットフラグをリセット
	m_pMouse->SetHitNewTile(false);
	// 当たっている新しく出てきたタイルの番号をリセット
	m_pMouse->SetHitNewTileIndex(TILE_NOT_SELECTED);
	// ドラッグ中は他UIのヒット判定をしない
	if (m_pMouse->IsMouseDrag() && m_draggingIndex >= 0)
	{
		// ドラッグ中の処理だけやる
		// 左ボタン押している間はドラッグ
		if (mouseState.leftButton)
		{
			// ドラッグ中のタイルの位置をマウスに合わせる
			m_pTile[m_draggingIndex].canvas->SetPosition(m_pMouse->GetPosition());
			// 当たった新しいタイルの位置をセット
			m_pMouse->SetNewTilePosition(m_pTile[m_draggingIndex].canvas->GetPosition());
		}
		// 左ボタン離した瞬間だけリセット
		else
		{
			// パネルに当たっていなければ元の位置に戻す
			if (m_pMouse->GetHitPanelIndex() == TILE_NOT_SELECTED)	ResetTilePosition();
			// パネルに当たっていればパネルに配置
			else AddToPanel();
			// ドラッグ終了
			m_pMouse->SetMouseDrag(false);
			// ドラッグ中のインデックスをリセット
			m_draggingIndex = TILE_NOT_SELECTED;
		}
	}
	// ドラッグしていない状態
	else
	{
		// ドラッグしていない状態のみヒット判定&ドラッグ開始
		for (int i = 0; i < m_pTile.size(); i++)
		{
			// 操作画面外なら次のループへ
			if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 ||
				m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() ||
				m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())	continue;
			// マウスとタイルの当たり判定
			if (m_pTile[i].canvas->IsHit(m_pMouse->GetPosition()))
			{
				// マウスのヒットフラグをセット
				m_pMouse->SetHitNewTile(true);
				// 当たった新しいタイルのインデックスをセット
				m_pMouse->SetHitNewTileIndex(i);
				// 当たった新しいタイルの位置をセット
				m_pMouse->SetNewTilePosition(m_pTile[i].canvas->GetPosition());
				// ドラッグ開始（ドラッグしていない時のみ）
				if (mouseState.leftButton && !m_pMouse->IsMouseDrag())
				{
					// ドラッグ中のインデックスをセット
					m_draggingIndex = i;
					// ドラッグ中フラグをセット
					m_pMouse->SetMouseDrag(true);
				}
				// ループを抜ける
				break;
			}
		}
	}
	// 経過時間を加算
	if (!m_pMouse->IsMouseDrag())m_time += elapsedTime;
	// 一定時間経過したら新しいタイルを追加
	if (m_time >= NEW_TILE_INTERVAL)
	{
		// UIを追加
		AddNextTiles();
		// 時間をリセット
		m_time = 0.0f;
	}
	// 全背景UIの経過時間を更新
	for (int i = 0; i < m_pBack.size(); i++)m_pBack[i].canvas->SetTime(m_pBack[i].canvas->GetTime() + elapsedTime);
	// 全タイルの経過時間を更新
	for (int i = 0; i < m_pTile.size(); i++)m_pTile[i].canvas->SetTime(m_pTile[i].canvas->GetTime() + elapsedTime);
	// 全設置済みタイルの経過時間を更新
	for (int i = 0; i < m_pPlacedTile.size(); i++)m_pPlacedTile[i].canvas->SetTime(m_pPlacedTile[i].canvas->GetTime() + elapsedTime);
	// 選択中のUIがあるなら座標を変更する
	if (m_pMouse->GetHitNewTileIndex() >= 0 && mouseState.leftButton)m_pTile[m_pMouse->GetHitNewTileIndex()].canvas->SetPosition(m_pMouse->GetPosition());
#ifdef _DEBUG
	// デバッグ用の文字列を取得
	const auto debugString = m_pCommonResources->GetDebugString();
	// デバッグ用の文字列を追加
	debugString->AddString("DragNextTile:%i", m_draggingIndex);
#endif
}
/*
*	@brief 描画
*	@details 次のタイルクラスの描画を行う
*	@param なし
*	@return なし
*/
void NextTiles::Render()
{
	// 背景の描画
	for (unsigned int i = 0; i < m_pBack.size(); i++) m_pBack[i].canvas->Render();
	// タイルの描画
	for (unsigned int i = 0; i < m_pTile.size(); i++)m_pTile[i].canvas->Render();
}
/*
*	@brief 設置済みタイルを描画
*	@details 設置済みタイルクラスのタイルを描画する
*	@param なし
*	@return なし
*/
void NextTiles::DrawPlacedTiles() const
{
	// 設置済みタイルの描画
	for (unsigned int i = 0; i < m_pPlacedTile.size(); i++)	m_pPlacedTile[i].canvas->Render();
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
	// タイル情報の一時保存変数
	TileInfo tileInfo;
	// UIオブジェクトの生成
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// 指定画像でUI作成
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// タイル情報を設定
	// キャンバス情報を保存
	tileInfo.canvas = std::move(userInterface);
	// テクスチャのキーを設定
	tileInfo.textureKey = key;
	// 選択可能ならタイルとして追加	
	if (type == UIType::TILE)m_pTile.push_back(std::move(tileInfo));
	// 選択不可なら背景として追加		
	else m_pBack.push_back(std::move(tileInfo));
}
/*
*	@brief 定期的にUIを追加する
*	@details 更新中に定期的に一個UIを追加する
*	@param なし
*	@return なし
*/
void NextTiles::AddNextTiles()
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 基準となるタイル
	std::string tileName;
	// 今ミニキャラがいるタイルを基準にする
	tileName = m_miniCharacterTileName;
	// 次に進めるタイルのリストを取得
	std::vector<std::string> availableTiles = GetAvailableNextTiles(tileName, m_miniCharacterVelocity);
	// 接続可能なタイルをランダムに選ぶ
	std::string selectedTile = GetRandomConnectableTile(availableTiles, m_previousTileName);
	// 位置を設定
	Vector2 position(NEW_TILE_POSITION.x, NEW_TILE_POSITION.y - (float(m_pTile.size()) * TILE_SIZE));
	// 5個たまってたら一番古いタイルを消す
	if (m_pTile.size() == MAX_TILE_STOCK)
	{
		// 先頭を削除し,eraseで前詰め
		m_pTile.erase(m_pTile.begin());
		// 初期位置も同様
		m_initialPositions.erase(m_initialPositions.begin());
	}
	// UI追加
	Add(selectedTile, position, NEXT_TILES_BACK_SIZE, KumachiLib::ANCHOR::MIDDLE_CENTER, UIType::TILE);
	// 初期位置を保存
	m_initialPositions.push_back(position);
	// 位置を詰め直す
	for (size_t i = 0; i < m_pTile.size(); i++)
	{
		// Y座標を調整
		float newY = NEW_TILE_POSITION.y - (float)i * TILE_SIZE;
		// 座標を再設定
		m_pTile[i].canvas->SetPosition(Vector2(position.x, newY));
		// 初期位置も更新
		m_initialPositions[i] = Vector2(position.x, newY);
	}
	// 前回生成したタイル名を更新
	m_previousTileName = selectedTile;
}
/*
*	@brief 新しいパネルを配置
*	@details 新しいパネルを配置する
*	@param なし
*	@return なし
*/
void NextTiles::AddToPanel()
{
	// 名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// タイルデータが空の場合、以下の処理をする
	if (m_pCSVMap->GetTileData(m_pMouse->GetHitPanelRowIndex(), m_pMouse->GetHitPanelColIndex()).tileInfo.modelName == "")
	{
		// ドラッグ中のタイルをパネルに配置
		m_pTile[m_draggingIndex].canvas->SetPosition(m_pMouse->GetPanelPosition());
		// 最後に置いたタイルを保存
		m_lastPlacedTileName = m_pTile[m_draggingIndex].textureKey;
		// パネルに新しいタイルを配置
		m_pCSVMap->SetTileModel(m_pMouse->GetHitPanelRowIndex(), m_pMouse->GetHitPanelColIndex(), m_pTile[m_draggingIndex].textureKey);
		// 配置したタイルは別の配列に移し、元の配列からは消す
		m_pPlacedTile.push_back(std::move(m_pTile[m_draggingIndex]));
		// 元の配列から消したらその場所はemptyになる
		m_pTile.erase(m_pTile.begin() + m_draggingIndex);
		// 設置音を鳴らす
		m_pCommonResources->GetAudioManager()->PlaySound("PutTile", m_pCommonResources->GetSettingManager()->GetSEVolume());
		// 配置可能タイルの座標を詰める
		for (unsigned int i = 0; i < m_pTile.size(); i++)
		{
			// Y座標を調整
			float positionY = NEW_TILE_POSITION.y - (float)i * TILE_SIZE;
			// タイルの位置を更新
			m_pTile[i].canvas->SetPosition(Vector2(m_pTile[i].canvas->GetPosition().x, positionY));
		}
	}
	// タイルが既に存在する場合は元の位置に戻す
	else ResetTilePosition();
}
/*
*	@brief 元の位置にタイルを戻す
*	@details 元の位置にタイルを戻す
*	@param なし
*	@return なし
*/
void NextTiles::ResetTilePosition()
{
	// ドラッグ中のタイルがある場合は元の位置に戻す
	m_pTile[m_draggingIndex].canvas->SetPosition(m_initialPositions[m_draggingIndex]);
}
/*
*	@brief 接続可能な次のタイルを取得
*	@details 現在のタイル名と速度から接続可能な次のタイルを取得する
*	@param currentTileName 現在のタイル名
*	@param velocity ミニキャラの速度ベクトル
*	@return 接続可能な次のタイルの名前のリスト
*/
std::vector<std::string> NextTiles::GetAvailableNextTiles(const std::string& currentTileName, const DirectX::SimpleMath::Vector3& velocity) const
{
	// 進行方向を取得
	Direction dir = GetDirectionFromVelocity(velocity);
	// 接続可能なタイルのリストを返す
	return TileConnectionTable::GetConnectableTiles(currentTileName, dir);
}
/*
*	@brief 速度から進行方向を取得
*	@details ミニキャラの速度ベクトルから進行方向を取得する
*	@param velocity ミニキャラの速度ベクトル
*	@return 進行方向
*/
Direction NextTiles::GetDirectionFromVelocity(const DirectX::SimpleMath::Vector3& velocity) const
{
	// 速度ベクトルのX, Z成分をチェックして進行方向を決定
	// Z成分が正なら上、
	if (velocity.z < 0) return Direction::UP;
	// Z成分が負なら下
	if (velocity.z > 0) return Direction::DOWN;
	// X成分が正なら右
	if (velocity.x > 0) return Direction::RIGHT;
	// X成分が負なら左
	if (velocity.x < 0) return Direction::LEFT;
	// 速度ゼロや斜めは例外処理
	return Direction::UP;
}
/*
*	@brief 接続可能なタイルをランダムに取得
*	@details 接続可能なタイルのリストからランダムに一つ選ぶ
*	@param availableTiles 接続可能なタイルのリスト
*	@param previousTileName 前回生成したタイル名
*	@return ランダムに選ばれた接続可能なタイルの名前
*/
std::string NextTiles::GetRandomConnectableTile(const std::vector<std::string>& availableTiles, const std::string& previousTileName) const
{
	// 乱数設定
	std::random_device seed;
	// 乱数エンジンを定義
	std::default_random_engine engine(seed());
	// 分布を設定
	std::uniform_int_distribution<int> rand(0, (int)availableTiles.size() - 1);
	// ランダムにインデックスを取得
	int randomIndex = rand(engine);
	// 選ばれたタイル名を取得
	std::string selected = availableTiles[randomIndex];
	// m_pTileの一番新しいタイル名を取得
	std::string currentTileName = m_pTile.empty() ? "" : m_pTile.back().textureKey;
	// もし前回と同じなら再帰呼び出し
	if (selected == previousTileName && selected == currentTileName)return GetRandomConnectableTile(availableTiles, previousTileName);
	// 選ばれたタイルを返す
	return selected;
}

