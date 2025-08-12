/*
*	@file UnicycleWheel.cpp
*	@brief UnicycleWheel�N���X�̎���
*/
#include "pch.h"
#include "UnicycleWheel.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleBody/UnicycleBody.h"
/*
*	@brief �R���X�g���N�^
*	@details ��֎Ԃ̃^�C���N���X�̃R���X�g���N�^
*	@param parent �e�R���|�[�l���g�ւ̃|�C���^
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x�i���W�A���j
*	@return �Ȃ�
*/
UnicycleWheel::UnicycleWheel(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_pParent(parent)// �e�R���|�[�l���g
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// �m�[�h�ԍ�
	, m_partNumber(MiniCharacter::GetPartsNumber()) // ���i�ԍ�
	, m_partID(MiniCharacter::UNICYCLE_WHEEL) // ���iID
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_time(0.0f) // ����
	, m_wheelAngle(0.0f) // ��֎Ԃ̃^�C���̊p�x
	, m_initialPosition(initialPosition)// �����ʒu
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// �����p�x
	, m_currentPosition{} // ���݂̈ʒu
	, m_currentAngle{} // ���݂̉�]�p
	, m_rotationMiniCharacterAngle{} // �v���C���[��]�p
	, m_mass{} // ����
	, m_MiniCharacterVelocity{}// �v���C���[���x
{
}
/*
*	@brief �f�X�g���N�^
*	@details ��֎Ԃ̃^�C���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
UnicycleWheel::~UnicycleWheel()
{
	// �㏈�����s��
	Finalize();
}
/*
*	@brief ��֎Ԃ̃^�C���N���X�̏��������s��
*	@details ��֎Ԃ̃^�C���N���X�̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param commonResources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void UnicycleWheel::Initialize(CommonResources* commonResources)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = commonResources;
	// ���f����ǂݍ���
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Wheel");
}
/*
*	@brief ��֎Ԃ̃^�C���N���X�̍X�V���s��
*	@details ��֎Ԃ̃^�C���N���X�̍X�V���s���A�ʒu��p�x���X�V����B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̉�]�p
*	@return �Ȃ�
*/
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
	// �ԗ֔��a
	const float wheelRadius = 1.0f;
	// �i�s���x
	float wheelSpeed = m_MiniCharacterVelocity.Length();
	// �i�񂾋��� = ���x �~ ���ԁA��]�p�x = ���� �� ���a
	float deltaAngle = wheelSpeed * elapsedTime / wheelRadius;
	// �ݐω�]�p�x�ɉ��Z
	m_wheelAngle += deltaAngle;
	// ��]�N�H�[�^�j�I�������i�E��]�j
	Quaternion wheelRotation = Quaternion::CreateFromAxisAngle(Vector3::Right, m_wheelAngle);
	// �������čŏI��]��
	m_currentAngle = wheelRotation * m_currentAngle;
	// �e��MiniCharacter�̎�����Ȃ��i�v���C�V�[������Ȃ��j�ꍇ�z�C�[���̉�]���x�𒲐�����
	if (!dynamic_cast<MiniCharacter*>(pMiniCharacter))wheelSpeed = 1.0f * elapsedTime * 30;
	// ���Ԍo�߂Ńz�C�[������]������
	m_currentAngle = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_time * wheelSpeed) * m_currentAngle;
}
/*
*	@brief ��֎Ԃ̃^�C���N���X�̕`����s��
*	@details ��֎Ԃ̃^�C���N���X�̕`����s���A���[���h�s���ݒ肵�ă��f����`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void UnicycleWheel::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g�ƃX�e�[�g���擾����
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s��𐶐�����
	m_worldMatrix = Matrix::CreateScale(1) * // �X�P�[��
		Matrix::CreateFromQuaternion(m_currentAngle) * // ��]
		Matrix::CreateTranslation(m_currentPosition); // �ʒu
	// ���f����`�悷��
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
}
/*
*	@brief ��֎Ԃ̃^�C���N���X�̌㏈�����s��
*	@details ��֎Ԃ̃^�C���N���X�̌㏈�����s���A���\�[�X���������B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void UnicycleWheel::Finalize()
{
	// ���f�������݂���ꍇ�͉������
	if (m_pModel)m_pModel = nullptr;
}
