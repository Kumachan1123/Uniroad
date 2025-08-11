/*
*	@file Image.cpp
*	@brief �摜�N���X
*/
#include "pch.h"
#include "Image.h"
// �C���v�b�g���C�A�E�g�̒�`
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Image::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@details �摜�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Image::Image()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_viewportWidth(0) // �r���[�|�[�g�̕�
	, m_viewportHeight(0) // �r���[�|�[�g�̍���
	, m_pCreateShader(CreateShader::GetInstance()) // �V�F�[�_�[�쐬�N���X
	, m_pDrawPolygon(DrawPolygon::GetInstance()) // �`��N���X
	, m_pVertexShader(nullptr) // ���_�V�F�[�_
	, m_pPixelShader(nullptr) // �s�N�Z���V�F�[�_
	, m_shaders{} // �V�F�[�_�[�̍\����
	, m_vertices{} // ���_�z��
	, m_constBuffer{} // �R���X�^���g�o�b�t�@
{
}
/*
*	@brief �f�X�g���N�^
*	@details �摜�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Image::~Image()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief ������
*	@details �摜�N���X�̏��������s��
*	@param resources ���ʃ��\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Image::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �E�B���h�E�̕��ƍ�����ݒ�
	m_viewportWidth = width;
	m_viewportHeight = height;
	// �`��N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �V�F�[�_�[�̍쐬
	CreateShaders();
}
/*
*	@brief �V�F�[�_�[�̍쐬
*	@details �摜�̃V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Image::CreateShaders()
{
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ������ϊ�
	std::wstring wpathVS = std::wstring(m_vertexShaderFilePath.begin(), m_vertexShaderFilePath.end());
	std::wstring wpathPS = std::wstring(m_pixelShaderFilePath.begin(), m_pixelShaderFilePath.end());
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(wpathVS.c_str(), m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(wpathPS.c_str(), m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, m_bufferSize);
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[���Z�b�g�i�g��Ȃ��̂�nullptr�j
	m_shaders.gs = nullptr;
}

/*
*	@brief �X�V
*	@details �摜�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Image::Update(const float elapsedTime)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
	// �������Ȃ�
}
