/*
*	@file Shadow.cpp
*	@brief �e�|���S����`�悷��N���X�̎����t�@�C��
*/
#include "pch.h"
#include "Shadow.h"
/*
*	@brief �R���X�g���N�^
*	@details �e�|���S����`�悷��N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Shadow::Shadow()
	: m_pTexture(nullptr) // �e�N�X�`��
	, m_pPrimitiveBatch{} // �v���~�e�B�u�o�b�`
	, m_pBasicEffect{} // �x�[�V�b�N�G�t�F�N�g
	, m_pInputLayout{} // �C���v�b�g���C�A�E�g
	, m_pDepthStencilState{} // �[�x�X�e���V���X�e�[�g
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
{
}
/*
*	@brief ������
*	@details �e�|���S����`�悷��N���X�̏�����
*	@param pCommonResources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void Shadow::Initialize(CommonResources* resources)
{
	// DirectX::���O��Ԃ��g�p
	using namespace DirectX;
	// ���ʃ��\�[�X�ւ̃|�C���^���Z�b�g
	m_pCommonResources = resources;
	// ���\�[�X����f�o�C�X���擾
	ID3D11Device* pDevice = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	//// ���ʃX�e�[�g���擾
	//CommonStates* states = m_pCommonResources->GetCommonStates();
	// �x�[�V�b�N�G�t�F�N�g��ݒ�(���_�J���[�Ȃ��A�e�N�X�`���L���A���C�e�B���O����)
	m_pBasicEffect = std::make_unique<BasicEffect>(pDevice);
	m_pBasicEffect->SetVertexColorEnabled(false);
	m_pBasicEffect->SetTextureEnabled(true);
	m_pBasicEffect->SetLightingEnabled(false);
	// ���̓��C�A�E�g��ݒ肷��
	DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionTexture>(pDevice, m_pBasicEffect.get(), m_pInputLayout.ReleaseAndGetAddressOf()));
	// �v���~�e�B�u�o�b�`���쐬
	m_pPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	// �摜��ǂݍ���
	m_pTexture = resources->GetTextureManager()->GetTexture("Shadow");
	// �[�x�X�e���V���o�b�t�@��ݒ肷��
	CreateDepthStenciBuffer(pDevice);
}
/*
*	@brief �`��
*	@details �e�|���S����`�悷��
*	@param view �r���[�}�g���b�N�X
*	@param projection �v���W�F�N�V�����}�g���b�N�X
*	@param position �e�|���S���̈ʒu
*	@param radius �e�|���S���̔��a(�f�t�H���g��0.7f)
*/
void Shadow::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::SimpleMath::Vector3& position,
	float radius)
{
	// DirectX�EDirectX::SimpleMath���O��Ԃ��g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// �A���t�@�u�����h��K�p����
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);

	// �[�x�X�e���V���X�e�[�g��K�p����
	context->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);	// �Q�ƒl�F1

	// �J�����O��ݒ肷��
	context->RSSetState(states->CullCounterClockwise());

	// �e�N�X�`���T���v����K�p����
	ID3D11SamplerState* sampler = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, &sampler);

	// ���̓��C�A�E�g��K�p����
	context->IASetInputLayout(m_pInputLayout.Get());

	// �G�t�F�N�g��ݒ肵�K�p����
	m_pBasicEffect->SetWorld(Matrix::Identity);
	m_pBasicEffect->SetView(view);
	m_pBasicEffect->SetProjection(projection);
	m_pBasicEffect->SetTexture(m_pTexture.Get());
	m_pBasicEffect->Apply(context);

	// �e�|���S���̒��_�����`����
	VertexPositionTexture vertices[] = {
		VertexPositionTexture(Vector3::Zero,Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3::Zero,Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3::Zero,Vector2(0.0f, 1.0f)),
		VertexPositionTexture(Vector3::Zero,Vector2(1.0f, 1.0f)),
	};
	// �e�|���S���̃C���f�b�N�X�����`����FDrawIndexed�ŕ`�悷��Ƃ��ɕK�v
	uint16_t indexes[] = { 0, 1, 2, 1, 3, 2 };
	// �e�|���S���̒��_����ݒ肷��
	vertices[0].position = Vector3(-radius, 1.0f, -radius) + position;
	vertices[1].position = Vector3(radius, 1.0f, -radius) + position;
	vertices[2].position = Vector3(-radius, 1.0f, radius) + position;
	vertices[3].position = Vector3(radius, 1.0f, radius) + position;
	// �`��J�n
	m_pPrimitiveBatch->Begin();
	// �e�|���S����`�悷��
	m_pPrimitiveBatch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indexes,
		_countof(indexes),
		vertices,
		_countof(vertices)
	);

	// �`��I��
	m_pPrimitiveBatch->End();
}
/*
*	@brief �[�x�X�e���V���o�b�t�@��ݒ肷��
*	@details �e�|���S����`�悷�邽�߂̐[�x�X�e���V���o�b�t�@��ݒ肷��
*	@param pDevice Direct3D�f�o�C�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void Shadow::CreateDepthStenciBuffer(ID3D11Device* pDevice)
{
	// �[�x�X�e���V���X�e�[�g�̐ݒ���s��
	D3D11_DEPTH_STENCIL_DESC desc{};
	// �e�F�`���̃X�e���V���o�b�t�@�ƎQ�ƒl�������Ƃ��e��`�悷��
	desc.DepthEnable = true;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����
	desc.StencilEnable = true;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff
	// �\��
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// �Q�ƒl�ƃX�e���V���l�����l�Ȃ�
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// OK�@�������Ȃ�
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG�@�������Ȃ�
	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;
	// �[�x�X�e���V���X�e�[�g���쐬����
	pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf());

}
