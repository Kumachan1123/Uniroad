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

	m_initialPosition = GetParent()->GetCSVMap()->GetStartPosition();

	Attach(std::make_unique<MiniCharacterBody>(this, Vector3(0.0f, 1.8f, 0.0f), 0.0f));
}

void MiniCharacter::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 時間経過でプレイヤーを-Z方向に移動
	m_MiniCharacterVelocity += Vector3(0.0f, 0.0f, 1.0f) * elapsedTime * 1.0f; // 速度を設定

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
}

void MiniCharacter::Finalize()
{
}


//void MiniCharacter::LoadModel()
//{
//	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
//
//	// モデルを読み込む準備
//	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
//	fx->SetDirectory(L"Resources/models/PMiniCharacter");
//	//テクスチャパスを設定する
//	wcscpy_s(m_modelPath, L"Resources/models/PMiniCharacter/MiniCharacter_Body.cmo");
//
//	// モデルを読み込む
//	m_pModel = Model::CreateFromCMO(device, m_modelPath, *fx);
//	//m_face = Model::CreateFromCMO(device, L"Resources/models/enemy/Boss_Face_Attack.cmo", *fx);
//}
