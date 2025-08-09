/*
*	@file Button.cpp
*	@brief �{�^���̑匳�ƂȂ�N���X
*/
#include "pch.h"
#include "Button.h"
// �C���v�b�g���C�A�E�g�̒�`
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Button::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief �R���X�g���N�^
*	@details �{�^���̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Button::Button()
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
*	@details �{�^���̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Button::~Button()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief ������
*	@details �{�^���̏��������s��
*	@param resources ���ʃ��\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Button::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �E�B���h�E�̕��ƍ�����ݒ�
	m_viewportWidth = width;
	m_viewportHeight = height;
	// �摜
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("SpeedUP"));
	// �`��N���X�̏�����
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �V�F�[�_�[�̍쐬
	CreateShaders();
}
/*
*	@brief �X�V
*	@details �{�^���̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Button::Update(const float elapsedTime)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
}

/*
*	@brief �V�F�[�_�[�̍쐬
*	@details �{�^���̃V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Button::CreateShaders()
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
*	@brief �摜��\��
*	@details �{�^���̉摜��\������
*	@param startX �J�nX���W
*	@param startY �J�nY���W
*	@param width �摜�̕�
*	@param height �摜�̍���
*	@param frameIndex �A�j���[�V�����̃R�}�ԍ�
*	@param frameCols �摜�̗�
*	@param frameRows �摜�̍s��
*	@return �Ȃ�
*/
void Button::DrawQuad(float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = height * aspect;
	// NDC���W�n�ɕϊ�
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + width, startY + correctedHeight));
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
/*
*	@brief �����蔻��
*	@details �}�E�X�̍��W�ƃ{�^���̋�`���r���ē����蔻����s��
*	@param mousePosition �}�E�X�̍��W
*	@param buttonRect �{�^���̋�`
*	@return �����蔻�肪�����true�A�Ȃ����false
*/
bool Button::Hit(const DirectX::SimpleMath::Vector2& mousePosition, const ButtonRect& buttonRect)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// ����E�E�����X�N���[�����W�֕ϊ�
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	// �E�B���h�E�n���h�����擾
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�T�C�Y�擾
	RECT rect;
	// �N���C�A���g�̈�T�C�Y���擾
	GetClientRect(hwnd, &rect);
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	float aspect = static_cast<float>(rect.right) / static_cast<float>(rect.bottom);
	float correctedHeight = buttonRect.size.y * aspect;
	// NDC���W�n�ɕϊ�
	Vector2 ndcLT = (Vector2(buttonRect.position.x, buttonRect.position.y));
	Vector2 ndcRB = (Vector2(buttonRect.position.x + buttonRect.size.x, buttonRect.position.y + correctedHeight));
	// ����ƉE���̍��W���X�N���[�����W�֕ϊ�
	Vector2 leftTop = Vector2(ndcLT.x * rect.right, ndcLT.y * rect.bottom);
	Vector2 rightBottom = Vector2(ndcRB.x * rect.right, ndcRB.y * rect.bottom);
	// �}�E�X�̍��W���摜�͈͓̔��ɂ���Ȃ�true��Ԃ�
	if (leftTop.x <= mousePosition.x && mousePosition.x <= rightBottom.x && leftTop.y <= mousePosition.y && mousePosition.y <= rightBottom.y)
		return true;
	// �����蔻��Ȃ��Ȃ�false��Ԃ�
	return false;
}
