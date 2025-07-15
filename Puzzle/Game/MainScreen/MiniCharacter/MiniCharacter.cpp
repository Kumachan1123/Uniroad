#include <pch.h>
#include "MiniCharacter.h"




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
	, m_enteredTilePtr(nullptr)
	, m_hasEnteredTile(false)
	, m_isMoving(true)
	, m_initialPosition(initialPosition)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition{}
	, m_currentVelocity{}
	, m_currentAngle{}
	, m_prevTileName("Start")
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
	Attach(std::make_unique<Sheep>(this, Vector3(0.0f, 3.5f, 0.0f), 0.0f));
}

void MiniCharacter::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ----- タイルによる進行方向の自動制御 -----
	std::string currentTileName = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName;
	bool isAtTileCenter = IsAtTileCenter(m_currentPosition, GetParent()->GetCSVMap()->GetTileData(m_currentPosition).pos);
	if (currentTileName != m_prevTileName)
	{
		// タイルを移動した瞬間
		const auto& prevTile = GetParent()->GetCSVMap()->GetTileData(m_prevPosition);
		// 前のタイルのOnExitを呼び出す
		if (prevTile.tileBasePtr) prevTile.tileBasePtr->OnExit(this);
		// 新しいタイルのOnEnterを呼び出す
		const auto& currentTile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
		if (currentTile.tileBasePtr ||
			(m_prevTileName == "" && currentTileName != ""))
		{
			currentTile.tileBasePtr->OnEnter(this);
			m_prevTileName = currentTileName;
			// 前の位置を更新
			m_prevPosition = m_currentPosition;
		}


	}
	if (isAtTileCenter)
	{
		const auto& currentTile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
		if (currentTile.tileBasePtr)
			currentTile.tileBasePtr->OnCenterReached(this);
	}
	else if (currentTileName == ""/* && isAtTileCenter*/)
	{
		// 移動を停止させて3秒後に落下させる
		//m_currentVelocity += Vector3(0.0f, -9.8f, 0.0f); // 重力を適用
		//m_currentVelocity = Vector3::Zero; // 落下中は移動しない
		m_isMoving = false;
		//m_initialPosition = m_currentPosition;// 落下後の位置を初期位置に設定
	}


	// 時間経過でプレイヤーを移動
	if (m_isMoving)m_MiniCharacterVelocity += m_currentVelocity * elapsedTime / 4; // 速度を設定

	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition + m_MiniCharacterVelocity;

	// 1. 目標回転を計算（速度ベクトルから）
	Quaternion targetQuat;
	if (m_currentVelocity.LengthSquared() > 0.0f)
	{
		float yaw = atan2f(m_currentVelocity.x, m_currentVelocity.z);
		targetQuat = Quaternion::CreateFromYawPitchRoll(yaw, 0.0f, 0.0f);
	}
	else
	{
		targetQuat = Quaternion::Identity;
	}

	// 2. 現在の回転から徐々に補間
	float rotateSpeed = 0.05f; // 0～1の範囲で補間速度（小さいほどゆっくり）
	m_rotationMiniCharacterAngle = Quaternion::Slerp(m_rotationMiniCharacterAngle, targetQuat, rotateSpeed);
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
	// 速度表示
	debugString->AddString("MiniCharacter Velocity: (%f, %f, %f)",
		m_currentVelocity.x, m_currentVelocity.y, m_currentVelocity.z);
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
/*
*	@brief タイルの中心にいるかどうかを判定する
*	@details プレイヤーの位置がタイルの中心に近いかどうかを判定する。
*	@param charPos プレイヤーの位置
*	@param tileCenter タイルの中心位置
*	@param epsilon 判定の許容誤差
*	@return タイルの中心にいる場合はtrue、そうでない場合はfalse
*/
bool MiniCharacter::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	// タイルの中心とプレイヤーの位置の距離を計算
	float distance = (charPos - tileCenter).Length();
	return distance < epsilon;
}

