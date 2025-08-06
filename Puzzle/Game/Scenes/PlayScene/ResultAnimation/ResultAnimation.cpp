/*
*	@file ResultAnimation.cpp
*	@brief ���ʃA�j���[�V�����N���X
*/
#include "pch.h"
#include "ResultAnimation.h"
/*
*	@brief	�C���v�b�g���C�A�E�g
*	@return �Ȃ�
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  ResultAnimation::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief �R���X�g���N�^
*	@details ���ʃA�j���[�V�����N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ResultAnimation::ResultAnimation()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_gameover(false) // �Q�[���I�[�o�[�t���O
	, m_gameclear(false) // �Q�[���N���A�t���O
	, m_animationEnable(false) // �A�j���[�V�����L���t���O
	, m_animationEnd(false) // �A�j���[�V�����I���t���O
	, m_decideTexture(true) // �摜����t���O
	, m_frameRows(1) // �摜�̍s��
	, m_frameCols(1) // �摜�̗�
	, m_pCreateShader(CreateShader::GetInstance()) // �V�F�[�_�[�쐬�N���X
	, m_pDrawPolygon(DrawPolygon::GetInstance()) // �|���S���`��N���X
	, m_time(0.0f) // ����
	, m_vertices{}// ���_�z��
	, m_pTextures{} // �e�N�X�`���z��
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���ʃA�j���[�V�����N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ResultAnimation::~ResultAnimation()
{
}
/*
*	@brief ������
*	@details ���ʃA�j���[�V�����N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void ResultAnimation::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �`��N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �V�F�[�_�[�쐬
	CreateShaders();
}
/*
*	@brief �X�V
*	@details ���ʃA�j���[�V�����N���X�̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ResultAnimation::Update(float elapsedTime)
{
	// �A�j���[�V�������I�����Ă���ꍇ�͉������Ȃ�
	if (m_animationEnd) return;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �e�摜��Y���W���X�V
	for (int i = 0; i < m_frameCols; ++i)m_positionsY[i] = m_gameclear ? POS_Y * Easing::EaseOutExpo(m_time) : POS_Y * Easing::RandomJitter(m_time);
	// 5�b�o������A�j���[�V�������I��
	if (m_time >= 3.0f)m_animationEnd = true;
}
/*
*	@brief �`��
*	@details ���ʃA�j���[�V�����N���X�̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ResultAnimation::Render()
{

	// �摜�`��
	for (int i = 0; i < m_frameCols; ++i)
	{
		DrawQuad(m_pTextures, m_vertices, m_positionX + (SIZE_X * i), m_positionsY[i], SIZE_X, SIZE_Y, i, m_frameCols, m_frameRows);
	}

}
/*
*	@brief �e�N�X�`������
*	@details ���ʃA�j���[�V�����N���X�Ŏg���e�N�X�`�������肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ResultAnimation::DecideTexture()
{
	// �Q�[���I�[�o�[���Q�[���N���A���Ńe�N�X�`����������
	std::string textureName = m_gameclear ? "GameClear" : "GameOver";
	// �e�N�X�`���̍s���Ɨ񐔂�ݒ�
	// �s����1
	m_frameRows = 1;
	// �񐔂̓Q�[���N���A�Ȃ�9�A�Q�[���I�[�o�[�Ȃ�8
	m_frameCols = m_gameclear ? 9 : 8;
	// �摜�̈ʒuX��ݒ�
	m_positionX = m_gameclear ? POS_X_CLEAR : POS_X_OVER;
	// �摜�̈ʒuY�̐���ݒ�
	m_positionsY.resize(m_frameCols);
	// �摜�̈ʒuY��ݒ�
	for (int i = 0; i < m_frameCols; ++i)m_positionsY[i] = POS_Y;
	// �}�l�[�W���[����e�N�X�`�����擾
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture(textureName));
	// ����ς݂ɂ���
	m_decideTexture = false;
}
/*
*	@brief �V�F�[�_�[�̍쐬
*	@details ���ʃA�j���[�V�����N���X�̃V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ResultAnimation::CreateShaders()
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

void ResultAnimation::DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture, DirectX::VertexPositionTexture* vertices, float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
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
/*
*	@brief ���ʂ�ݒ�
*	@details ���ʃA�j���[�V�����N���X�ɃQ�[���I�[�o�[���Q�[���N���A����ݒ肷��
*	@param gameover �Q�[���I�[�o�[���ǂ���
*	@param gameclear �Q�[���N���A���ǂ���
*	@return �Ȃ�
*/
void ResultAnimation::SetResult(bool gameover, bool gameclear)
{
	// �Q�[���I�[�o�[�t���O�ƃQ�[���N���A�t���O��ݒ�
	m_gameover = gameover;
	m_gameclear = gameclear;


	// �Q�[���I�[�o�[���Q�[���N���A�Ȃ�
	if (m_gameover || m_gameclear)
	{
		// �A�j���[�V������L���ɂ���
		m_animationEnable = true;
		// �e�N�X�`��������
		if (m_decideTexture)DecideTexture();
	}
	// �ǂ���ł��Ȃ��ꍇ�̓A�j���[�V�����𖳌��ɂ���
	else
	{
		m_animationEnable = false;
	}



}