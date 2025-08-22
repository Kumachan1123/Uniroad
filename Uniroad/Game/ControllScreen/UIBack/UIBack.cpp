/*
*	@file UIBack.cpp
*	@brief �����ʂ̔w�i�N���X
*/
#include <pch.h>
#include "UIBack.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC>  UIBack::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details �����ʂ̔w�i�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
UIBack::UIBack(CommonResources* resources)
	: m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_time(0.0f)// �o�ߎ���
	, m_constBuffer()// �R���X�^���g�o�b�t�@
	, m_pCommonResources{ resources }// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	using namespace DirectX::SimpleMath;
	m_constBuffer.colors = Vector4(1.0f, 1.0f, 0.0f, 1.0f);// �F�̏�����
}
/*
*	@brief	�f�X�g���N�^
*	@details �����ʂ̔w�i�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
UIBack::~UIBack()
{
	/*do nothing*/
}

void UIBack::Create(DX::DeviceResources* pDR)
{
	// �f�o�C�X���\�[�X���Z�b�g
	m_pDR = pDR;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �}�l�[�W���[����e�N�X�`�����擾
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("UI_Back"));
}

void UIBack::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ���ԍX�V 
	m_time += elapsedTime;
}

void UIBack::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	���_��� 
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(-1.0f,  1.0f, 0.0f),  Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(1.0f,  1.0f, 0.0f),   Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(1.0, -1.0f, 0.0f),    Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(-1.0, -1.0f, 0.0f),   Vector2(0.0f, 1.0f)),// ����
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	// �r���[�s���]�u���Ċi�[
	m_constBuffer.matView = m_view.Transpose();
	// �v���W�F�N�V�����s���]�u���Ċi�[
	m_constBuffer.matProj = m_proj.Transpose();
	// ���[���h�s���]�u���Ċi�[
	m_constBuffer.matWorld = m_world.Transpose();
	// ���Ԃ��i�[
	m_constBuffer.time = Vector4(m_time);
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
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

void UIBack::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/UI/VS_UI.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/UI/PS_UI.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g�p���Ȃ��̂�nullptr���Z�b�g
	m_shaders.gs = nullptr;
}
