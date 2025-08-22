/*
*	@file Bloom.h
*	@brief �u���[���G�t�F�N�g�̃w�b�_�t�@�C��
*	@details �֐����ĂԂ����Ńu���[���G�t�F�N�g�������ł���
*/
#pragma once
#ifndef BLOOM_DEFINED
#define BLOOM_DEFINED
// DirectX�̃w�b�_�t�@�C��
#include <PostProcess.h>
#include <CommonStates.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/Microsoft/RenderTexture/RenderTexture.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

//�O���錾
class CommonResources;

// �u���[���G�t�F�N�g�N���X
class Bloom
{
public:
	// �V���O���g���C���X�^���X
	// �V���O���g���C���X�^���X���擾
	static Bloom* const GetInstance();
public:
	// �\����
	// �u���[
	struct Blur
	{
		ID3D11RenderTargetView* RTV;// �����_�[�^�[�Q�b�g�r���[
		ID3D11ShaderResourceView* SRV;// �V�F�[�_�[���\�[�X�r���[
	};

public:
	// public�֐�
	// �f�X�g���N�^
	~Bloom();
	// �|�X�g�v���Z�X�𐶐�
	void CreatePostProcess(CommonResources* resources);
	// �I�t�X�N���[���`��p��RTV��؂�ւ���
	void ChangeOffScreenRT();
	// �|�X�g�v���Z�X�ɕK�v�Ȑݒ����������
	void PostProcess();
private:
	// private�֐�
	// �R���X�g���N�^
	Bloom();
	// �����_�[�e�N�X�`�����쐬����
	void CreateRenderTexture();
	// �R�s�[�R���X�g���N�^
	Bloom(const Bloom&) = delete;
	// ������Z�q�̋֎~
	Bloom& operator=(const Bloom&) = delete;
private:
	//private�ϐ�
	// �V���O���g���C���X�^���X
	static std::unique_ptr<Bloom> m_pInstance;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���ʃ��\�[�X
	ID3D11DeviceContext1* m_pDeviceContext;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// �u���[���G�t�F�N�g�̃R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	// �u���[1
	Blur m_blur1;
	// �u���[2
	Blur m_blur2;
	// �|�X�g�v���Z�X
	std::unique_ptr<DirectX::BasicPostProcess> m_pBasicPostProcess;
	// �f���A���|�X�g�v���Z�X
	std::unique_ptr<DirectX::DualPostProcess> m_pDualPostProcess;
	// �X�N���[���T�C�Y
	RECT m_screenSize;
	// �I�t�X�N���[���p�̃����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_pOffScreenRT;
	// �u���[1�p�̃����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_pBlur1RT;
	// �u���[2�p�̃����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_pBlur2RT;
	// �ۑ����ꂽ�I�t�X�N���[���p�̃����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_pSavedOffScreenRT;
	// �I�t�X�N���[���p�̃����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pOffScreenRTV;
	// �I�t�X�N���[���p�̃V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pOffScreenSRV;
	// �f�t�H���g�̐[�x�X�e���V���r���[
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDefaultDSV;
	// �f�t�H���g�̃����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pDefaultRTV;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
};
#endif //BLOOM_DEFINED
