#include <pch.h>
#include "OverWorldMap.h"
/*
*	@brief コンストラクタ
*	@details OverWorldMapクラスのコンストラクタ
*	@param なし
*/
OverWorldMap::OverWorldMap()
	: m_mapTiles()// マップタイルのコンテナ
{
	Initialize();
}
/*
*	@brief 初期化
*	@details OverWorldMapクラスの初期化
*	@param なし
*	@return なし
*/
void OverWorldMap::Initialize()
{
	const int SIZE = 2;
	// 手始めに3x3のマップタイルを生成してみる
	for (int row = 0; row < SIZE; ++row)
	{
		for (int col = 0; col < SIZE; ++col)
		{
			auto tile = std::make_unique<MapTile>();
			tile->SetFrame(36, 40); // マップチップの行数と列数を設定
			tile->SetChipNum(0); // マップチップの番号を設定
			tile->SetMapPosition(row, col); // マップ上の位置を設定
			m_mapTiles.push_back(std::move(tile)); // タイルをコンテナに追加
		}
	}
}

void OverWorldMap::Update(float elapsedTime)
{
	// 各マップタイルの更新
	for (auto& tile : m_mapTiles)
	{
		tile->Update(elapsedTime);
	}
}

void OverWorldMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// 各マップタイルの描画
	for (auto& tile : m_mapTiles)
	{
		tile->Render(view, projection);
	}
}

void OverWorldMap::Finalize()
{
	// 各マップタイルの終了処理
	for (auto& tile : m_mapTiles)
	{
		tile->Finalize();
	}
}
