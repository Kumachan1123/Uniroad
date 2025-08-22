/*
*	@file StageSelect.cpp
*	@brief �X�e�[�W�Z���N�g�V�[���̃X�e�[�W�Z���N�g�N���X
*/
#include "pch.h"
#include "StageSelect.h"
/*
*	@brief �R���X�g���N�^
*	@details �X�e�[�W�Z���N�g�N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
StageSelect::StageSelect(CommonResources* resources)
	: m_pCommonResources(resources) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModel(nullptr) // ���f���|�C���^�[
	, m_pShadowMapLight(nullptr) // �V���h�E�}�b�v���C�g�ւ̃|�C���^
{
}
/*
*	@brief �f�X�g���N�^
*	@details �X�e�[�W�Z���N�g�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
StageSelect::~StageSelect()
{

	// ���ʃ��\�[�X�̉���͕s�v
}
/*
*	@brief ������
*	@details �X�e�[�W�Z���N�g�N���X�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelect::Initialize()
{
	// ���f���̎擾
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageSelect");
}
/*
*	@brief �X�V
*	@details �X�e�[�W�Z���N�g�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void StageSelect::Update(float elapsedTime)
{
	// �X�V�����͓��ɂȂ�
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
	// �V���h�E�}�b�v���C�g�̍X�V
	m_pShadowMapLight->Update(elapsedTime);
}
/*
*	@brief �`��
*	@details �X�e�[�W�Z���N�g�N���X�̕`����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void StageSelect::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
	const auto& context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	const auto& states = m_pCommonResources->GetCommonStates();
	// ���f���̕`��
	m_pModel->Draw(context, *states, Matrix::Identity, view, proj, false, [&]
		{
			// �e�������������C�e�B���O
			m_pShadowMapLight->ApplyShader(context, states);
		});
}
/*
*	@brief �I��
*	@details �X�e�[�W�Z���N�g�N���X�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelect::Finalize()
{
	// ���f�������݂���ꍇ�͉������
	if (m_pModel) m_pModel = nullptr;
}
