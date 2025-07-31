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
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageSelect");
}

void StageSelect::Update(float elapsedTime)
{
}

void StageSelect::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	const auto& context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	const auto& states = m_pCommonResources->GetCommonStates();
	const auto& device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// ���f���̕`��
	m_pModel->Draw(context, *states, Matrix::Identity, view, proj);
}

void StageSelect::Finalize()
{
}
