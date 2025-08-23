/*
*	@file Sheep_Body.cpp
*	@brief �Ђ��̓��̃N���X
*/
#include "pch.h"
#include "Sheep_Body.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
/*
*	@brief �Ђ��̓��̃N���X
*	@detail �Ђ��̓��̃N���X�̃R���X�g���N�^
*	@param parent �e�R���|�[�l���g
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x
*	@return �Ȃ�
*/
SheepBody::SheepBody(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)// �e�R���|�[�l���g
	, m_nodeNumber{ MiniCharacter::GetNodeCountAfterCountUp() }// �m�[�h�ԍ����擾
	, m_partNumber{ MiniCharacter::GetPartsNumber() }// ���i�ԍ����擾
	, m_partID{ MiniCharacter::BODY } // ���iID���擾
	, m_pCommonResources(nullptr)// ���ʃ��\�[�X
	, m_pModel(nullptr)// ���f��
	, m_initialPosition(initialPosition)// �����ʒu
	, m_time(0.0f) // ����
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle)) // �����p�x
	, m_currentPosition{ 0.0f, 0.0f, 0.0f } // ���݂̈ʒu
	, m_currentAngle{} // ���݂̊p�x
	, m_mass(0.0f) // ����
{
}
/*
*	@brief �f�X�g���N�^
*	@detail �Ђ��̓��̃N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SheepBody::~SheepBody()
{
	// �㏈�����s��
	Finalize();
}
/*
*	@brief ����������
*	@detail �Ђ��̓��̃N���X�̏�����
*	@param commonResources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void SheepBody::Initialize(CommonResources* commonResources)
{
	// DirectX::SimpleMath���g�p���邽�߂̖��O��Ԃ��w��
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ肷��
	assert(commonResources);
	m_pCommonResources = commonResources;
	// ���f����ǂݍ���
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Sheep_Body");
	// ������ǉ�
	Attach(std::make_unique<SheepHead>(this, Vector3(0.0f, 1.0f, 0.0f), 0.0f));
	// ��֎Ԃ̓��̂�ǉ�
	Attach(std::make_unique<UnicycleBody>(this, Vector3(0.0f, -1.0f, 0.0f), 0.0f));

}
/*
*	@brief �X�V����
*	@detail �Ђ��̓��̃N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̊p�x
*	@return �Ȃ�
*/
void SheepBody::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// DirectX::SimpleMath���g�p���邽�߂̖��O��Ԃ��w��
	using namespace DirectX::SimpleMath;
	// ���Ԃ��X�V����
	m_time += elapsedTime;
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition + m_initialPosition;
	// ���݂̊p�x���X�V����
	m_currentAngle = m_rotationBodyAngle * currentAngle;
	// ���[���h�s��𐶐�����
	m_worldMatrix = Matrix::CreateScale(1) * // �X�P�[���s��𐶐�
		Matrix::CreateFromQuaternion(m_currentAngle) * // ��]�s��𐶐�
		Matrix::CreateTranslation(m_currentPosition); // ���s�ړ��s��𐶐�
	// �x�[�X���擾����
	auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent());
	// �V���h�E�}�b�v�Ƀ��f����o�^����
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
	// �u���́v���i���X�V����
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		// ���i���X�V����
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}
/*
*	@brief ���i��ǉ�����
*	@detail �Ђ��̓��̃N���X�ɕ��i��ǉ�����
*	@param MiniCharacterPart �ǉ����镔�i
*	@return �Ȃ�
*/
void SheepBody::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i������������
	MiniCharacterPart->Initialize(m_pCommonResources);
	// ���i��ǉ�����
	m_pMiniCharacterParts.emplace_back(std::move(MiniCharacterPart));
}
/*
*	@brief ���i���폜����
*	@detail �Ђ��̓��̃N���X���畔�i���폜����
*	@param MiniCharacterPart �폜���镔�i
*	@return �Ȃ�
*/
void SheepBody::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���͉������Ȃ�
}
/*
*	@brief �`�悷��
*	@detail �Ђ��̓��̃N���X�̕`��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void SheepBody::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX::SimpleMath���g�p���邽�߂̖��O��Ԃ��w��
	using namespace DirectX::SimpleMath;
	/// Direct3D�f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʃX�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���f����`�悷��
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
	// �u���́v���i��`�悷��
	for (auto& part : m_pMiniCharacterParts)part->Render(view, proj);
}
/*
*	@brief �㏈�����s��
*	@detail �Ђ��̓��̃N���X�̌㏈��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SheepBody::Finalize()
{
	// ���f�����������
	if (m_pModel)m_pModel = nullptr;
	// ���i��S�č폜����
	m_pMiniCharacterParts.clear();
}
