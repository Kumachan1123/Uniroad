/*
*	@file CreateShaders.cpp
*	@brief �V�F�[�_�[���쐬����V���O���g���N���X
*	@details �|���S����`�悷��ۂɎg�p����V�F�[�_�[���쐬����
*/
#include <pch.h>
#include "CreateShader.h"
using namespace KumachiLib;
// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<CreateShader> CreateShader::m_pInstance = nullptr;
/*
*	@brief �V���O���g���C���X�^���X���擾
*	@details CreateShader�N���X�̃V���O���g���C���X�^���X���擾����
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
CreateShader* const CreateShader::GetInstance()
{
	// �C���X�^���X���Ȃ��ꍇ
	if (m_pInstance == nullptr)
	{
		// �C���X�^���X�𐶐�
		m_pInstance.reset(new CreateShader());
	}
	// �C���X�^���X��Ԃ�
	return m_pInstance.get();
}

/*
*	@brief �R���X�g���N�^
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
CreateShader::CreateShader()
	: m_pDevice(nullptr)// �f�o�C�X
	, m_pIED(nullptr)// �C���v�b�g�G�������g�f�B�X�N���v�^
	, m_NumElements(0)// �v�f��
	, m_pInputLayout(nullptr)// ���C�A�E�g
{
}
/*
*	@brief �f�X�g���N�^
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
CreateShader::~CreateShader()
{
	// �C���v�b�g���C�A�E�g�̉��
	m_pInputLayout.Reset();
	// �C���v�b�g�G�������g�f�B�X�N���v�^�̉��
	m_pIED = nullptr;
	// �f�o�C�X�̉��
	m_pDevice = nullptr;
}
/*
*	@brief ������
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param device �f�o�C�X
*	@param pIED  �C���v�b�g�G�������g�f�B�X�N���v�^
*	@param NumElements �v�f��
*	@param InputLayout ���C�A�E�g
*	@return �Ȃ�
*/
void CreateShader::Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIED, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout)
{
	// �f�o�C�X��ݒ�
	m_pDevice = device;
	// �C���v�b�g�G�������g�f�B�X�N���v�^��ݒ�
	m_pIED = pIED;
	// �v�f����ݒ�
	m_NumElements = NumElements;
	// �C���v�b�g���C�A�E�g
	m_pInputLayout = InputLayout;
}
/*
*	@brief �������i�f�o�C�X�̂݁j
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param device �f�o�C�X
*	@return �Ȃ�
*/
void CreateShader::Initialize(ID3D11Device1* device)
{
	// �f�o�C�X��ݒ�
	m_pDevice = device;
}

/*
*	@brief ���_�V�F�[�_�[���쐬
*	@details ���_�V�F�[�_�[���쐬����
*	@param fileName �V�F�[�_�[�̃t�@�C����
*	@param vs ���_�V�F�[�_�[�̊i�[��
* 	@return �Ȃ�
*/
void CreateShader::CreateVertexShader(const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs)
{

	// �o�C�i���t�@�C����ǂݍ���
	BinaryFile VS = BinaryFile::LoadFile(fileName);
	//�V�F�[�_�[���쐬
	if (FAILED(m_pDevice->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))
	{
		// �G���[����
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		// �I��
		return;
	}
	// ���̓��C�A�E�g���쐬
	m_pDevice->CreateInputLayout(&m_pIED[0], m_NumElements, VS.GetData(), VS.GetSize(), m_pInputLayout.GetAddressOf());
}
/*
*	@brief �s�N�Z���V�F�[�_�[���쐬
*	@details �s�N�Z���V�F�[�_�[���쐬����
*	@param fileName �V�F�[�_�[�̃t�@�C����
*	@param ps �s�N�Z���V�F�[�_�[�̊i�[��
*	@return �Ȃ�
*/
void CreateShader::CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	// �o�C�i���t�@�C����ǂݍ���
	BinaryFile PS = BinaryFile::LoadFile(fileName);
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(m_pDevice->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))
	{
		// �G���[����
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		// �I��
		return;
	}
}
/*
*	@brief �W�I���g���V�F�[�_�[���쐬
*	@details �W�I���g���V�F�[�_�[���쐬����
*	@param fileName �V�F�[�_�[�̃t�@�C����
*	@param gs �W�I���g���V�F�[�_�[�̊i�[��
*	@return �Ȃ�
*/
void CreateShader::CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	// �o�C�i���t�@�C����ǂݍ���
	BinaryFile GS = BinaryFile::LoadFile(fileName);
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(m_pDevice->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))
	{
		// �G���[����
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		// �I��
		return;
	}
}
/*
*	@brief �R���X�^���g�o�b�t�@���쐬
*	@details �R���X�^���g�o�b�t�@���쐬����
*	@param cBuffer �R���X�^���g�o�b�t�@�̊i�[��
*	@param bufferSize �o�b�t�@�̃T�C�Y
*	@return �Ȃ�
*/
void CreateShader::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize)
{
	// �o�b�t�@�̐���
	D3D11_BUFFER_DESC bd;
	// �o�b�t�@�̐�����������
	ZeroMemory(&bd, sizeof(bd));
	// �g�p�@
	bd.Usage = D3D11_USAGE_DEFAULT;
	// �o�b�t�@�̃T�C�Y
	bd.ByteWidth = bufferSize;
	// �o�C���h�t���O
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPU�A�N�Z�X�t���O
	bd.CPUAccessFlags = 0;
	// �o�b�t�@���쐬
	m_pDevice->CreateBuffer(&bd, nullptr, &cBuffer);
}

