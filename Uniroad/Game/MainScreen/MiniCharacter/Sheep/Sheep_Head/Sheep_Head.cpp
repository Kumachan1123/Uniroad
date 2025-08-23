/*
*	@file Sheep_Head.cpp
*	@brief �Ђ��̓��N���X�̎���
*/
#include "pch.h"
#include "Sheep_Head.h"
#include "Game/MainScreen/MiniCharacter/Sheep/Sheep_Body/Sheep_Body.h"
/*
*	@brief �Ђ��̓��N���X�̃R���X�g���N�^
*	@details �Ђ��̓��N���X�̃R���X�g���N�^
*	@param parent �e�R���|�[�l���g�ւ̃|�C���^
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x�i���W�A���j
*	@return �Ȃ�
*/
SheepHead::SheepHead(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_pParent(parent)// �e�R���|�[�l���g
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// �m�[�h�ԍ�
	, m_partNumber(MiniCharacter::GetPartsNumber()) // ���i�ԍ�
	, m_partID(MiniCharacter::UNICYCLE_WHEEL) // ���iID
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_time(0.0f) // ����
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
*	@brief �Ђ��̓��N���X�̃f�X�g���N�^
*	@details �Ђ��̓��N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SheepHead::~SheepHead()
{
	// �㏈�����s��
	Finalize();
}
/*
*	@brief �Ђ��̓��N���X�̏�����
*	@details �Ђ��̓��N���X�̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param commonResources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void SheepHead::Initialize(CommonResources* commonResources)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = commonResources;
	// ���f����ǂݍ���
	ChangeExpression("Sheep_Head");
}
/*
*	@brief �Ђ��̓��N���X�̍X�V
*	@details �Ђ��̓��N���X�̍X�V���s���A�ʒu��p�x���X�V����B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̊p�x
*	@return �Ȃ�
*/
void SheepHead::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition + m_initialPosition;
	// ���݂̉�]�p���X�V����
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// �x�[�X���擾
	auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent()->GetParent());
	// �V���h�E�}�b�v�Ƀ��f����o�^����
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
	// �~�j�L�����N���X���擾����
	auto pMiniCharacter = dynamic_cast<MiniCharacter*>(m_pParent->GetParent());
	// �~�j�L�����N�^�[�����݂��Ȃ��ꍇ�͉������Ȃ�
	if (pMiniCharacter == nullptr) return;
	// �\��ɉ����ă��f����؂�ւ���
	if (pMiniCharacter->GetExpression() == MiniCharacter::Expression::NORMAL)
		ChangeExpression("Sheep_Head");	// �ʏ�̊�
	else if (pMiniCharacter->GetExpression() == MiniCharacter::Expression::HAPPY)
		ChangeExpression("Sheep_HappyHead");	// �S�[���̎��̊�
	else if (pMiniCharacter->GetExpression() == MiniCharacter::Expression::BAD)
		ChangeExpression("Sheep_BadHead");	// ���������ɂȂ������◎���Ă��鎞�̊�

}
/*
*	@brief �Ђ��̓��N���X�̕`��
*	@details �Ђ��̓��N���X�̕`����s���B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void SheepHead::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g�ƃX�e�[�g���擾����
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s��𐶐�����
	m_worldMatrix = Matrix::CreateScale(Vector3::One) * // �X�P�[��
		Matrix::CreateFromQuaternion(m_currentAngle) * // ��]
		Matrix::CreateTranslation(m_currentPosition); // �ʒu
	// ���f����`�悷��
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
}
/*
*	@brief �Ђ��̓��N���X�̌㏈��
*	@details �Ђ��̓��N���X�̌㏈�����s���B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SheepHead::Finalize()
{
	// ���f�������݂���ꍇ�͉������
	if (m_pModel)m_pModel = nullptr;
}
/*
*	@brief �\���؂�ւ���
*	@details ���f���L�[��n���ĕ\���؂�ւ���
*	@param key �\��̃L�[
*	@return �Ȃ�
*/
void SheepHead::ChangeExpression(const std::string& key)
{
	// ���f���}�l�[�W���[���烂�f�����擾����
	m_pModel = m_pCommonResources->GetModelManager()->GetModel(key);
}
