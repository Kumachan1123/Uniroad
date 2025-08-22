/*
*	@file	DrawPolygon.cpp
*	@brief	�|���S���`��p�N���X
*	@details �|���S����`�悷�邽�߂̃N���X
*/
#include <pch.h>
#include "DrawPolygon.h"
// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<DrawPolygon> DrawPolygon::m_pInstance = nullptr;
/*
*	@brief �V���O���g���C���X�^���X���擾
*	@details �V���O���g���C���X�^���X���擾����
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
DrawPolygon* const DrawPolygon::GetInstance()
{
	// �C���X�^���X���Ȃ��ꍇ
	if (m_pInstance == nullptr)
	{
		// �C���X�^���X�𐶐�
		m_pInstance.reset(new DrawPolygon());
	}
	// �C���X�^���X��Ԃ�
	return m_pInstance.get();
}

/*
*	@brief �R���X�g���N�^
*	@details �R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
DrawPolygon::DrawPolygon()
	: m_pPrimitiveBatchTexture(nullptr)// �v���~�e�B�u�o�b�`(���_�A�e�N�X�`��)
	, m_pPrimitiveBatchColorTexture(nullptr)// �v���~�e�B�u�o�b�`(���_�A�F�A�e�N�X�`��)
	, m_pStates(nullptr)// �R�����X�e�[�g
	, m_pDeviceContext(nullptr)	// �f�o�C�X�R���e�L�X�g
	, m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_pDevice(nullptr)// �f�o�C�X
{
}
/*
*	@brief �f�X�g���N�^
*	@details �f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
DrawPolygon::~DrawPolygon()
{
	// �v���~�e�B�u�o�b�`�̉���i���_�A�e�N�X�`���j
	ReleasePositionTexture();
	// �v���~�e�B�u�o�b�`�̉���i���_�A�F�A�e�N�X�`���j
	ReleasePositionColorTexture();
}
/*
*	@brief ������
*	@details �������i���_�A�e�N�X�`���j���s��
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X���擾
	m_pDevice = pDR->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	m_pDeviceContext = pDR->GetD3DDeviceContext();
	// �v���~�e�B�u�o�b�`���쐬����
	m_pPrimitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	// �R�����X�e�[�g���쐬����
	m_pStates = std::make_unique<CommonStates>(m_pDevice);
}

/*
*	@brief ������
*	@details �������i���_�A�F�A�e�N�X�`���j���s��
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X���擾
	m_pDevice = pDR->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	m_pDeviceContext = pDR->GetD3DDeviceContext();
	// �v���~�e�B�u�o�b�`�̍쐬
	m_pPrimitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());
	// �R�����X�e�[�g���쐬����
	m_pStates = std::make_unique<CommonStates>(m_pDevice);
}
/*
*	@brief �`��J�n
*	@details �`��J�n���ɌĂяo���֐�
*	@param pInputLayout ���̓��C�A�E�g
*	@param textures �e�N�X�`���z��
*	@return �Ȃ�
*/
void DrawPolygon::DrawStart(ID3D11InputLayout* pInputLayout, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures)
{
	// �s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����
	for (int i = 0; i < textures.size(); i++)m_pDeviceContext->PSSetShaderResources(i, 1, textures[i].GetAddressOf());
	// �C���v�b�g���C�A�E�g�̓o�^
	m_pDeviceContext->IASetInputLayout(pInputLayout);
}
/*
*	@brief �`��O�ݒ�
*	@details �`��O�Ɋe��X�e�[�g��ݒ肷��
*	@param ss �T���v���[�X�e�[�g
*	@param bs �u�����h�X�e�[�g
*	@param rs ���X�^���C�U�[�X�e�[�g
*	@param dss �[�x�X�e���V���X�e�[�g
*	@return �Ȃ�
*/
void DrawPolygon::DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss)
{
	// �摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1];
	// �T���v���[�X�e�[�g�̎�ނ𔻕ʂ��āA�T���v���[��ݒ肷��
	switch (ss)
	{
	case DrawPolygon::SamplerStates::ANISOTROPIC_CLAMP:// �A�j�\�g���s�b�N�N�����v
		// �A�j�\�g���s�b�N�N�����v�̃T���v���[���擾
		sampler[0] = m_pStates->AnisotropicClamp();
		break;
	case DrawPolygon::SamplerStates::ANISOTROPIC_WRAP:// �A�j�\�g���s�b�N���b�v
		// �A�j�\�g���s�b�N���b�v�̃T���v���[���擾
		sampler[0] = m_pStates->AnisotropicWrap();
		break;
	case DrawPolygon::SamplerStates::LINEAR_CLAMP: // ���j�A�N�����v
		// ���j�A�N�����v�̃T���v���[���擾
		sampler[0] = m_pStates->LinearClamp();
		break;
	case DrawPolygon::SamplerStates::LINEAR_WRAP: // ���j�A���b�v
		// ���j�A���b�v�̃T���v���[���擾
		sampler[0] = m_pStates->LinearWrap();
		break;
	case DrawPolygon::SamplerStates::POINT_CLAMP: // �|�C���g�N�����v
		// �|�C���g�N�����v�̃T���v���[���擾
		sampler[0] = m_pStates->PointClamp();
		break;
	case DrawPolygon::SamplerStates::POINT_WRAP: // �|�C���g���b�v
		// �|�C���g���b�v�̃T���v���[���擾
		sampler[0] = m_pStates->PointWrap();
		break;
	default: // �f�t�H���g��nullptr
		// �T���v���[��nullptr�ɐݒ�
		sampler[0] = nullptr;
		break;
	}
	// �s�N�Z���V�F�[�_�ɃT���v���[��o�^����
	m_pDeviceContext->PSSetSamplers(0, 1, sampler);
	// �u�����h�X�e�[�g�̓o�^
	ID3D11BlendState* blendState;
	// �u�����h�X�e�[�g�̎�ނ𔻕ʂ��āA�u�����h�X�e�[�g��ݒ肷��
	switch (bs)
	{
	case DrawPolygon::BlendStates::ALPHA:// �A���t�@�u�����h
		// �A���t�@�u�����h�̃u�����h�X�e�[�g���擾
		blendState = m_pStates->AlphaBlend();
		break;
	case DrawPolygon::BlendStates::ADDITIVE: // ���Z�u�����h
		// ���Z�u�����h�̃u�����h�X�e�[�g���擾
		blendState = m_pStates->Additive();
		break;
	case DrawPolygon::BlendStates::OPAQUE: // �s����
		// �s�����̃u�����h�X�e�[�g���擾
		blendState = m_pStates->Opaque();
		break;
	case DrawPolygon::BlendStates::NONPREMULTIPLIED: // ��v���}���`�v���C�h
		// ��v���}���`�v���C�h�̃u�����h�X�e�[�g���擾
		blendState = m_pStates->NonPremultiplied();
		break;
	default: // �f�t�H���g��nullptr
		// �u�����h�X�e�[�g��nullptr�ɐݒ�
		blendState = nullptr;
		break;
	}
	// �u�����h�X�e�[�g��ݒ肷��
	m_pDeviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	// �[�x�X�e���V���X�e�[�g�̓o�^
	ID3D11DepthStencilState* depthStencilState;
	// �[�x�X�e���V���X�e�[�g�̎�ނ𔻕ʂ��āA�[�x�X�e���V���X�e�[�g��ݒ肷��
	switch (dss)
	{
	case DrawPolygon::DepthStencilStates::DEPTH_DEFAULT:// �f�t�H���g
		// �f�t�H���g�̐[�x�X�e���V���X�e�[�g���擾
		depthStencilState = m_pStates->DepthDefault();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_NONE: // �[�x�Ȃ�
		// �[�x�Ȃ��̐[�x�X�e���V���X�e�[�g���擾
		depthStencilState = m_pStates->DepthNone();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ: // �[�x�ǂݎ��
		// �[�x�ǂݎ��̐[�x�X�e���V���X�e�[�g���擾
		depthStencilState = m_pStates->DepthRead();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ_REVERSE_Z: // �[�x�ǂݎ��i�tZ�j
		// �[�x�ǂݎ��i�tZ�j�̐[�x�X�e���V���X�e�[�g���擾
		depthStencilState = m_pStates->DepthReadReverseZ();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_REVERSE_Z: // �tZ
		// �tZ�̐[�x�X�e���V���X�e�[�g���擾
		depthStencilState = m_pStates->DepthReverseZ();
		break;
	default: // �f�t�H���g��nullptr
		// �[�x�X�e���V���X�e�[�g��nullptr�ɐݒ�
		depthStencilState = nullptr;
		break;
	}
	// �[�x�X�e���V���X�e�[�g��ݒ肷��
	m_pDeviceContext->OMSetDepthStencilState(depthStencilState, 0);

	// ���X�^���C�U�[�X�e�[�g�̓o�^
	ID3D11RasterizerState* rasterizerState;
	// ���X�^���C�U�[�X�e�[�g�̎�ނ𔻕ʂ��āA���X�^���C�U�[�X�e�[�g��ݒ肷��
	switch (rs)
	{
	case DrawPolygon::RasterizerStates::CULL_CLOCKWISE: // ���v���
		// ���v���̃��X�^���C�U�[�X�e�[�g���擾
		rasterizerState = m_pStates->CullClockwise();
		break;
	case DrawPolygon::RasterizerStates::CULL_COUNTERCLOCKWISE: // �����v���
		// �����v���̃��X�^���C�U�[�X�e�[�g���擾
		rasterizerState = m_pStates->CullCounterClockwise();
		break;
	case DrawPolygon::RasterizerStates::CULL_NONE: // �J�����O�Ȃ�
		// �J�����O�Ȃ��̃��X�^���C�U�[�X�e�[�g���擾
		rasterizerState = m_pStates->CullNone();
		break;
	case DrawPolygon::RasterizerStates::WIREFRAME: // ���C���[�t���[��
		// ���C���[�t���[���̃��X�^���C�U�[�X�e�[�g���擾
		rasterizerState = m_pStates->Wireframe();
		break;
	default: // �f�t�H���g��nullptr
		// ���X�^���C�U�[�X�e�[�g��nullptr�ɐݒ�
		rasterizerState = nullptr;
		break;
	}
	// ���X�^���C�U�[�X�e�[�g��ݒ肷��
	m_pDeviceContext->RSSetState(rasterizerState);
}
/*
*	@brief ���\�[�X�̍X�V
*	@details ���\�[�X�̍X�V���s��
*	@param resource �X�V���郊�\�[�X
*	@param pSrcData �X�V����f�[�^
*	@return �Ȃ�
*/
void DrawPolygon::UpdateSubResources(ID3D11Resource* resource, const void* pSrcData)
{
	// �T�u���\�[�X�̍X�V���s��
	m_pDeviceContext->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);
}

