#pragma once
// / <summary>
// / �V�F�[�_�[���쐬����N���X
// / </summary>
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include <memory>

class CreateShader
{
public:
	// �V���O���g���C���X�^���X
	static CreateShader* const GetInstance();


public:// �֐�

	// �f�X�g���N�^
	~CreateShader();
	// ������
	void Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout);
	// ���_�V�F�[�_���쐬
	void CreateVertexShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs);
	// �s�N�Z���V�F�[�_�[���쐬
	void CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);
	// �W�I���g���V�F�[�_�[���쐬
	void CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);
	// ������C���v�b�g���C�A�E�g��Ԃ�
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() { return m_pInputLayout; }
	// �R���X�^���g�o�b�t�@���쐬
	void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize);
private:
	// �R���X�g���N�^
	CreateShader();
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
	CreateShader(const CreateShader&) = delete;
	CreateShader& operator=(const CreateShader&) = delete;
	// �V���O���g���C���X�^���X
	static std::unique_ptr<CreateShader> m_instance;
	// �f�o�C�X
	ID3D11Device1* m_device;
	// ���̓��C�A�E�g
	const D3D11_INPUT_ELEMENT_DESC* m_pIDE;
	// �v�f��
	UINT m_NumElements;
	// ���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};



