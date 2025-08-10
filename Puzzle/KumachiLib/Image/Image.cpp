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
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
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
/*
*	@brief �摜��\��
*	@details �摜��\������
*	@param buttonRect �{�^���̋�`
*	@param frameIndex �t���[���̃C���f�b�N�X
*	@param frameCols �t���[���̗�
*	@param frameRows �t���[���̍s��
*	@return �Ȃ�
*/
void Image::DrawQuad(const Rect& buttonRect, int frameIndex, int frameCols, int frameRows)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = buttonRect.size.y * aspect;
	// �{�^���̋�`��ݒ�
	float startX = buttonRect.position.x - buttonRect.size.x / 2.0f;
	float startY = buttonRect.position.y - correctedHeight / 2.0f;
	// NDC���W�n�ɕϊ�
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + buttonRect.size.x, startY + correctedHeight));
	// ���_���W�̐ݒ�
	VertexPositionTexture vertices[VERTEX_COUNT]{};
	vertices[0] = { VertexPositionTexture(Vector3(ndcLT.x, ndcLT.y, 0), Vector2(0, 0)) };// ����
	vertices[1] = { VertexPositionTexture(Vector3(ndcRB.x, ndcLT.y, 0), Vector2(1, 0)) };// �E��
	vertices[2] = { VertexPositionTexture(Vector3(ndcRB.x, ndcRB.y, 0), Vector2(1, 1)) };// �E��
	vertices[3] = { VertexPositionTexture(Vector3(ndcLT.x, ndcRB.y, 0), Vector2(0, 1)) };// ����
	// �R���X�^���g�o�b�t�@�ɓn���f�[�^��ݒ�
	// ���[���h�s���P�ʍs��ɐݒ�
	m_constBuffer.matWorld = Matrix::Identity;
	// �r���[�s���P�ʍs��ɐݒ�
	m_constBuffer.matView = Matrix::Identity;
	// �v���W�F�N�V�����s���P�ʍs��ɐݒ�
	m_constBuffer.matProj = Matrix::Identity;
	// �A�j���[�V�����̃R�}��ݒ�
	m_constBuffer.count = Vector4((float)(frameIndex));
	// ������ݒ�
	m_constBuffer.height = Vector4((float)(frameRows));
	// ����ݒ�
	m_constBuffer.width = Vector4((float)(frameCols));
	// �󂯓n���p�o�b�t�@�̓��e�X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �f�v�X�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertices);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