/*
*	@brief �|���S���`��
*	@details �|���S����`�悷��(VertexPositionTexture�p)
*	@param vertices ���_���(VertexPositionTexture)
*	@return �Ȃ�
*/
void DrawPolygon::DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices)
{
	// �`��J�n
	m_pPrimitiveBatchTexture->Begin();
	// �l�p�`��`��
	m_pPrimitiveBatchTexture->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	// �`��I��
	m_pPrimitiveBatchTexture->End();

}
/*
*	@brief �|���S���`��
*	@details �|���S����`�悷��(VertexPositionColorTexture�p)
*	@param topology �v���~�e�B�u�g�|���W�[
*	@param vertices ���_���(VertexPositionColorTexture)
*	@param count ���_��
*	@return �Ȃ�
*/
void DrawPolygon::DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count)
{
	// �`��J�n
	m_pPrimitiveBatchColorTexture->Begin();
	// ���_��`��
	m_pPrimitiveBatchColorTexture->Draw(topology, vertices, count);
	// �`��I��
	m_pPrimitiveBatchColorTexture->End();
}
/*
*	@brief ���
*	@details ����i���_�A�e�N�X�`���j���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawPolygon::ReleasePositionTexture()
{
	// �v���~�e�B�u�o�b�`(���_�A�e�N�X�`��)�̉��
	m_pPrimitiveBatchTexture.reset();
	// �R�����X�e�[�g�̉��
	m_pStates.reset();
	// �f�o�C�X�R���e�L�X�g�̉��
	m_pDeviceContext = nullptr;
	// �f�o�C�X���\�[�X�̉��
	m_pDR = nullptr;
	// �f�o�C�X�̉��
	m_pDevice = nullptr;
}
/*
*	@brief ���
*	@details ����i���_�A�F�A�e�N�X�`���j���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawPolygon::ReleasePositionColorTexture()
{
	// �v���~�e�B�u�o�b�`(���_�A�F�A�e�N�X�`��)�̉��
	m_pPrimitiveBatchColorTexture.reset();
	// �R�����X�e�[�g�̉��
	m_pStates.reset();
	// �f�o�C�X�R���e�L�X�g�̉��
	m_pDeviceContext = nullptr;
	// �f�o�C�X���\�[�X�̉��
	m_pDR = nullptr;
	// �f�o�C�X�̉��
	m_pDevice = nullptr;
}
/*
*	@brief �V�F�[�_�[�Ƀo�b�t�@�𑗂�
*	@details �V�F�[�_�[�Ƀo�b�t�@�𑗂�
*	@param startSlot �V�F�[�_�[�̊J�n�X���b�g
*	@param numBuffers ����o�b�t�@�̐�
*	@param ppBuffer ����o�b�t�@�̔z��
*	@return �Ȃ�
*/
void DrawPolygon::SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	// ���_�V�F�[�_�[�Ƀo�b�t�@�𑗂�
	m_pDeviceContext->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	// �W�I���g���V�F�[�_�[�Ƀo�b�t�@�𑗂�
	m_pDeviceContext->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	// �s�N�Z���V�F�[�_�[�Ƀo�b�t�@�𑗂�
	m_pDeviceContext->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
}
/*
*	@brief �V�F�[�_�[���Z�b�g����
*	@details �V�F�[�_�[���Z�b�g����
*	@param shaders �V�F�[�_�[�̍\����
*	@param ppClassInstances �N���X�C���X�^���X�̔z��
*	@param nubClassInstances �N���X�C���X�^���X�̐�
*	@return �Ȃ�
*/
void DrawPolygon::SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	// ���_�V�F�[�_�[���Z�b�g����
	if (shaders.vs.Get() != nullptr) m_pDeviceContext->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);
	// �W�I���g���V�F�[�_�[���Z�b�g����
	if (shaders.gs.Get() != nullptr) m_pDeviceContext->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances);
	// �s�N�Z���V�F�[�_�[���Z�b�g����
	if (shaders.ps.Get() != nullptr) m_pDeviceContext->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances);
}
/*
*	@brief �V�F�[�_�[���������
*	@details �V�F�[�_�[���������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawPolygon::ReleaseShader()
{
	// ���_�V�F�[�_�[�����
	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);
	// �W�I���g���V�F�[�_�[�����
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	// �s�N�Z���V�F�[�_�[�����
	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0);
}