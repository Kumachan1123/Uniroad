/*
*	@file Shadow.h
*	@brief �e�|���S����`�悷��N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

// �O���錾
class CommonResources;

// �e�|���S����`�悷��N���X
class Shadow
{
public:
	// public�֐�
	// �R���X�g���N�^
	Shadow();
	// �f�X�g���N�^
	~Shadow() = default;
	// ����������
	void Initialize(CommonResources* resources);
	// �`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::SimpleMath::Vector3& position,
		float radius = 0.7f);
private:
	// private�֐�
	// �[�x�X�e���V���o�b�t�@��ݒ肷��
	void CreateDepthStenciBuffer(ID3D11Device* pDevice);
private:
	// private�ϐ�
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatch;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �[�x�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;

};