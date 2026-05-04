#pragma once
#ifndef COLLISION_TILE_H
#define COLLISION_TILE_H
#include <Game\GameObject\IGameObject.h>
#include <Game\MyResources\MyResources.h>

class CollisionTile : public IGameObject
{
public:
// アクセサ
// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 位置をセット
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// マップチップの行数と列数をセット
	void SetFrame(int rows, int cols) { m_frameRows = rows; m_frameCols = cols; }
	// マップ上での行と列を取得
	int GetRow() const { return m_row; }
	int GetCol() const { return m_col; }

	// マップ上での位置をセット
	void SetMapPosition(int row, int col) { m_row = row; m_col = col; }

	// 当たり判定があるかを取得
	bool HasCollision() const { return m_hasCollision; }
	// 当たり判定があるかをセット
	void SetHasCollision(bool hasCollision) { m_hasCollision = hasCollision; }


public:
	CollisionTile();
	~CollisionTile() = default;
	void Initialize()override;
	void Update(float elapsedTime)override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;
	void Finalize()override;

private:
	// マップチップの行数と列数
	int m_frameRows;//行
	int m_frameCols;//列
	// タイル一枚の座標
	DirectX::SimpleMath::Vector3 m_position;
	// マップのタイルの位置（行と列）
	int m_row;// 行
	int m_col;// 列
	// 当たり判定があるか
	bool m_hasCollision;
};
#endif // !COLLISION_TILE_H