/*
*	@file Medal.cpp
*	@brief ���_���N���X
*/
#include "pch.h"
#include "Medal.h"
/*
*	@brief �R���X�g���N�^
*	@details ���_���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Medal::Medal()
	:m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pMiniCharacter(nullptr) // �~�j�L�����N�^�[�ւ̃|�C���^
	, m_itemInfo() // �A�C�e�����
	, m_row(-1) // �s�ԍ��i�ۑ��p�j
	, m_col(-1) // ��ԍ��i�ۑ��p�j
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_time(0.0f) // �o�ߎ���
	, m_deleteTime(0.0f) // ���_�����l������ď�����܂ł̎���
	, m_rotationSpeed(0.0f) // ��]���x
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��]
	, m_isCollected(false) // ���_�����l�����ꂽ���ǂ���
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���_���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Medal::~Medal()
{
	// �������Ȃ�
}
/*
*	@brief ������
*	@details ���_���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param info �A�C�e�����
*	@return �Ȃ�
*/
void Medal::Initialize(CommonResources* resources, const ItemInfo& info)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �A�C�e������ݒ�
	m_itemInfo = info;
	// �����̉�]���x��ݒ�
	m_rotationSpeed = DEFAULT_ROTATION_SPEED;
}
/*
*	@brief �X�V
*	@details ���_���̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Medal::Update(float elapsedTime)
{
	// DirectX::SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �l�����ꂽ��
	if (m_isCollected)
	{
		// ������܂ł̎��Ԃ����Z
		m_deleteTime += elapsedTime;
		// Y���W�����炩�ɕς���
		m_position.y += Easing::EaseInCirc(m_deleteTime / 3.0f);

		// �l�����ꂽ�������܂ł̎��Ԃ�2�b�𒴂�����
		if (m_deleteTime > 2.0f)
		{
			// �A�C�e�����폜
			OnDiscard(m_pMiniCharacter);
			// �������I����
			return;
		}
	}

	// ���Ԍo�߂ŉ�]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time * DirectX::XM_PI * 2.0f / 5.0f * m_rotationSpeed, 0.0f, 0.0f);
}
/*
*	@brief �����蔻��`��
*	@details ���_���̓����蔻���`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Medal::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX::SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���f�����ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_pModel == nullptr) return;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʂ̃X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s���ݒ�
	m_worldMatrix = Matrix::CreateScale(m_itemInfo.scale) *
		Matrix::CreateFromQuaternion(m_rotation) *
		Matrix::CreateTranslation(m_position);
	// �����_�����O
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
}
/*
*	@brief �A�C�e���̎擾������
*	@details ���_�����擾�����Ƃ��A��������������Z����
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void Medal::OnGet(MiniCharacter* miniCharacter)
{
	// ���Ɋl������Ă���ꍇ�͉������Ȃ�
	if (m_isCollected) return;
	// �~�j�L�����N�^�[�̃��_�����������Z
	miniCharacter->GetParent()->GetCSVItem()->CountMedals();
	// ��]���x���l�����̑��x�ɐݒ�
	m_rotationSpeed = COLLECTED_ROTATION_SPEED;
	// ���_�����l�����ꂽ�t���O��ݒ�
	m_isCollected = true;
	// �|�C���^�[��ݒ�
	m_pMiniCharacter = miniCharacter;
}
/*
*	@brief �A�C�e���̎g�p������
*	@details ���_�����g�p�����Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void Medal::OnUse(MiniCharacter* miniCharacter)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(miniCharacter);
}
/*
*	@brief �A�C�e���̔j��������
*	@details ���_����j�������Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void Medal::OnDiscard(MiniCharacter* miniCharacter)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(miniCharacter);
	// �A�C�e�����폜
	m_pMiniCharacter->GetParent()->GetCSVItem()->RemoveItem(m_row, m_col);
}
