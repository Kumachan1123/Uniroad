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
	m_commonResources = resources;
	// マップのタイルの辞書を初期化
	InitializeTileDictionary();
	// 当たり判定描画の初期化
	DrawCollision::Initialize(m_commonResources);
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
	m_pModel = m_commonResources->GetModelManager()->GetModel("Block");
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
				// ワールド行列を作成（スケーリングと位置の設定）
				Matrix world = Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos);
				// モデル取得
				DirectX::Model* model = m_commonResources->GetModelManager()->GetModel(tileInfo.modelName);
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
					m_wallBox.push_back(box);
				}
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
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 当たり判定のボックスを描画
	for (int i = 0; i < m_wallBox.size(); i++)	DrawCollision::DrawBoundingBox(m_wallBox[i], Colors::Red);
	// 描画終了
	DrawCollision::DrawEnd();

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
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通のステートを取得
	auto states = m_commonResources->GetCommonStates();
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
	//// 当たり判定の描画
	//DrawCollision(view, proj);
}
