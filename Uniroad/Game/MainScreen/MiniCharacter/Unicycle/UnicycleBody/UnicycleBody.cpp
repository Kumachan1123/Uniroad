/*
*	@file UnicycleBody.cpp
*	@brief ��֎Ԃ̎ԑ̃N���X�̎���
*/
#include "pch.h"
#include "UnicycleBody.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleWheel/UnicycleWheel.h"
/*
*	@brief �R���X�g���N�^
*	@details ��֎Ԃ̎ԑ̃N���X�̃R���X�g���N�^
*	@param parent �e�R���|�[�l���g�ւ̃|�C���^
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x�i���W�A���j
*	@return �Ȃ�
*/
UnicycleBody::UnicycleBody(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)// �e�R���|�[�l���g�ւ̃|�C���^
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// �m�[�h�ԍ�
	, m_partNumber(MiniCharacter::GetPartsNumber())// ���i�ԍ� 
	, m_partID(MiniCharacter::UNICYCLE_BODY)// ���iID
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_initialPosition(initialPosition)// �����ʒu
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// �����p�x
	, m_currentPosition{} // ���݂̈ʒu
	, m_currentAngle{} // ���݂̉�]�p
	, m_rotationMiniCharacterAngle{} // �v���C���[��]�p
	, m_mass{} // ����
	, m_MiniCharacterVelocity{} // �v���C���[���x
	, m_worldMatrix{} // ���[���h�s��
	, m_time(0.0f)// ����
{
}
/*
*	@brief �f�X�g���N�^
*	@details ��֎Ԃ̎ԑ̃N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
UnicycleBody::~UnicycleBody()
{
	// �㏈�����s��
	Finalize();
}
/*
*	@brief ��֎Ԃ̎ԑ̃N���X�̏��������s��
*	@details ��֎Ԃ̎ԑ̃N���X�̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param commonResources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void UnicycleBody::Initialize(CommonResources* commonResources)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX;
	// ���ʃ��\�[�X�ւ̃|�C���^���L���ł��邱�Ƃ��m�F����
	assert(commonResources);
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ肷��
	m_pCommonResources = commonResources;
	// ���f����ǂݍ���
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Body");
	// ������ǉ�
	Attach(std::make_unique<UnicycleWheel>(this, DirectX::SimpleMath::Vector3(0.0f, -0.85f, 0.0f), 0.0f));
}
/*
*	@brief ��֎Ԃ̎ԑ̃N���X�̍X�V���s��
*	@details ��֎Ԃ̎ԑ̃N���X�̍X�V���s���A���݂̈ʒu�Ɖ�]�p���X�V����B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̉�]�p
*	@return �Ȃ�
*/
void UnicycleBody::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition + m_initialPosition;
	// ���݂̉�]�p���X�V����
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// �x�[�X���擾����
	const auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent()->GetParent());
	// �V���h�E�}�b�v�Ƀ��f����o�^����
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
	// �u���́v���i���X�V����
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		// ���i���X�V����
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}

void UnicycleBody::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
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
