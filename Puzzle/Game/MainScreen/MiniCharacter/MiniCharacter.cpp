#include <pch.h>
#include "MiniCharacter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


// 砲塔カウンター
int MiniCharacter::s_nodeCount = 0;
// 部品カウンター
int MiniCharacter::s_partsNumber = 0;

MiniCharacter::MiniCharacter(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_parent(parent)
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())
	, m_partNumber(MiniCharacter::GetPartsNumber())
	, m_partID(MiniCharacter::MINICHARACTER)
	, m_pCommonResources(nullptr)
	, m_initialPosition(initialPosition)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition{}
	, m_currentVelocity{}
	, m_currentAngle{}
	, m_rotationMiniCharacterAngle{}
	, m_mass{}
	, m_MiniCharacterVelocity{}

{
}

MiniCharacter::~MiniCharacter()
{
	Finalize();
}

void MiniCharacter::Initialize(CommonResources* resources)
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// 共通リソースが存在することを確認する
	assert(resources);
	// 共通リソースを設定する
	m_pCommonResources = resources;

	m_initialPosition = GetParent()->GetCSVMap()->GetStart().pos;
	UpdateSpeedByStartTile();
	Attach(std::make_unique<MiniCharacterBody>(this, Vector3(0.0f, 1.8f, 0.0f), 0.0f));
}

void MiniCharacter::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 時間経過でプレイヤーを-Z方向に移動
	m_MiniCharacterVelocity += m_currentVelocity * elapsedTime / 4; // 速度を設定

	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition + m_MiniCharacterVelocity;
	// 現在の回転角を更新する
	m_currentAngle = currentAngle * m_initialAngle * m_rotationMiniCharacterAngle;

	// 砲塔部品を更新する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		// 砲塔部品を更新する
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}

void MiniCharacter::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	MiniCharacterPart->Initialize(m_pCommonResources);
	m_pMiniCharacterParts.push_back(std::move(MiniCharacterPart));
}

void MiniCharacter::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
}

void MiniCharacter::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 砲塔部品を描画する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		MiniCharacterPart->Render(view, proj);
	}
	// ---デバッグ表示---
	const auto debugString = m_pCommonResources->GetDebugString();
	// 座標表示
	debugString->AddString("MiniCharacter Position: (%f, %f, %f)",
		m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
	// 最も近いタイルの名前を表示
	debugString->AddString("MiniCharacter Tile: %s",
		GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName.c_str());
}

void MiniCharacter::Finalize()
{
}

/*
*	@brief プレイヤーの開始タイルによって速度を更新する
*	@details スタート地点の前後左右のタイルを調べて、プレイヤーの速度を更新する。
*	@param なし
*	@return なし
*/
void MiniCharacter::UpdateSpeedByStartTile()
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// 行初期値
	int startRow = -1;
	// 列初期値
	int startCol = -1;

	// スタート位置を探す
	for (int row = 0; row < GetParent()->GetCSVMap()->GetMaxRow(); ++row)
	{
		for (int col = 0; col < GetParent()->GetCSVMap()->GetMaxCol(); ++col)
		{
			const auto& tile = GetParent()->GetCSVMap()->GetTileData(row, col);
			if (tile.tileInfo.modelName == "Start") {
				startRow = row;
				startCol = col;
				break;
			}
		}
		if (startRow != -1) break;
	}
	// 上下左右の方向情報
	struct Dir { int dRow, dCol; };
	const Dir dirs[4] =
	{
		{ -1,  0 }, // 上
		{  1,  0 }, // 下
		{  0, -1 }, // 左
		{  0,  1 }, // 右
	};
	// 対応する速度ベクトル
	const Vector3 velocities[4] =
	{
		{0, 0, -1}, // 上（前進）
		{0, 0,  1}, // 下（後進）
		{-1, 0, 0}, // 左
		{ 1, 0, 0}, // 右
	};

	for (int i = 0; i < 4; ++i)
	{
		int nRow = startRow + dirs[i].dRow;
		int nCol = startCol + dirs[i].dCol;
		if (nRow >= 0 && nRow < GetParent()->GetCSVMap()->GetMaxCol() &&
			nCol >= 0 && nCol < GetParent()->GetCSVMap()->GetMaxRow())
		{
			const auto& neighbor = GetParent()->GetCSVMap()->GetTileData(nRow, nCol);
			const std::string& model = neighbor.tileInfo.modelName;

			if ((i <= 1 && model == "DefaultStraightVertical") || // 上下
				(i >= 2 && model == "DefaultStraightHorizontal")) // 左右
			{
				m_currentVelocity = velocities[i];
				break;
			}
		}
	}
}

