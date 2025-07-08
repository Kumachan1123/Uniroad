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
{
	// 共通リソースへのポインタを保存
	m_pCommonResources = resources;
	// マップのタイルの辞書を初期化
	InitializeTileDictionary();
	// 当たり判定描画の初期化
	DrawCollision::Initialize(m_pCommonResources);
	// マップの読み込み
	LoadModel();
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
void CSVMap::InitializeTileDictionary()
{
	// タイルの種類とその情報を辞書に登録
	// 通常の床
	m_tileDictionary["w"] = TileInfo{ "Block", true };
	// スタート地点
	m_tileDictionary["s"] = TileInfo{ "Start", true };
	// ゴール地点
	m_tileDictionary["g"] = TileInfo{ "Goal", true };
	// 直進（縦）
	m_tileDictionary["v"] = TileInfo{ "DefaultStraightVertical", true };
	// 直進（横）
	m_tileDictionary["h"] = TileInfo{ "DefaultStraightHorizontal", true };
	// 十字
	m_tileDictionary["x"] = TileInfo{ "Cross", true };
	// 右下カーブ
	m_tileDictionary["rd"] = TileInfo{ "RightDown", true };
	// 左下カーブ
	m_tileDictionary["ld"] = TileInfo{ "LeftDown", true };
	// 右上カーブ
	m_tileDictionary["ru"] = TileInfo{ "RightUp", true };
	// 左上カーブ
	m_tileDictionary["lu"] = TileInfo{ "LeftUp", true };
	// 空白
	m_tileDictionary["0"] = TileInfo{ "", false };


}
/*
*	@brief モデルを読み込む
*	@details モデルマネージャーから必要なモデルを取得する。
*	@param なし
*	@return なし
*/
void CSVMap::LoadModel()
{
	// モデルを読み込む
	//m_pModel = m_pCommonResources->GetModelManager()->GetModel("Block");
}
/*
*	@brief CSV形式のマップを読み込む
*	@details 指定されたファイルパスからCSV形式のマップデータを読み込み、タイルの情報を解析してマップを構築する。
*	@param filePath 読み込むCSVファイルのパス
*	@return なし
*/
void CSVMap::LoadMap(const std::string& filePath)
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
	m_mapData.resize(MAXCOL);
	// 各行に列数確保
	for (int i = 0; i < MAXCOL; ++i)m_mapData[i].resize(MAXRAW);
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
				// マップデータにタイル情報を保存
				m_mapData[row][col] = MapTileData{ tileInfo, pos, tileInfo.hasCollision };
				// ワールド行列を作成（スケーリングと位置の設定）
				Matrix world = Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos);
				// モデル取得
				DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(tileInfo.modelName);
				// タイルデータ保存
				m_tiles.push_back(TileRenderData{ model, world });
				// 当たり判定
				if (tileInfo.hasCollision)
				{
					// 当たり判定用のボックスを作成
					BoundingBox box;
					// ボックスの中心と拡大率を設定
					box.Center = pos;
					// 拡大率を設定
					box.Extents = tileInfo.scale;
					// ボックスの拡大率を2倍にする（当たり判定用）
					//m_wallBox.push_back(box);
				}
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
				m_mapData[row][col] = MapTileData{ emptyTileInfo, pos, false };
			}
			// マップの列に値を設定
			++col;
		}
		// マップの行に値を設定
		++row;
	}
}
/*
*	@brief 当たり判定を描画する
*	@details デバッグモードで当たり判定のボックスを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void CSVMap::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
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
		if (tile.model)
		{
			// モデルの描画
			tile.model->Draw(context, *states, tile.world, view, proj, false);
		}
	}
	const auto debugString = m_pCommonResources->GetDebugString();
	for (int i = 0; i < 5; ++i)
		debugString->AddString("MapPos%f,%f", GetTileData(i, 4).pos.x, GetTileData(i, 4).pos.z);
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
const  MapTileData& CSVMap::GetTileData(int row, int col) const
{
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	return m_mapData[row][col];
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
	// DirectXとSimpleMathの名前空間を使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	// モデル名が空でない場合
	if (!modelName.empty())
	{
		// タイル情報を更新
		m_mapData[row][col].tileInfo.modelName = modelName;
		// モデルを取得
		DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(modelName);
		// タイルの位置を計算
		Vector3 pos = m_mapData[row][col].pos;
		// ワールド座標を計算
		Matrix world = Matrix::CreateScale(m_mapData[row][col].tileInfo.scale) * Matrix::CreateTranslation(pos);
		// タイルのレンダリングデータを更新
		m_tiles.push_back(TileRenderData{ model, world });
	}
	else
	{
		// モデル名が空の場合は何もしない
	}
}
/*
*	@brief スタート地点を返す
*	@details modelnameがStartのタイルの位置を返す。
*	@param なし
*	@return スタート地点の位置への参照
*/
const DirectX::SimpleMath::Vector3& CSVMap::GetStartPosition() const
{
	// DirectXとSimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// マップデータを走査してスタート地点を探す
	for (const auto& row : m_mapData)
	{
		// 各行のタイルを走査
		for (const auto& tile : row)
		{
			// タイルのモデル名が"Start"の場合
			if (tile.tileInfo.modelName == "Start")
			{
				// スタート地点の位置を返す
				return tile.pos;
			}
		}
	}
	// スタート地点が見つからない場合はゼロベクトルを返す
	return Vector3::Zero;
}
