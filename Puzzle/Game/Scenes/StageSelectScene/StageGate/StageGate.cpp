/*
*	@file StageGate.cpp
*	@brief �X�e�[�W�Q�[�g�N���X
*/
#include "pch.h"
#include "StageGate.h"
/*
*	@brief �R���X�g���N�^
*	@details �X�e�[�W�Q�[�g�N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
StageGate::StageGate(CommonResources* resources)
	: m_pCommonResources(resources) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModel(nullptr) // ���f���|�C���^�[
{
}
/*
*	@brief �f�X�g���N�^
*	@details �X�e�[�W�Q�[�g�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
StageGate::~StageGate()
{
	// �I���������Ăяo��
	Finalize();
}
/*
*	@brief ������
*	@details �X�e�[�W�Q�[�g�N���X�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageGate::Initialize()
{
	// ���f���̎擾
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageGate");
}
/*
*	@brief �X�V
*	@details �X�e�[�W�Q�[�g�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void StageGate::Update(float elapsedTime)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*	@brief �`��
*	@details �X�e�[�W�Q�[�g�N���X�̕`����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void StageGate::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	const auto& context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	const auto& states = m_pCommonResources->GetCommonStates();
	m_pModel->Draw(context, *states, Matrix::Identity * Matrix::CreateTranslation(m_position), view, proj, false);
}
/*
*	@brief �I��
*	@details �X�e�[�W�Q�[�g�N���X�̏I�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageGate::Finalize()
{
	// ���f���̉��
	m_pModel = nullptr;
}
