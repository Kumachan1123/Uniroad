/*
*	@file Road.cpp
*	@brief ���H�̕`����s���N���X
*/
#include "pch.h"
#include "Road.h"
/*
*	@brief ���H�̕`����s���N���X�̃R���X�g���N�^
*	@details ���H�̕`����s���N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
Road::Road(CommonResources* resources)
	: m_pCommonResources(resources) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModels() // ���f���ւ̃|�C���^
	, m_positions() // ���H�̈ʒu
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ���H�̉�]
	, m_world(DirectX::SimpleMath::Matrix::Identity) // ���H�̃��[���h�s��
	, m_view(DirectX::SimpleMath::Matrix::Identity) // �r���[�}�g���b�N�X
	, m_proj(DirectX::SimpleMath::Matrix::Identity) // �v���W�F�N�V�����}�g���b�N�X
	, m_time(0.0f) // ����
	, m_scrollSpeed(5.0f) // �X�N���[�����x
{
}
/*
*	@brief ���H�̕`����s���N���X�̃f�X�g���N�^
*	@details ���H�̕`����s���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Road::~Road()
{
	// �㏈�����s��
	Finalize();
}
/*
*	@brief ������
*	@details ���H�̕`����s���N���X�̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param commonResources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void Road::Initialize()
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �����쐬
	for (int i = 0; i < 2; i++)
	{	// ���f����ǂݍ���
		m_pModels.push_back(m_pCommonResources->GetModelManager()->GetModel("Road"));
		// �����ʒu�����Z�b�g
		m_positions.push_back(Vector3(i * 10.0f, -0.5f, 0.0f));
	}
}
/*
*	@brief �X�V
*	@details ���H�̕`����s���N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Road::Update(float elapsedTime)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �X�N���[��
	for (int i = 0; i < 2; ++i)	m_positions[i].x -= m_scrollSpeed * elapsedTime;
	// ���[�v�����F��O�ɗ���������ɉ�
	// ���̒������`
	const float roadLength = 10.0f;
	// ���H�̈ʒu���X�V
	for (int i = 0; i < 2; ++i)
	{
		// ���H�̈ʒu����O�ɗ����牜�ɉ�
		if (m_positions[i].x < -roadLength)
		{
			// ��������̓��H�̈ʒu���擾
			int other = (i == 0) ? 1 : 0;
			// ��O�̓��H�̈ʒu�����̓��H�̈ʒu�ɐݒ�
			m_positions[i].x = m_positions[other].x + roadLength;
		}
	}
}
/*
*	@brief �`��
*	@details ���H�̕`����s���N���X�̕`����s��
*	@param view �r���[�}�g���b�N�X
*	@param proj �v���W�F�N�V�����}�g���b�N�X
*	@return �Ȃ�
*/
void Road::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �f�o�C�X�R���e�L�X�g���擾
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	const auto states = m_pCommonResources->GetCommonStates();
	// ���f����`��
	for (int i = 0; i < 2; ++i)
	{
		// ���[���h�s����쐬 
		auto world = DirectX::SimpleMath::Matrix::CreateTranslation(m_positions[i]);
		// ���f����`��
		m_pModels[i]->Draw(context, *states, world, view, proj, false);
	}
}
/*
*	@brief �I��
*	@details ���H�̕`����s���N���X�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Road::Finalize()
{
	// ���\�[�X���������
	m_pCommonResources = nullptr;
	// ���f�������
	for (auto& model : m_pModels) if (model)model = nullptr;

}
