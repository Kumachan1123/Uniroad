/*
*	@file ShadowMapLight.h
*	@brief �V���h�E�}�b�v�p���C�g�N���X
*/
#pragma once
// �O�����C�u����
#include "Libraries/Microsoft/ReadData.h"
#include "Libraries/Microsoft/RenderTexture/RenderTexture.h"
#include "Libraries/MyLib/DepthStencil.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

// �O���錾
class CommonResources;
namespace DX
{
	class RenderTexture;
}
// �V���h�E�}�b�v�p���C�g�N���X
class ShadowMapLight
{
private:
	// ���C�g�̒萔�o�b�t�@�p�\����
	struct cbLight
	{
		DirectX::XMMATRIX lightViewProjection;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR lightPosition;		// ���C�g�̈ʒu
		DirectX::XMVECTOR lightDirection;		// ���C�g�̕���
		DirectX::XMVECTOR lightAmbient;			// ���C�g�̊���
	};
public:
	// �A�N�Z�T
	// ���f���ƃ��[���h�s��̃y�A��ݒ�
	void SetShadowModel(DirectX::Model* model, const  DirectX::SimpleMath::Matrix& world) { m_pShadowInfos.push_back(std::make_pair(model, world)); }
	// ���C�g�̍��W��ݒ�
	void SetLightPosition(const DirectX::SimpleMath::Vector3& position) { m_lightPosition = position; }
	// �r���[�|�[�g��ݒ肷��
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }


public:
	// public�֐�
	// �R���X�g���N�^
	ShadowMapLight(CommonResources* commonResources);
	// �f�X�g���N�^
	~ShadowMapLight();
	// �X�V
	void Update(float elapsedTime);
	// �e�ɂȂ郂�f����`��
	void RenderShadow();
	// ���f���`�掞�̃����_�����ŌĂԏ������܂Ƃ߂��֐�
	void ApplyShader(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states);
private:
	// private�֐�
	// �V�F�[�_�����[�h����
	void LoadShader(ID3D11Device* device);
	// �o�b�t�@���쐬����
	void CreateConstanBuffer(ID3D11Device* device);
	// �V���h�E�}�b�v�p�̊e��I�u�W�F�N�g���쐬����
	void CreateShadowMapObject(ID3D11Device* device);
private:
	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// Direct3D�f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
	// ���C�g�̍��W
	DirectX::SimpleMath::Vector3 m_lightPosition;
	// ���C�g�̉�]
	DirectX::SimpleMath::Quaternion m_lightQuaternion;
	// ���C�g�̉�p�F���C�g���猩�����i���f���J�����̉�p
	float m_lightTheta;
	// �萔�o�b�t�@�p�̃o�b�t�@�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	std::unique_ptr<DX::RenderTexture> m_pShadowMapRT;
	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	std::unique_ptr<mylib::DepthStencil> m_pShadowMapDS;
	// ���_�V�F�[�_(�V���h�E�}�b�v�p)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVSShadowMap;
	// �s�N�Z���V�F�[�_(�V���h�E�}�b�v�p)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPSShadowMap;
	// �T���v��
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pShadowMapSampler;
	// �e�ɂ��������f���ƃ��[���h�s��̃y�A�z��
	std::vector<std::pair<DirectX::Model*, DirectX::SimpleMath::Matrix>> m_pShadowInfos;
	// �V�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_pSRV;
	// �ʏ�`��p�r���[�|�[�g
	D3D11_VIEWPORT m_viewport;
};