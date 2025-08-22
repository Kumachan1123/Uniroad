/*
*	@file CreateShaders.h
*	@brief �V�F�[�_�[���쐬����V���O���g���N���X
*	@details �|���S����`�悷��ۂɎg�p����V�F�[�_�[���쐬����
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <utility>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/BinaryFile/BinaryFile.h"

// �V�F�[�_�[���쐬����V���O���g���N���X
class CreateShader
{
public:
	// �V���O���g���C���X�^���X
	// �V���O���g���C���X�^���X���擾
	static CreateShader* const GetInstance();
public:
	// public�֐�
	// �f�X�g���N�^
	~CreateShader();
	// ������
	void Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout);
	// �������i�f�o�C�X�̂݁j
	void Initialize(ID3D11Device1* device);
	// ���_�V�F�[�_���쐬
	void CreateVertexShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs);
	// �s�N�Z���V�F�[�_�[���쐬
	void CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);
	// �W�I���g���V�F�[�_�[���쐬
	void CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);
	// �R���s���[�g�V�F�[�_�[���쐬
	void CreateComputeShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11ComputeShader>& cs);
	// ������C���v�b�g���C�A�E�g��Ԃ�
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() { return m_pInputLayout; }
	// �R���X�^���g�o�b�t�@���쐬
	void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize);
private:
	// private�֐�
	// �R���X�g���N�^
	CreateShader();
	// �R�s�[�R���X�g���N�^
	CreateShader(const CreateShader&) = delete;
	// ������Z�q�̋֎~
	CreateShader& operator=(const CreateShader&) = delete;
private:
	// private�ϐ�
	// �V���O���g���C���X�^���X
	static std::unique_ptr<CreateShader> m_pInstance;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���̓��C�A�E�g
	const D3D11_INPUT_ELEMENT_DESC* m_pIED;
	// �v�f��
	UINT m_NumElements;
	// ���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};
