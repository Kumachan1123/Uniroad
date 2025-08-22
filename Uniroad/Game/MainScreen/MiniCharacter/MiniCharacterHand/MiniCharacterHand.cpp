#include "pch.h"
#include "MiniCharacterHand.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBody/MiniCharacterBody.h"
#include "KumachiLib/Easing/Easing.h"

MiniCharacterHand::MiniCharacterHand(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())
	, m_partNumber(MiniCharacter::GetPartsNumber())
	, m_partID(MiniCharacter::HEAD)
	, m_pCommonResources(nullptr)
	, m_pModel(nullptr)
	, m_time(0.0f)
	, m_initialPosition(initialPosition)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition{}
	, m_currentAngle{}
	, m_rotationMiniCharacterAngle{}
	, m_mass{}
	, m_MiniCharacterVelocity{}
{
	//m_pMiniCharacter = dynamic_cast<MiniCharacterSelectStage*>(parent->GetParent()->GetParent());
}

MiniCharacterHand::~MiniCharacterHand()
{
	Finalize();
}

void MiniCharacterHand::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	assert(commonResources);
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerHand");
}

void MiniCharacterHand::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	m_time += elapsedTime;
	// 本体の回転で手のオフセットを回す
	DirectX::SimpleMath::Vector3 handOffsetRotated = DirectX::SimpleMath::Vector3::Transform(m_initialPosition, currentAngle);
	m_currentPosition = currentPosition + handOffsetRotated;

	// Y座標アニメーション
	m_currentPosition.y += Easing::Reflect(Easing::EaseInSine, m_time) / 8;

	// 回転角の更新
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
}

void MiniCharacterHand::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();

	m_worldMatrix =
		Matrix::CreateScale(1) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition) *
		Matrix::CreateFromQuaternion(m_pParent->GetParent()->GetParent()->GetAngle()) *
		Matrix::CreateTranslation(m_pParent->GetParent()->GetParent()->GetPosition());
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);

}

void MiniCharacterHand::Finalize()
{
}
