#include "pch.h"
#include "MiniCharacterBody.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
#include "KumachiLib/Easing/Easing.h"

MiniCharacterBody::MiniCharacterBody(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber{ MiniCharacter::GetNodeCountAfterCountUp() }
	, m_partNumber{ MiniCharacter::GetPartsNumber() }
	, m_partID{ MiniCharacter::BODY }
	, m_pCommonResources(nullptr)
	, m_pModel(nullptr)
	, m_initialPosition(initialPosition)
	, m_time(0.0f)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition{ 0.0f, 0.0f, 0.0f }
	, m_currentAngle{}
	, m_mass(0.0f)

{
}

MiniCharacterBody::~MiniCharacterBody()
{
	Finalize();
}

void MiniCharacterBody::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	assert(commonResources);
	m_pCommonResources = commonResources;
	// ���f����ǂݍ���
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerBody");
	Attach(std::make_unique<MiniCharacterHead>(this, DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), 0.0f));// ������ǉ�
	Attach(std::make_unique<MiniCharacterHand>(this, DirectX::SimpleMath::Vector3(0.8f, 0.0f, 0.0f), 0.0f)); // �E���ǉ�
	Attach(std::make_unique<MiniCharacterHand>(this, DirectX::SimpleMath::Vector3(-0.8f, 0.0f, 0.0f), 0.0f)); // �����ǉ�
}

void MiniCharacterBody::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	m_time += elapsedTime;
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition + m_initialPosition;
	//m_currentPosition.y += Easing::Reflect(Easing::EaseInSine, m_time); // �C�[�W���O���g����Y���W��ω�������
	// ���݂̉�]�p���X�V����
	m_currentAngle = m_rotationBodyAngle * currentAngle;
	// �u���́v���i���X�V����
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		// ���i���X�V����
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}

void MiniCharacterBody::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	MiniCharacterPart->Initialize(m_pCommonResources);
	m_pMiniCharacterParts.emplace_back(std::move(MiniCharacterPart));

}

void MiniCharacterBody::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{

}

void MiniCharacterBody::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s��𐶐�����
	m_worldMatrix = Matrix::CreateScale(1) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition);


	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
	for (auto& part : m_pMiniCharacterParts)
	{
		part->Render(view, proj);
	}
}

void MiniCharacterBody::Finalize()
{
}
