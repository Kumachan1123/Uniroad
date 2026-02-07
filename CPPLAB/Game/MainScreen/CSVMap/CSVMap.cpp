/*
*	@file CSVMap.cpp
*	@brief CSV形式のマップを読み込み、描画するクラス
*/
#include <pch.h>
#include "CSVMap.h"
/*
*	@brief コンストラクタ
*	@details 生成時に共通リソースへのポインタを受け取り、初期化を行う。
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
CSVMap::CSVMap(CommonResources* resources)
	:m_pShadowMapLight(nullptr)// シャドウマップライトの初期化
	, m_pCommonResources(resources)// 共通リソースの初期化
{
	// マップのタイルの辞書を初期化
	InitializeTileDictionary();
}
/*
*	@brief デストラクタ
*	@details 生成時に確保したリソースを解放する。
*	@param なし
*	@return なし
*/
CSVMap::~CSVMap()
{
	// 共通リソースの解放は不要
	// タイルの辞書をクリア
	m_tileDictionary.clear();
	// タイルのレンダリングデータをクリア
	m_tiles.clear();
	// マップデータをクリア
	m_mapData.clear();
}
/*
*	@brief タイルの辞書を初期化する
*	@details タイルの種類とその情報を辞書に登録する。
*	@param なし
*	@return なし
*/
void CSVMap::InitializeTileDictionary()
{
	// タイルの種類とその情報を辞書に登録
	// 通常の床
	m_tileDictionary["b"] = TileInfo{ "Block" };
	// スタート地点
	m_tileDictionary["s"] = TileInfo{ "StartBlock" };
	// ゴール地点
	m_tileDictionary["g"] = TileInfo{ "GoalBlock" };
	// 直進（縦）
	m_tileDictionary["v"] = TileInfo{ "DefaultStraightVerticalBlock" };
	// 直進（横）
	m_tileDictionary["h"] = TileInfo{ "DefaultStraightHorizontalBlock" };
	// 十字
	m_tileDictionary["x"] = TileInfo{ "CrossBlock" };
	// 右下カーブ
	m_tileDictionary["rd"] = TileInfo{ "RightDownBlock" };
	// 左下カーブ
	m_tileDictionary["ld"] = TileInfo{ "LeftDownBlock" };
	// 右上カーブ
	m_tileDictionary["ru"] = TileInfo{ "RightUpBlock" };
	// 左上カーブ
	m_tileDictionary["lu"] = TileInfo{ "LeftUpBlock" };
	// 空白
	m_tileDictionary["0"] = TileInfo{ "" };
}
/*
*	@brief CSV形式のマップを読み込む
*	@details 指定されたファイルパスからCSV形式のマップデータを読み込み、タイルの情報を解析してマップを構築する。
*	@param filePath 読み込むCSVファイルのパス
*	@return なし
*/
void CSVMap::LoadMap(const std::string& filePath)
{
	// DirectXの名前空間を使用
	using namespace DirectX;
	// SimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// ファイルを開く
	std::ifstream file(filePath);
	// ファイルが開けなかった場合は何もしない
	if (!file.is_open()) return;
	// CSV読み込む前に2次元配列を確保する
	m_mapData.resize(MAXCOL);
	// 各行に列数確保
	for (int i = 0; i < MAXCOL; i++)m_mapData[i].resize(MAXRAW);
	// マップの初期化
	std::string line;
	// 行番号を初期化
	int row = 0;
	// マップ中心補正値計算
	float mapWidth = MAXRAW * 2.0f;
	float mapHeight = MAXCOL * 2.0f;
	// 中心補正値を計算
	float offsetX = mapWidth / 2.0f - 1.0f;
	float offsetZ = mapHeight / 2.0f - 1.0f;
	// マップの行と列を読み込む
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
			if (it != m_tileDictionary.end())
			{
				// タイル情報を取得
				const TileInfo& tileInfo = it->second;
				// タイルの位置計算（マップ中心補正）
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ワールド座標を計算
				Vector3 pos(x, 0.0f, z);
				// タイルを生成
				std::unique_ptr<TileBase> tileBase = TileFactory::CreateTileByName(tileInfo.modelName);
				// マップデータにタイル情報を保存
				m_mapData[row][col] = MapTileData{ tileInfo, pos,std::move(tileBase) };
				// ワールド行列を作成（スケーリングと位置の設定）
				Matrix world = Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos);
				// モデル取得
				DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(tileInfo.modelName);
				// タイルデータ保存
				m_tiles.push_back(TileRenderData{ model, world });
			}
			else
			{
				// タイルの位置計算（マップ中心補正）
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ワールド座標を計算
				Vector3 pos(x, 0.0f, z);
				// ワールド行列を作成（スケーリングと位置の設定）
				Matrix world = Matrix::CreateScale(Vector3::One) * Matrix::CreateTranslation(pos);
				// セルの文字列が辞書に存在しない場合は空のタイルを追加
				m_tiles.push_back(TileRenderData{ nullptr, Matrix::Identity });
				// デフォルトの床タイルを使用
				const TileInfo& emptyTileInfo = m_tileDictionary[""];
				// マップデータに空のタイル情報を保存
				m_mapData[row][col] = MapTileData{ emptyTileInfo, pos, nullptr };
			}
			// マップの列に値を設定
			++col;
		}
		// マップの行に値を設定
		++row;
	}
}
/*
*	@brief マップを描画する
*	@details マップのタイルを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void CSVMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通のステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// 全タイルを描画する
	for (const auto& tile : m_tiles)
	{
		// モデルが存在する場合のみ描画
		if (!tile.model)continue;
		// モデルの描画
		tile.model->Draw(context, *states, tile.world, view, proj, false, [&]
			{
				// 影を加味したライティング
				m_pShadowMapLight->ApplyShader(context, states);
			});
	}
}
/*
*	@brief 指定位置のタイル情報を取得する
*	@details 指定された列と行の位置にあるタイルの情報を取得する。
*	@param row 行番号
*	@param col 列番号
*	@return 指定位置のタイル情報への参照
*/
const  MapTileData& CSVMap::GetTileData(int row, int col) const
{
	// 範囲外の場合は場外のタイルデータを返す
	if (col < 0 || col >= MAXCOL || row < 0 || row >= MAXRAW)return OUT_OF_MAP_DATA;
	// 指定された位置のタイル情報を返す
	return m_mapData[row][col];
}
/*
*	@brief 指定座標のタイル情報を取得する
*	@details 指定されたワールド座標に最も近いタイルの情報を取得する。
*	@param pos ワールド座標
*	@return 指定座標のタイル情報への参照
*/
const MapTileData& CSVMap::GetTileData(const DirectX::SimpleMath::Vector3& pos) const
{
	// DirectXとSimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// 最小距離と対応するタイルのインデックスを初期化
	float minDistance = std::numeric_limits<float>::max();
	// 最も近いタイルの行と列を初期化
	int closestRow = -1;
	int closestCol = -1;
	// そもそもマップの外だったら空のデータを渡す
	if (pos.x < -MAXCOL || pos.x >= MAXRAW || pos.z < -MAXCOL || pos.z >= MAXCOL)return OUT_OF_MAP_DATA;
	// マップデータを走査して最も近いタイルを探す
	for (int row = 0; row < MAXCOL; ++row)
	{
		for (int col = 0; col < MAXRAW; ++col)
		{
			// 現在のタイルの情報を取得
			const MapTileData& tile = m_mapData[row][col];
			// タイルの位置との距離を計算
			float distance = (tile.pos - pos).LengthSquared();
			// 最小距離を更新
			if (distance < minDistance)
			{
				// 最小の距離を更新
				minDistance = distance;
				// 最も近いタイルの行と列を更新
				closestRow = row;
				closestCol = col;
			}
		}
	}
	// 最も近いタイルの情報を返す
	assert(closestRow >= 0 && closestRow < MAXCOL && closestCol >= 0 && closestCol < MAXRAW);
	// 指定座標のタイル情報を取得する
	return m_mapData[closestRow][closestCol];
}
/*
*	@brief 指定座標の行番号を取得する
*	@details 指定されたワールド座標に対応する行番号を計算して返す。
*	@param pos ワールド座標
*	@return 指定座標の行番号
*/
int CSVMap::GetRowFromPosition(const DirectX::SimpleMath::Vector3& pos) const
{
	// マップの行数を取得
	int rowCount = static_cast<int>(m_mapData.size());
	// 指定位置の行番号を計算
	int row = static_cast<int>((pos.z + MAXCOL) / 2.0f);
	// 行番号が範囲外の場合は-1を返す
	if (row < 0 || row >= rowCount)return -1;
	// 行番号を返す
	return row;
}
/*
*	@brief 指定座標の列番号を取得する
*	@details 指定されたワールド座標に対応する列番号を計算して返す。
*	@param pos ワールド座標
*	@return 指定座標の列番号
*/
int CSVMap::GetColFromPosition(const DirectX::SimpleMath::Vector3& pos) const
{
	// マップの列数を取得
	int colCount = static_cast<int>(m_mapData[0].size());
	// 指定位置の列番号を計算
	int col = static_cast<int>((pos.x + MAXRAW) / 2.0f);
	// 列番号が範囲外の場合は-1を返す
	if (col < 0 || col >= colCount)return -1;
	// 列番号を返す
	return col;
}
/*
*	@brief 指定した位置に指定したモデルを配置する
*	@details 指定された行と列の位置に、指定されたモデル名のタイルを配置する。
*	@param row 行番号
*	@param col 列番号
*	@param modelName モデル名
*	@return なし
*/
void CSVMap::SetTileModel(int row, int col, const std::string& modelName)
{
	//SimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// 範囲外チェック
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	// モデル名が空でない場合
	if (modelName.empty())return;
	// タイル情報を更新
	m_mapData[row][col].tileInfo.modelName = modelName;
	// モデルを取得
	DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(modelName);
	// タイルを生成
	std::unique_ptr<TileBase> tileBase = TileFactory::CreateTileByName(m_mapData[row][col].tileInfo.modelName);
	// タイルのポインターを設定
	m_mapData[row][col].tileBasePtr = std::move(tileBase);
	// タイルの位置を計算
	Vector3 pos = m_mapData[row][col].pos;
	// ワールド座標を計算
	Matrix world = Matrix::CreateScale(m_mapData[row][col].tileInfo.scale) * Matrix::CreateTranslation(pos);
	// タイルのレンダリングデータを更新
	m_tiles.push_back(TileRenderData{ model, world });
}
/*
*	@brief スタート地点を返す
*	@details modelnameがStartのタイルの位置を返す。
*	@param なし
*	@return スタート地点への参照
*/
const MapTileData& CSVMap::GetStart() const
{
	// マップデータを走査してスタート地点を探す
	for (const auto& row : m_mapData)
	{
		// 各行のタイルを走査
		for (const auto& tile : row)
		{
			// タイルのモデル名が"Start"の場合
			if (tile.tileInfo.modelName == "StartBlock")
			{
				// スタート地点の位置を返す
				return tile;
			}
		}
	}
	// スタート地点が見つからない場合は(0,0)の位置を返す
	return   m_mapData[0][0];
}