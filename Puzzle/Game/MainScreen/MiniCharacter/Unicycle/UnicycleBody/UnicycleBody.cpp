#include "pch.h"
#include "UnicycleBody.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleWheel/UnicycleWheel.h"

UnicycleBody::UnicycleBody(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())
	, m_partNumber(MiniCharacter::GetPartsNumber())
	, m_partID(MiniCharacter::UNICYCLE_BODY)
	, m_pCommonResources(nullptr)
	, m_pModel(nullptr)
	, m_initialPosition(initialPosition)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition{}
	, m_currentAngle{}
	, m_rotationMiniCharacterAngle{}
	, m_mass{}
	, m_MiniCharacterVelocity{}
{
	//m_pMiniCharacter = dynamic_cast<MiniCharacter*>(parent->GetParent()->GetParent());
}

UnicycleBody::~UnicycleBody()
{
	Finalize();
}

void UnicycleBody::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	assert(commonResources);
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Body");
	Attach(std::make_unique<UnicycleWheel>(this, DirectX::SimpleMath::Vector3(0.0f, -0.85f, 0.0f), 0.0f));// 頭部を追加
}

void UnicycleBody::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition;
	// 現在の回転角を更新する
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// 「胴体」部品を更新する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		// 部品を更新する
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}

void UnicycleBody::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(1) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition);
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
	for (auto& part : m_pMiniCharacterParts)
	{
		part->Render(view, proj);
	}
}

void UnicycleBody::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	MiniCharacterPart->Initialize(m_pCommonResources);
	m_pMiniCharacterParts.emplace_back(std::move(MiniCharacterPart));
}

void UnicycleBody::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
}

void UnicycleBody::Finalize()
{
}
