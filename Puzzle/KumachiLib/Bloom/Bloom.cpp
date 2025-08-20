/*
*	@file Bloom.cpp
*	@brief �u���[���G�t�F�N�g�̎����t�@�C��
*	@details �֐����ĂԂ����Ńu���[���G�t�F�N�g�������ł���
*/
#include <pch.h>
#include "Bloom.h"

// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<Bloom> Bloom::m_pInstance = nullptr;
/*
*	@brief �V���O���g���C���X�^���X���擾
*	@details Bloom�N���X�̃V���O���g���C���X�^���X���擾����
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
Bloom* const Bloom::GetInstance()
{
	// �C���X�^���X���Ȃ��ꍇ
	if (m_pInstance == nullptr)
	{
		// �C���X�^���X�𐶐�
		m_pInstance.reset(new Bloom());
	}
	// �C���X�^���X��Ԃ�
	return m_pInstance.get();
}
/*
*	@brief �R���X�g���N�^
*	@details �u���[���G�t�F�N�g�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Bloom::Bloom()
	: m_pDeviceContext{}// �f�o�C�X�R���e�L�X�g
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pDevice{}// �f�o�C�X
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_blur1{}// �u���[1
	, m_blur2{}// �u���[2
	, m_pBasicPostProcess{}// �|�X�g�v���Z�X
	, m_pDualPostProcess{}// �f���A���|�X�g�v���Z�X
	, m_screenSize{}// �X�N���[���T�C�Y
	, m_pStates{}// �R�����X�e�[�g
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e��|�C���^�[�̉��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Bloom::~Bloom()
{
	// ���ʃ��\�[�X�̉��
	m_pCommonResources = nullptr;
	// �f�o�C�X�R���e�L�X�g�̉��
	m_pDeviceContext = nullptr;
	// �f�o�C�X���\�[�X�̉��
	m_pDR = nullptr;
	// �f�o�C�X�̉��
	m_pDevice = nullptr;
}
/*
*	@brief �|�X�g�v���Z�X�𐶐�
*	@details �u���[���G�t�F�N�g���������邽�߂̃|�X�g�v���Z�X�𐶐�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Bloom::CreatePostProcess(CommonResources* resources)
{
	using namespace DirectX;
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �f�o�C�X���擾
	m_pDevice = m_pDR->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	m_pDeviceContext = m_pDR->GetD3DDeviceContext();
	// �����_�[�e�N�X�`�����쐬
	this->CreateRenderTexture();
	// �|�X�g�v���Z�X�𐶐�
	m_pBasicPostProcess = std::make_unique<BasicPostProcess>(m_pDevice);
	// �f���A���|�X�g�v���Z�X�𐶐�
	m_pDualPostProcess = std::make_unique<DualPostProcess>(m_pDevice);
}
/*
*	@brief �I�t�X�N���[���`��p��RTV��؂�ւ���
*	@details �u���[���G�t�F�N�g���������邽�߂ɁA�I�t�X�N���[���`��p�̃����_�[�e�N�X�`����ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Bloom::ChangeOffScreenRT()
{
	using namespace DirectX;
	// �I�t�X�N���[���p��RTV���擾
	m_pOffScreenRTV = m_pOffScreenRT->GetRenderTargetView();
	// �f�t�H���g��DSV���擾
	m_pDefaultDSV = m_pCommonResources->GetDeviceResources()->GetDepthStencilView();
	// RTV��DSV���o�C���h
	m_pDeviceContext->OMSetRenderTargets(1, m_pOffScreenRTV.GetAddressOf(), m_pDefaultDSV.Get());
	// RTV���N���A
	m_pDeviceContext->ClearRenderTargetView(m_pOffScreenRTV.Get(), Colors::Black);
	// DSV���N���A
	m_pDeviceContext->ClearDepthStencilView(m_pDefaultDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
/*
*	@brief �|�X�g�v���Z�X�ɕK�v�Ȑݒ����������
*	@details �u���[���G�t�F�N�g���������邽�߂̃|�X�g�v���Z�X�����s����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Bloom::PostProcess()
{
	using namespace DirectX;
	// �u���[1�p��RTV���擾
	m_blur1.RTV = m_pBlur1RT->GetRenderTargetView();
	//	�u���[1�p��SRV���擾
	m_blur1.SRV = m_pBlur1RT->GetShaderResourceView();
	// �u���[2�p��RTV���擾
	m_blur2.RTV = m_pBlur2RT->GetRenderTargetView();
	//	�u���[2�p��SRV���擾
	m_blur2.SRV = m_pBlur2RT->GetShaderResourceView();
	// �I�t�X�N���[���p��SRV���擾
	m_pOffScreenSRV = m_pOffScreenRT->GetShaderResourceView();
	// �V�F�[�_�[���\�[�X�r���[�̉���
	ID3D11ShaderResourceView* nullsrv[] = { nullptr,nullptr };
	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = { 0.0f,0.0f,m_screenSize.right / 2.0f,m_screenSize.bottom / 2.0f,0.0f,1.0f };
	// �r���[�|�[�g��ݒ�
	m_pDeviceContext->RSSetViewports(1, &vp);
	// -----------------------------------------------------
	//	Pass1::offscreen -> blur1 �� ���邢�����𒊏o����
	// -----------------------------------------------------
	// �����_�[�^�[�Q�b�g��ݒ�
	m_pDeviceContext->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);
	// �G�t�F�N�g��ݒ�
	m_pBasicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
	// 臒l��ݒ�
	m_pBasicPostProcess->SetBloomExtractParameter(0.35f);
	// �I�t�X�N���[���p��SRV��ݒ�
	m_pBasicPostProcess->SetSourceTexture(m_pOffScreenSRV.Get());
	// �|�X�g�v���Z�X�����s
	m_pBasicPostProcess->Process(m_pDeviceContext);
	// -----------------------------------------------------
	//	Pass2::blur1 -> blur2 �� ���ɂڂ���
	// -----------------------------------------------------
	// �����_�[�^�[�Q�b�g��ݒ�
	m_pDeviceContext->OMSetRenderTargets(1, &m_blur2.RTV, nullptr);
	// �G�t�F�N�g��ݒ�
	m_pBasicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	// ���ڂ����̃p�����[�^��ݒ�
	m_pBasicPostProcess->SetBloomBlurParameters(true, 1.250f, 1.025f);
	// �u���[1�p��SRV��ݒ�
	m_pBasicPostProcess->SetSourceTexture(m_blur1.SRV);
	// �|�X�g�v���Z�X�����s
	m_pBasicPostProcess->Process(m_pDeviceContext);
	// �V�F�[�_�[���\�[�X�r���[������
	m_pDeviceContext->PSSetShaderResources(0, 1, nullsrv);
	// -----------------------------------------------------
	//	Pass3::blur2 -> blur1 �� �c�ɂڂ���
	// -----------------------------------------------------
	// �����_�[�^�[�Q�b�g��ݒ�
	m_pDeviceContext->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);
	// �G�t�F�N�g��ݒ�
	m_pBasicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	// �c�ڂ����̃p�����[�^��ݒ�
	m_pBasicPostProcess->SetBloomBlurParameters(false, 1.250f, 1.025f);
	// �u���[2�p��SRV��ݒ�
	m_pBasicPostProcess->SetSourceTexture(m_blur2.SRV);
	// �|�X�g�v���Z�X�����s
	m_pBasicPostProcess->Process(m_pDeviceContext);
	// -----------------------------------------------------
	// Pass4::offscreen -> blur1 �� �t���[���o�b�t�@�ɕ`�悷��
	// -----------------------------------------------------
	// �f�t�H���g��RTV���擾
	m_pDefaultRTV = m_pCommonResources->GetDeviceResources()->GetRenderTargetView();
	// �����_�[�^�[�Q�b�g��ݒ�
	m_pDeviceContext->OMSetRenderTargets(1, m_pDefaultRTV.GetAddressOf(), nullptr);
	// �f�t�H���g�̃r���[�|�[�g���擾
	const auto& defaultVP = m_pCommonResources->GetDeviceResources()->GetScreenViewport();
	// �r���[�|�[�g��ݒ�
	m_pDeviceContext->RSSetViewports(1, &defaultVP);
	// �G�t�F�N�g��ݒ�
	m_pDualPostProcess->SetEffect(DualPostProcess::BloomCombine);
	// �u���[���̃p�����[�^��ݒ�
	m_pDualPostProcess->SetBloomCombineParameters(1.125f, 1.0f, 1.0f, 1.0f);
	// �I�t�X�N���[���p��SRV��ݒ�
	m_pDualPostProcess->SetSourceTexture(m_pOffScreenSRV.Get());
	// �u���[1�p��SRV��ݒ�
	m_pDualPostProcess->SetSourceTexture2(m_blur1.SRV);
	// �|�X�g�v���Z�X�����s
	m_pDualPostProcess->Process(m_pDeviceContext);
	// �V�F�[�_�[���\�[�X�r���[������
	m_pDeviceContext->PSSetShaderResources(0, 2, nullsrv);
	// �����_�[�^�[�Q�b�g�����ɖ߂�
	m_pDeviceContext->OMSetRenderTargets(1, m_pDefaultRTV.GetAddressOf(), m_pDefaultDSV.Get());
}
/*
*	@brief �����_�[�e�N�X�`�����쐬����
*	@details �u���[���G�t�F�N�g���������邽�߂̃����_�[�e�N�X�`�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Bloom::CreateRenderTexture()
{
	// �X�N���[���T�C�Y���擾
	m_screenSize = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// �����̃T�C�Y���擾
	RECT halfSize{ 0,0, m_screenSize.right / 2, m_screenSize.bottom / 2 };
	// �I�t�X�N���[���p��RT���쐬
	m_pOffScreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	// �f�o�C�X��ݒ�
	m_pOffScreenRT->SetDevice(m_pDevice);
	// �E�B���h�E�T�C�Y��ݒ�
	m_pOffScreenRT->SetWindow(m_screenSize);
	// �u���[1�p��RT���쐬
	m_pBlur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	// �f�o�C�X��ݒ�
	m_pBlur1RT->SetDevice(m_pDevice);
	// �E�B���h�E�T�C�Y��ݒ�
	m_pBlur1RT->SetWindow(halfSize);
	// �u���[2�p��RT���쐬
	m_pBlur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	// �f�o�C�X��ݒ�
	m_pBlur2RT->SetDevice(m_pDevice);
	// �E�B���h�E�T�C�Y��ݒ�
	m_pBlur2RT->SetWindow(halfSize);
}