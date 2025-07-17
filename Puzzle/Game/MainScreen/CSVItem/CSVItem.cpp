/*
*	@file CSVItem.cpp
*	@brief CSV形式のアイテム分布を読み込み、描画するクラス
*/
#include <pch.h>
#include "CSVItem.h"


/*
*	@brief コンストラクタ
*	@details 生成時に共通リソースへのポインタを受け取り、初期化を行う。
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
CSVItem::CSVItem(CommonResources* resources)
	: m_time(0.0f) // 経過時間
	, m_pCommonResources(resources) // 共通リソースへのポインタ
{
	// アイテムのタイルの辞書を初期化
	InitializeTileDictionary();
	// 当たり判定描画の初期化
	DrawCollision::Initialize(m_pCommonResources);
}
/*
*	@brief デストラクタ
*	@details 生成時に確保したリソースを解放する。
*	@param なし
*	@return なし
*/
CSVItem::~CSVItem()
{
	// 共通リソースの解放は不要
	// DrawCollisionの終了処理は不要
	// タイルの辞書をクリア
	m_tileDictionary.clear();
	// タイルのレンダリングデータをクリア
	m_tiles.clear();

}
/*
*	@brief タイルの辞書を初期化する
*	@details タイルの種類とその情報を辞書に登録する。
*	@param なし
*	@return なし
*/
void CSVItem::InitializeTileDictionary()
{
	// タイルの種類とその情報を辞書に登録
	// 空白
	m_tileDictionary["0"] = ItemInfo{ "", false };
	// メダル
	m_tileDictionary["m"] = ItemInfo{ "Medal", false };
	//// メダル2
	//m_tileDictionary["m2"] = TileInfo{ "Medal2", false };
	//// メダル3
	//m_tileDictionary["m3"] = TileInfo{ "Medal3", false };

}

