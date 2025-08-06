/*
*	@file Canvas.cpp
*	@brief �L�����o�X�N���X
*/
#include <pch.h>
#include "Canvas.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> Canvas::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief	�R���X�g���N�^
*	@details �L�����o�X�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Canvas::Canvas(CommonResources* pCommonResources)
	: m_pCommonResources(pCommonResources)
	, m_pDR(nullptr)
	, m_textureWidth(0)
	, m_textureHeight(0)
	, m_windowWidth(0)
	, m_windowHeight(0)
	, m_position(0.0f, 0.0f)
	, m_scale(1.0f, 1.0f)
	, m_baseScale(1.0f, 1.0f)
	, m_anchor(KumachiLib::ANCHOR::TOP_LEFT)
	, m_shaderType(ShaderType::OTHER)
	, m_pTexture(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pGeometryShader(nullptr)
	, m_pInputLayout(nullptr)
	, m_pCBuffer(nullptr)
	, m_pDrawPolygon(DrawPolygon::GetInstance())
	, m_pCreateShader(CreateShader::GetInstance())
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
Canvas::~Canvas(/*do nothing*/)
{
}
/*
*	@brief	�e�N�X�`���̓ǂݍ���
*	@details �e�N�X�`���̓ǂݍ���
*	@param key �e�N�X�`���̃L�[
*	@return �Ȃ�
*/
void Canvas::LoadTexture(std::string key)
{
	// �e�N�X�`���}�l�[�W���[����e�N�X�`�����擾
	m_pTexture = m_pCommonResources->GetTextureManager()->GetTexture(key);
	// �T�C�Y�擾�̂��߂̏���
	// �ꎞ�ۑ��p���\�[�X�n���h��
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	// �e�N�X�`�����烊�\�[�X���擾
	m_pTexture->GetResource(resource.GetAddressOf());
	// �ꎞ�ۑ��p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	// ���\�[�X���e�N�X�`���ɕϊ�
	DX::ThrowIfFailed(resource.As(&texture2D));
	// �e�N�X�`���̏����擾
	// �e�N�X�`���̏����i�[����\����
	D3D11_TEXTURE2D_DESC desc;
	// �e�N�X�`���̏����擾
	texture2D->GetDesc(&desc);
	// �e�N�X�`����z��ɒǉ�
	m_pTextures.push_back(m_pTexture.Get());
	// �e�N�X�`���̕����擾
	m_textureWidth = desc.Width;
	// �e�N�X�`���̍������擾
	m_textureHeight = desc.Height;
}
/*
*	@brief	UI�̐���
*	@details UI�̐���
*	@param pDR �f�o�C�X���\�[�X
*	@param key �e�N�X�`���̃L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void Canvas::Create(DX::DeviceResources* pDR, const std::string& key, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor)
{
	// �f�o�C�X���\�[�X
	m_pDR = pDR;
	// �ʒu
	m_position = position;
	// �X�P�[��
	m_baseScale = m_scale = scale;
	// �A���J�[
	m_anchor = anchor;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �e�N�X�`����ǂݍ���
	LoadTexture(key);
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief	�`��֐�
*	@details �|���S����`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Canvas::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_���
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))// �傫���ƃA���J�[
		, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))// �ʒu�ƕ��ƍ���
		, Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))// �E�B���h�E�̕��ƍ���
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �E�B���h�E�T�C�Y��ݒ�
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	// ���Ԃ�ݒ�
	m_constBuffer.time = m_time;
	// �F��ݒ�
	m_constBuffer.color = Vector3(0.5, 0.5, 0.5);
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
	// �`��J�n
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	//	�V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();


}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@details �e��V�F�[�_�[���쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Canvas::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Tile/VS_Tile.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Tile/PS_Tile.cso", m_pPixelShader);
	// �W�I���g���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Tile/GS_Tile.cso", m_pGeometryShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[��n��
	m_shaders.gs = m_pGeometryShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[��n��
	m_shaders.ps = m_pPixelShader.Get();
}
/*
*	@brief	�E�B���h�E�̃T�C�Y��ݒ�
*	@details �E�B���h�E�̃T�C�Y��ݒ�
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Canvas::SetWindowSize(const int& width, const int& height)
{
	// �E�B���h�E�̕�
	m_windowWidth = width;
	// �E�B���h�E�̍���
	m_windowHeight = height;
}
/*
*	@brief	�����蔻��
*	@details �}�E�X�̍��W��UI�͈͓̔��ɂ��邩�𔻒肷��
*	@param pos �}�E�X�̍��W
*	@return true:�����蔻�肠��, false:�����蔻��Ȃ�
*/
bool Canvas::IsHit(const DirectX::SimpleMath::Vector2& pos) const
{
	using namespace DirectX::SimpleMath;
	// �摜�̍���̍��W���擾
	Vector2 leftTop = m_position - Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.x / 2;
	// �摜�̉E���̍��W���擾
	Vector2 rightBottom = m_position + Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.y / 2;
	// �}�E�X�̍��W���摜�͈͓̔��ɂ���Ȃ�true��Ԃ�
	if (leftTop.x <= pos.x && pos.x <= rightBottom.x && leftTop.y <= pos.y && pos.y <= rightBottom.y)
		return true;
	// �����蔻��Ȃ��Ȃ�false��Ԃ�
	return false;
}