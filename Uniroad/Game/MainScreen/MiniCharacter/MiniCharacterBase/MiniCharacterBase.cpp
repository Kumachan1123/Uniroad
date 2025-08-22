/*
*	@file MiniCharacterBase.cpp
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X
*/
#include <pch.h>
#include "MiniCharacterBase.h"
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�̃R���X�g���N�^
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�̃R���X�g���N�^�B
*	@param parent �e�R���|�[�l���g�ւ̃|�C���^
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x
*	@return �Ȃ�
*/
MiniCharacterBase::MiniCharacterBase(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)// �e�R���|�[�l���g�ւ̃|�C���^
	, m_nodeNumber(MiniCharacterBase::GetNodeNumber())// �m�[�h�ԍ����擾����
	, m_partNumber(MiniCharacterBase::GetPartNumber())// ���i�ԍ����擾����
	, m_partID(MiniCharacterBase::GetPartID())// ���iID���擾����
	, m_initialPosition(initialPosition)// �����ʒu��ݒ肷��
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// ������]�p��ݒ肷��
	, m_currentPosition(initialPosition)// ���݂̈ʒu�������ʒu�ɐݒ肷��
	, m_currentAngle(m_initialAngle)// ���݂̉�]�p��������]�p�ɐݒ肷��
	, m_mass(0.0f) // ���ʂ�����������
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^������������
	, m_pCSVMap(nullptr) // CSV�}�b�v�ւ̃|�C���^������������
	, m_pCSVItem(nullptr) // CSV�A�C�e���ւ̃|�C���^������������
	, m_pNextTiles(nullptr) // ���Ɍ����^�C���̃N���X�ւ̃|�C���^������������
	, m_pPlaneArea(nullptr) // ���ʃG���A�ւ̃|�C���^������������
	, m_pShadowMapLight(nullptr) // �V���h�E�}�b�v���C�g�ւ̃|�C���^������������
	, m_pCamera(nullptr) // �J�����ւ̃|�C���^������������
	, m_isMoving(false)// �~�j�L�����̈ړ��t���O������������
	, m_isGameOver(false) // �Q�[���I�[�o�[�t���O������������
	, m_isGameClear(false)	// �Q�[���N���A�t���O������������
	, m_titleAnimationState(NONE)// �^�C�g���V�[���ɂ�����A�j���[�V�����X�e�[�g������������
{
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�̃f�X�g���N�^
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�̃f�X�g���N�^�B
*	@param �Ȃ�
*	@return �Ȃ�
*/
MiniCharacterBase::~MiniCharacterBase()
{
	// �m�[�h���N���A����
	Finalize();
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɂ���
	m_pCommonResources = nullptr;
	// CSV�}�b�v�ւ̃|�C���^��nullptr�ɂ���
	m_pCSVMap = nullptr;
	// CSV�A�C�e���ւ̃|�C���^��nullptr�ɂ���
	m_pCSVItem = nullptr;
	// ���Ɍ����^�C���̃N���X�ւ̃|�C���^��nullptr�ɂ���
	m_pNextTiles = nullptr;
	// ���ʃG���A�ւ̃|�C���^��nullptr�ɂ���
	m_pPlaneArea = nullptr;
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�̏�����
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�̏������B
*	@param commonResources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void MiniCharacterBase::Initialize(CommonResources* commonResources)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ肷��
	m_pCommonResources = commonResources;
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�̍X�V
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�̍X�V�B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̉�]�p
*	@return �Ȃ�
*/
void MiniCharacterBase::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition;
	// ���݂̉�]�p���X�V����
	m_currentAngle = currentAngle;
	// �m�[�h���X�V����
	for (auto& node : m_nodes)
	{
		// �m�[�h�̍X�V�������Ăяo��
		node->Update(elapsedTime, m_currentPosition, m_currentAngle);
		// node��MiniCharacter�Ȃ�
		if (dynamic_cast<MiniCharacter*>(node.get()))
		{
			// ���W���J�����ɓn�����W�Ƃ���
			m_cameraPosition = node->GetPosition();
			// ���W���A�C�R���ɓn�����W�Ƃ���
			m_iconPosition = node->GetPosition();
		}
	}
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�ɕ��i��ǉ�����
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�ɕ��i��ǉ�����B
*	@param MiniCharacterPart �ǉ����镔�i�̃��j�[�N�|�C���^
*	@return �Ȃ�
*/
void MiniCharacterBase::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// �ǉ��������i������������
	MiniCharacterPart->Initialize(m_pCommonResources);
	// �m�[�h�ɒǉ�����
	m_nodes.emplace_back(std::move(MiniCharacterPart));
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X���畔�i���폜����
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X���畔�i���폜����B
*	@param MiniCharacterPart �폜���镔�i�̃��j�[�N�|�C���^
*	@return �Ȃ�
*/
void MiniCharacterBase::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i���폜����
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
		[&MiniCharacterPart](const std::unique_ptr<IComponent>& node)
		{
			return node.get() == MiniCharacterPart.get();
		});
	// �폜���镔�i�����������ꍇ�͕��i���폜����
	if (it != m_nodes.end())	m_nodes.erase(it);
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�̕`��
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�̕`��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void MiniCharacterBase::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{	// �m�[�h��`�悷��
	for (auto& node : m_nodes)node->Render(view, proj);
}
/*
*	@brief �~�j�L�����N�^�[�̃x�[�X�N���X�̌㏈��
*	@details �~�j�L�����N�^�[�̃x�[�X�N���X�̌㏈���B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MiniCharacterBase::Finalize()
{
	// �m�[�h���N���A����
	m_nodes.clear();
}
