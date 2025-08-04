/*
*	@file	DrawPolygon.h
*	@brief	�|���S���`��p�N���X
*	@details �|���S����`�悷�邽�߂̃N���X
*/
#pragma once
#ifndef DRAW_POLYGON_DEFINED
#define DRAW_POLYGON_DEFINED
// �W�����C�u����
#include <vector>
// DirectX�̃w�b�_�t�@�C��
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// �O�����C�u����
#include "Game/CommonResources/CommonResources.h"
#include <DeviceResources.h>
#include <Libraries/Microsoft/DebugDraw.h>

// �O���錾
class CommonResources;

// �|���S���`��p�N���X
class DrawPolygon
{
public:
	// �\����
	// �O�̃V�F�[�_�[���ЂƂ܂Ƃ߂ɂ��đ��邽�߂̍\����
	struct Shaders
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;// ���_�V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;// �s�N�Z���V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> gs;// �W�I���g���V�F�[�_�[
	};
public:
	// �񋓌^
	// �T���v���[�X�e�[�g�̎��
	enum class SamplerStates
	{
		ANISOTROPIC_CLAMP,// �A�j�\�g���s�b�N�N�����v
		ANISOTROPIC_WRAP,// �A�j�\�g���s�b�N���b�v
		LINEAR_CLAMP,// ���j�A�N�����v
		LINEAR_WRAP,// ���j�A���b�v
		POINT_CLAMP,// �|�C���g�N�����v
		POINT_WRAP,// �|�C���g���b�v
	};
	// �u�����h�X�e�[�g�̎��
	enum class BlendStates
	{
		ALPHA,	// �A���t�@�u�����h
		ADDITIVE,	// ���Z�u�����h
		OPAQUE,	// �s����
		NONPREMULTIPLIED,	// ��v���}���`�v���C�h
	};
	// ���X�^���C�U�[�X�e�[�g�̎��
	enum class RasterizerStates
	{
		CULL_CLOCKWISE,// ���v���
		CULL_COUNTERCLOCKWISE,// �����v���
		CULL_NONE,// �J�����O�Ȃ�
		WIREFRAME,// ���C���[�t���[��
	};
	// �[�x�X�e���V���X�e�[�g�̎��
	enum class DepthStencilStates
	{
		DEPTH_DEFAULT,// �f�t�H���g
		DEPTH_NONE,// �[�x�Ȃ�
		DEPTH_READ,	// �[�x�ǂݎ��
		DEPTH_READ_REVERSE_Z,// �[�x�ǂݎ��i�tZ�j
		DEPTH_REVERSE_Z,// �tZ
	};

public:
	// �V���O���g���C���X�^���X
	// �V���O���g���C���X�^���X���擾
	static DrawPolygon* const GetInstance();
public:
	// public�֐�
	// �f�X�g���N�^
	~DrawPolygon();
	// �������i���_�A�e�N�X�`���j
	void InitializePositionTexture(DX::DeviceResources* pDR);
	// �������i���_�A�F�A�e�N�X�`���j
	void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// �`��J�n
	void DrawStart(ID3D11InputLayout* pInputLayout, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures);
	// �`��O�ݒ�
	void DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss);
	// �T�u���\�[�X�̍X�V
	void UpdateSubResources(ID3D11Resource* resource, const void* pSrcData);
	// �|���S���`��i���_�A�e�N�X�`���j
	void DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices);
	// �|���S���`��i���_�A�F�A�e�N�X�`���j
	void DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count);
	// ����i���_�A�e�N�X�`���j
	void ReleasePositionTexture();
	// ����i���_�A�F�A�e�N�X�`���j
	void ReleasePositionColorTexture();
	// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
	void SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer);
	// �V�F�[�_�[���Z�b�g����
	void SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances);
	// �V�F�[�_�[���������
	void ReleaseShader();
private:
	// private�֐�
	// �R���X�g���N�^
	DrawPolygon();
	// �R�s�[�R���X�g���N�^
	DrawPolygon(const DrawPolygon&) = delete;
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
	DrawPolygon& operator=(const DrawPolygon&) = delete;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	ID3D11DeviceContext1* m_pDeviceContext;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// �v���~�e�B�u�o�b�`�i���_�A�e�N�X�`���j
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatchTexture;
	// �v���~�e�B�u�o�b�`�i���_�A�F�A�e�N�X�`���j
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_pPrimitiveBatchColorTexture;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	// �V���O���g���C���X�^���X
	static std::unique_ptr<DrawPolygon> m_pInstance;
};
#endif // DRAW_POLYGON_DEFINED