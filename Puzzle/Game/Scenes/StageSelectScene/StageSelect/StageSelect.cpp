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
	// ���f���̎擾
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageSelect");
	// �[�x�X�e���V���̐ݒ�
	CreateDepthStencilBuffer(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
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
			// �u�����h�X�e�[�g��ݒ肷��
			context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
			// �[�x�X�e���V���X�e�[�g��ݒ肷��
			context->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
			// �J�����O��ݒ肷��
			context->RSSetState(states->CullNone());
			// �e�N�X�`���T���v����K�p����
			ID3D11SamplerState* sampler = states->PointWrap();
			context->PSSetSamplers(0, 1, &sampler);
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
	// �[�x�X�e���V���X�e�[�g���������
	m_pDepthStencilState.Reset();
}
/*
*	@brief �[�x�X�e���V���o�b�t�@���쐬����
*	@details �X�e�[�W�Z���N�g�̐[�x�X�e���V���o�b�t�@���쐬����
*	@param pDevice Direct3D�f�o�C�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void StageSelect::CreateDepthStencilBuffer(ID3D11Device* pDevice)
{
	// �[�x�X�e���V�������`����
	D3D11_DEPTH_STENCIL_DESC desc = {};
	// ���F���`�掞�ɃX�e���V���o�b�t�@�̒l���C���N�������g����
	desc.DepthEnable = true;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����

	desc.StencilEnable = true;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff�i�}�X�N�l�j
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff�i�}�X�N�l�j

	// �|���S���̕\�ʂ̐ݒ�
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;
	// �[�x�X�e���V���X�e�[�g���쐬����
	DX::ThrowIfFailed(pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf()));
}