/*
*	@brief CSV形式のアイテムを読み込む
*	@details 指定されたファイルパスからCSV形式のアイテムデータを読み込み、タイルの情報を解析してアイテムを構築する。
*	@param filePath 読み込むCSVファイルのパス
*	@return なし
*/
void CSVItem::LoadItem(const std::string& filePath)
{
	// DirectXとSimpleMathの名前空間を使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ファイルを開く
	std::ifstream file(filePath);
	// ファイルが開けなかった場合は何もしない
	if (!file.is_open()) return;
	// CSV読み込む前に2次元配列を確保する
	// 行数確保
	m_mapItemData.resize(MAXCOL);
	// 各行に列数確保
	for (int i = 0; i < MAXCOL; ++i)m_mapItemData[i].resize(MAXRAW);
	// アイテムの初期化
	std::string line;
	// 行番号を初期化
	int row = 0;
	// アイテム中心補正値計算
	float mapWidth = MAXRAW * 2.0f;
	float mapHeight = MAXCOL * 2.0f;
	// 中心補正値を計算
	float offsetX = mapWidth / 2.0f - 1.0f;
	float offsetZ = mapHeight / 2.0f - 1.0f;
	// アイテムの行と列を読み込む
	while (std::getline(file, line) && row < MAXCOL)
	{
		// 行の文字列をカンマで分割してセルごとに処理
		std::stringstream ss(line);
		// セルの文字列を格納する変数
		std::string cell;
		// 列番号を初期化
		int col = 0;
		// セルごとに処理
		while (std::getline(ss, cell, ',') && col < MAXRAW)
		{
			// セルの文字列が空の場合はスキップ
			auto it = m_tileDictionary.find(cell);
			// セルの文字列が辞書に存在する場合
			if (it != m_tileDictionary.end() && it->second.modelName != "")
			{
				// タイル情報を取得
				const ItemInfo& tileInfo = it->second;
				// タイルの位置計算（アイテム中心補正）
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ワールド座標を計算
				Vector3 pos(x, 3.0f, z);
				// タイルを生成
				std::unique_ptr<ItemBase> itemBase = ItemFactory::CreateItemByName(tileInfo.modelName);
				// アイテムを初期化
				itemBase->Initialize(m_pCommonResources, tileInfo);
				// アイテムの位置を設定
				itemBase->SetPosition(pos);
				// アイテムのワールド行列を設定
				itemBase->SetWorldMatrix(Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos));
				// アイテムのモデルを設定
				itemBase->SetModel(m_pCommonResources->GetModelManager()->GetModel(tileInfo.modelName));

				//// ワールド行列を作成（スケーリングと位置の設定）
				//Matrix world = Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos);
				//// モデル取得
				//DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(tileInfo.modelName);
				// アイテムデータにタイル情報を保存
				m_mapItemData[row][col] = MapItemData{ tileInfo, pos, std::move(itemBase) };
				//// タイルデータ保存
				//m_tiles.push_back(ItemRenderData{ model, world });

			}
			else
			{
				// タイルの位置計算（アイテム中心補正）
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ワールド座標を計算
				Vector3 pos(x, 0.0f, z);
				// ワールド行列を作成（スケーリングと位置の設定）
				Matrix world = Matrix::CreateScale(Vector3::One) * Matrix::CreateTranslation(pos);
				// セルの文字列が辞書に存在しない場合は空のタイルを追加
				m_tiles.push_back(ItemRenderData{ nullptr, Matrix::Identity });

				// デフォルトの床タイルを使用
				const ItemInfo& emptyItemInfo = m_tileDictionary[""];
				// アイテムデータに空のタイル情報を保存
				m_mapItemData[row][col] = MapItemData{ emptyItemInfo, pos };
			}
			// アイテムの列に値を設定
			++col;
		}
		// アイテムの行に値を設定
		++row;
	}
}
/*
*	@brief 更新処理
*	@details アイテムの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void CSVItem::Update(float elapsedTime)
{
	m_time += elapsedTime; // 経過時間を更新
	for (int col = 0; col < MAXCOL; ++col)
	{
		for (int row = 0; row < MAXRAW; ++row)
		{
			// アイテムの更新
			if (m_mapItemData[col][row].itemBasePtr)
			{
				m_mapItemData[col][row].itemBasePtr->Update(elapsedTime);
			}
		}
	}
}
/*
*	@brief 当たり判定を描画する
*	@details デバッグモードで当たり判定のボックスを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void CSVItem::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectXとSimpleMathの名前空間を使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	// 当たり判定の描画を開始
#ifdef _DEBUG
	//// 描画開始
	//DrawCollision::DrawStart(view, proj);
	//// 当たり判定のボックスを描画
	//for (int i = 0; i < m_wallBox.size(); i++)	DrawCollision::DrawBoundingBox(m_wallBox[i], Colors::Red);
	//// 描画終了
	//DrawCollision::DrawEnd();

#endif
}
/*
*	@brief アイテムを描画する
*	@details アイテムのタイルを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void CSVItem::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通のステートを取得
	auto states = m_pCommonResources->GetCommonStates();

	// ワールド行列を生成
	Matrix world = Matrix::Identity;
	// 回転させる
	world = Matrix::CreateRotationY(m_time) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));

	for (int col = 0; col < MAXCOL; ++col)
	{
		for (int row = 0; row < MAXRAW; ++row)
		{
			// アイテムの更新
			if (m_mapItemData[col][row].itemBasePtr)
			{
				m_mapItemData[col][row].itemBasePtr->Render(view, proj);
			}
		}
	}
	//// 全タイルを描画する
	//for (const auto& tile : m_tiles)
	//{
	//	// モデルが存在する場合のみ描画
	//	if (tile.model)
	//	{
	//		// モデルの描画
	//		tile.model->Draw(context, *states, world * tile.world, view, proj, false);
	//	}
	//}
	const auto debugString = m_pCommonResources->GetDebugString();
	//// 当たり判定の描画
	//DrawCollision(view, proj);
}


/*
*	@brief 指定位置のタイル情報を取得する
*	@details 指定された列と行の位置にあるタイルの情報を取得する。
*	@param row 行番号
*	@param col 列番号
*	@return 指定位置のタイル情報への参照
*/
const  MapItemData& CSVItem::GetTileData(int row, int col) const
{
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	return m_mapItemData[row][col];
}
/*
*	@brief 指定座標のタイル情報を取得する
*	@details 指定されたワールド座標に最も近いタイルの情報を取得する。
*	@param pos ワールド座標
*	@return 指定座標のタイル情報への参照
*/
const MapItemData& CSVItem::GetTileData(const DirectX::SimpleMath::Vector3& pos) const
{
	// DirectXとSimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// 最小距離と対応するタイルのインデックスを初期化
	float minDistance = std::numeric_limits<float>::max();
	int closestRow = -1;
	int closestCol = -1;



	// アイテムデータを走査して最も近いタイルを探す
	for (int row = 0; row < MAXCOL; ++row)
	{
		for (int col = 0; col < MAXRAW; ++col)
		{
			const MapItemData& tile = m_mapItemData[row][col];
			// タイルの位置との距離を計算
			float distance = (tile.pos - pos).LengthSquared();
			// 最小距離を更新
			if (distance < minDistance)
			{
				minDistance = distance;
				closestRow = row;
				closestCol = col;
			}
		}
	}
	// 最も近いタイルの情報を返す
	assert(closestRow >= 0 && closestRow < MAXCOL && closestCol >= 0 && closestCol < MAXRAW);

	return m_mapItemData[closestRow][closestCol];
}

