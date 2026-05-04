#include "pch.h"
#include "CollisionTile.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
CollisionTile::CollisionTile()
	: m_frameRows(36)
	, m_frameCols(40)
	, m_position(Vector3::Zero)
	, m_row(0)
	, m_col(0)
{
	Initialize();
}

void CollisionTile::Initialize()
{}

void CollisionTile::Update(float elapsedTime)
{}
void CollisionTile::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{}

void CollisionTile::Finalize()
{}
