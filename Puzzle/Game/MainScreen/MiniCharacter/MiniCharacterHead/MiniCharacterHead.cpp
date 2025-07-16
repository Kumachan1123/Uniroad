#include "pch.h"
#include "MiniCharacterHead.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBody/MiniCharacterBody.h"

MiniCharacterHead::MiniCharacterHead(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())
	, m_partNumber(MiniCharacter::GetPartsNumber())
	, m_partID(MiniCharacter::HEAD)
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

MiniCharacterHead::~MiniCharacterHead()
{
	Finalize();
}

void MiniCharacterHead::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	assert(commonResources);
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerHead");

}

void MiniCharacterHead::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition;
	// 現在の回転角を更新する
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
}

void MiniCharacterHead::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(1) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition);
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
}

void MiniCharacterHead::Finalize()
{
}
