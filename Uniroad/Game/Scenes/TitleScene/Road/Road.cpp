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
	, m_pShadowMapLight(nullptr) // �V���h�E�}�b�v���C�g�ւ̃|�C���^
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
		m_positions.push_back(Vector3(i * 10.0f, -0.52f, 0.0f));
	}
	// �[�x�X�e���V���o�b�t�@���쐬
	CreateDepthStencilBuffer(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
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
		m_pModels[i]->Draw(context, *states, world, view, proj, false, [&]
			{
				//// �u�����h�X�e�[�g��ݒ肷��
				//context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
				//// �[�x�X�e���V���X�e�[�g��ݒ肷��
				//context->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);	// �Q�ƒl�F0
				//// �J�����O��ݒ肷��
				//context->RSSetState(states->CullCounterClockwise());
				//// �e�N�X�`���T���v����K�p����
				//ID3D11SamplerState* sampler = states->PointWrap();
				//context->PSSetSamplers(0, 1, &sampler);
				m_pShadowMapLight->ApplyShader(context, states);
			});
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
/*
*	@brief �[�x�X�e���V���o�b�t�@���쐬����
*	@details �[�x�X�e���V���o�b�t�@���쐬����B
*	@param pDevice Direct3D�f�o�C�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void Road::CreateDepthStencilBuffer(ID3D11Device* pDevice)
{
	// �[�x�X�e���V�������`����
	D3D11_DEPTH_STENCIL_DESC desc = {};

	// ���F���`�掞�ɃX�e���V���o�b�t�@�̒l���C���N�������g����i0��1�ɂ���j
	desc.DepthEnable = true;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����

	desc.StencilEnable = true;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff�i�}�X�N�l�j
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff�i�}�X�N�l�j

	// �|���S���̕\�ʂ̐ݒ�
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;
	// �[�x�X�e���V���X�e�[�g���쐬����
	DX::ThrowIfFailed(pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf()));
}
