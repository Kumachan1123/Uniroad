/*
*	@file GoalLock.cpp
*	@brief �S�[�����b�N�̏������`����N���X
*/
#include "pch.h"
#include "GoalLock.h"
/*
*	@brief �R���X�g���N�^
*	@details �S�[�����b�N�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
GoalLock::GoalLock()
	:m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pMiniCharacter(nullptr) // �~�j�L�����N�^�[�ւ̃|�C���^
	, m_itemInfo() // �A�C�e�����
	, m_row(-1) // �s�ԍ��i�ۑ��p�j
	, m_col(-1) // ��ԍ��i�ۑ��p�j
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_time(0.0f) // �o�ߎ���
	, m_deleteTime(0.0f) // �l������ď�����܂ł̎���
	, m_rotationSpeed(0.0f) // ��]���x
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��]
	, m_isCollected(false) // �l�����ꂽ���ǂ���
{
}

/*
*	@brief �f�X�g���N�^
*	@details �S�[�����b�N�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
GoalLock::~GoalLock()
{
	// �������Ȃ�
}

/*
*	@brief ������
*	@details �S�[�����b�N�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param info �A�C�e�����
*	@return �Ȃ�
*/
void GoalLock::Initialize(CommonResources* resources, const ItemInfo& info)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �A�C�e������ݒ�
	m_itemInfo = info;
}
/*
*	@brief �X�V
*	@details �S�[�����b�N�̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void GoalLock::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_time += elapsedTime;
}
/*
*	@brief �S�[�����b�N�̕`��
*	@details �S�[�����b�N�̕`�揈�����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void GoalLock::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
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
*	@details �S�[�����b�N���擾�����Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void GoalLock::OnGet(MiniCharacter* miniCharacter)
{
	// �~�j�L�����N�^�[�ւ̃|�C���^��ݒ�
	m_pMiniCharacter = miniCharacter;
	// �l�����ꂽ�t���O��ݒ�
	m_isCollected = true;
	// �l������ď�����܂ł̎��Ԃ����Z�b�g
	m_deleteTime = 0.0f;
}
/*
*	@brief �A�C�e���̎g�p������
*	@details �S�[�����b�N���g�p�����Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void GoalLock::OnUse(MiniCharacter* miniCharacter)
{
	UNREFERENCED_PARAMETER(miniCharacter);
}

/*
*	@brief �A�C�e���̔j��������
*	@details �S�[�����b�N��j�������Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void GoalLock::OnDiscard(MiniCharacter* miniCharacter)
{
	// �A�C�e�����폜
	m_pMiniCharacter->GetParent()->GetCSVItem()->RemoveItem(m_row, m_col);
}