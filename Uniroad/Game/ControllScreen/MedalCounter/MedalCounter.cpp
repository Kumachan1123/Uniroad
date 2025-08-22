/*
*	@file MedalCounter.cpp
*	@brief ���_���J�E���^�[�̏������`����N���X
*/
#include "pch.h"
#include "MedalCounter.h"
/*
*	@brief	�C���v�b�g���C�A�E�g
*	@return �Ȃ�
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  MedalCounter::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@details ���_���J�E���^�[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
MedalCounter::MedalCounter()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_collectedMedalCountSave(0)// ���W�������_���̐���ۑ�����ϐ�
	, m_viewportWidth(0)// �r���[�|�[�g�̕�
	, m_viewportHeight(0)// �r���[�|�[�g�̍���
	, m_frameRows{ 1 }//	�摜�̍s��
	, m_frameCols{ 10 }//	�摜�̗�
	, m_verticesMedal{} // ���_���̒��_�z��
	, m_verticesX{} // �u�~�v�̒��_�z��
	, m_verticesNumber1{} // 1�̈ʂ̐����̒��_�z��
	, m_verticesNumber10{} // 10�̈ʂ̐����̒��_�z��
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���_���J�E���^�[�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
MedalCounter::~MedalCounter()
{
	// ���_���J�E���^�[�̏I���������s��
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief �f�X�g���N�^
*	@details ���_���J�E���^�[�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MedalCounter::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �E�B���h�E�̕��ƍ�����ݒ�
	m_viewportWidth = width;
	m_viewportHeight = height;
	// ���_���摜
	m_pMedalTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Medal"));
	// �u�~�v�摜
	m_pXTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Multiply"));
	// �����摜
	m_pNumberTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Number"));
	// �`��N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �V�F�[�_�[�̍쐬
	CreateShaders();
}
/*
*	@brief �V�F�[�_�[�̍쐬
*	@details ���_���J�E���^�[�̃V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MedalCounter::CreateShaders()
{
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);
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
*	@brief �X�V����
*	@details ���_���J�E���^�[�̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MedalCounter::Update(float elapsedTime)
{
	// ���g�p�ϐ��̌x���𖳎�
	UNREFERENCED_PARAMETER(elapsedTime);
	// 10�̈ʂ̌v�Z
	m_collectedMedalCount.unit10 = m_collectedMedalCountSave / 10;
	// 1�̈ʂ̌v�Z
	m_collectedMedalCount.unit1 = m_collectedMedalCountSave % 10;
}

void MedalCounter::Render()
{
	// ���_���摜�`��
	DrawQuad(m_pMedalTextures, m_verticesMedal, MEDAL_POS_X, MEDAL_POS_Y, MEDAL_SIZE_X, MEDAL_SIZE_Y, 0, 1, 1);
	// �u�~�v�摜�`��
	DrawQuad(m_pXTextures, m_verticesX, X_POS_X, X_POS_Y, X_SIZE_X, X_SIZE_Y, 0, 1, 1);
	// 10�̈ʂ̐����摜�`��
	DrawQuad(m_pNumberTextures, m_verticesMedal, NUMBER10_POS_X, NUMBER10_POS_Y, NUMBER10_SIZE_X, NUMBER10_SIZE_Y, m_collectedMedalCount.unit10, m_frameCols, m_frameRows);
	// 1�̈ʂ̐����摜�`��
	DrawQuad(m_pNumberTextures, m_verticesMedal, NUMBER1_POS_X, NUMBER1_POS_Y, NUMBER1_SIZE_X, NUMBER1_SIZE_Y, m_collectedMedalCount.unit1, m_frameCols, m_frameRows);

}

void MedalCounter::DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_���W�̐ݒ�
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(0, 0)) };// ����
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(1, 0)) };// �E��
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(1, 1)) };// �E��
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(0, 1)) };// ����
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
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertices);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
