#include "pch.h"
#include "UnicycleWheel.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleBody/UnicycleBody.h"
#include "KumachiLib/Easing/Easing.h"

UnicycleWheel::UnicycleWheel(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())
	, m_partNumber(MiniCharacter::GetPartsNumber())
	, m_partID(MiniCharacter::UNICYCLE_WHEEL)
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

UnicycleWheel::~UnicycleWheel()
{
	Finalize();
}

void UnicycleWheel::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	assert(commonResources);
	m_pCommonResources = commonResources;
	// ���f����ǂݍ���
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Wheel");
}

void UnicycleWheel::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �e���擾����
	const auto pMiniCharacter = m_pParent->GetParent()->GetParent();

	// ���Ԃ����Z
	m_time += elapsedTime;
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition + m_initialPosition;
	// ���݂̉�]�p���X�V����
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// IsMoving() �� true �̏ꍇ�݂̂���ȍ~�̏������s��
	if (!pMiniCharacter || !pMiniCharacter->IsMoving())return;

	// �L�����N�^�[���̂��̂��瑬�x�x�N�g�����擾����
	m_MiniCharacterVelocity = pMiniCharacter->GetVelocity();
	// ���x�ɉ����ăz�C�[���̉�]���x��ς���
	float wheelSpeed = m_MiniCharacterVelocity.Length() * elapsedTime * 60;
	// �e��MiniCharacter�̎�����Ȃ��i�v���C�V�[������Ȃ��j�ꍇ�z�C�[���̉�]���x�𒲐�����
	if (!dynamic_cast<MiniCharacter*>(pMiniCharacter))wheelSpeed = 1.0f * elapsedTime * 30;
	// ���Ԍo�߂Ńz�C�[������]������
	m_currentAngle = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_time * wheelSpeed) * m_currentAngle;
}

void UnicycleWheel::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();

	// ���[���h�s��𐶐�����
	m_worldMatrix = Matrix::CreateScale(1) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition);
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);

}

void UnicycleWheel::Finalize()
{
}
