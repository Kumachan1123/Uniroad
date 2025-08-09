/*
*	@file SpeedUpUI.cpp
*	@brief �X�s�[�h�A�b�v�{�^���N���X
*/
#include "pch.h"
#include "SpeedUpUI.h"
// �C���v�b�g���C�A�E�g�̒�`
const std::vector<D3D11_INPUT_ELEMENT_DESC>  SpeedUpUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
// �\���ʒu�i����j�̒�`
const DirectX::SimpleMath::Vector2 SpeedUpUI::POSITION = DirectX::SimpleMath::Vector2(0.625f, 0.0f);
// �\���T�C�Y�̒�`
const DirectX::SimpleMath::Vector2 SpeedUpUI::SIZE = DirectX::SimpleMath::Vector2(0.07f, 0.07f);

/*
*	@brief �R���X�g���N�^
*	@details �X�s�[�h�A�b�vUI�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
SpeedUpUI::SpeedUpUI()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_viewportWidth(0) // �r���[�|�[�g�̕�
	, m_viewportHeight(0) // �r���[�|�[�g�̍���
	, m_pCreateShader(CreateShader::GetInstance()) // �V�F�[�_�[�쐬�N���X
	, m_pDrawPolygon(DrawPolygon::GetInstance()) // �`��N���X
	, m_isHit(false) // �����蔻��t���O
	, m_isPressed(false) // �{�^���������ꂽ�t���O
	, m_frameRows(2) // �摜�̍s��
	, m_frameCols(1) // �摜�̗�
	, m_pVertexShader(nullptr) // ���_�V�F�[�_
	, m_pPixelShader(nullptr) // �s�N�Z���V�F�[�_
	, m_shaders{} // �V�F�[�_�[�̍\����
	, m_vertices{} // ���_�z��
	, m_constBuffer{} // �R���X�^���g�o�b�t�@

{
}
/*
*	@brief �f�X�g���N�^
*	@details �X�s�[�h�A�b�vUI�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
SpeedUpUI::~SpeedUpUI()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief ������
*	@details �X�s�[�h�A�b�vUI�̏��������s��
*	@param resources ���ʃ��\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void SpeedUpUI::Initialize(CommonResources* resources, int width, int height)
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
*	@brief �V�F�[�_�[�̍쐬
*	@details �X�s�[�h�A�b�vUI�̃V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpeedUpUI::CreateShaders()
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
*	@brief �X�V
*	@details �X�s�[�h�A�b�vUI�̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void SpeedUpUI::Update(float elapsedTime)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �����蔻����s��
	m_isHit = Hit(mousePos);
	// �����蔻�肪����ꍇ�A�}�E�X�̍��{�^����������Ă����牟���ꂽ��Ԃ��g�O��
	if (m_isHit && MouseClick::IsLeftMouseButtonPressed(mouseState))m_isPressed = !m_isPressed;
}
/*
*	@brief �`��
*	@details �X�s�[�h�A�b�vUI�̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpeedUpUI::Render()
{
	// �{�^���̕`��
	DrawQuad(m_pTextures, m_vertices, POSITION.x, POSITION.y, SIZE.x, SIZE.y, m_isPressed, m_frameCols, m_frameRows);
}
/*
*	@brief �l�p�`�̕`��
*	@details �X�s�[�h�A�b�vUI�̎l�p�`��`�悷��
*	@param texture �`�悷��e�N�X�`���̃��X�g
*	@param vertices ���_�z��
*	@param startX �l�p�`�̍���X���W
*	@param startY �l�p�`�̍���Y���W
*	@param width �l�p�`�̕�
*	@param height �l�p�`�̍���
*	@param frameIndex �A�j���[�V�����̃t���[���C���f�b�N�X
*	@param frameCols �A�j���[�V�����̗�
*	@param frameRows �A�j���[�V�����̍s��
*	@return �Ȃ�
*/
void SpeedUpUI::DrawQuad(
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height,
	int frameIndex, int frameCols, int frameRows)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	// ���_���W�̌v�Z
	float correctedHeight = height * aspect;
	// NDC���W�n�ɕϊ�
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	// �E���̍��W��NDC���W�n�ɕϊ�
	Vector2 ndcRB = ToNDC(Vector2(startX + width, startY + correctedHeight));
	// ���_���W�̐ݒ�
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
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertices);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief UI�Ƃ̓����蔻��
*	@details �}�E�X�̍��W��UI�̈ʒu���r���ē����蔻����s��
*	@param position �}�E�X�̍��W
*	@return �����蔻�肪�����true�A�Ȃ����false
*/
bool SpeedUpUI::Hit(const DirectX::SimpleMath::Vector2& position)
{
	using namespace DirectX::SimpleMath;
	// ����E�E�����X�N���[�����W�֕ϊ�
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	// �E�B���h�E�n���h�����擾
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�T�C�Y�擾
	RECT rect;
	// �N���C�A���g�̈�T�C�Y���擾
	GetClientRect(hwnd, &rect);
	// �E�B���h�E�̃N���C�A���g�̈�̃T�C�Y���擾
	float aspect = static_cast<float>(rect.right) / static_cast<float>(rect.bottom);
	// �\���T�C�Y��␳
	float correctedHeight = SIZE.y * aspect;
	// NDC���W�n�ɕϊ�
	Vector2 ndcLT = (Vector2(POSITION.x, POSITION.y));
	Vector2 ndcRB = (Vector2(POSITION.x + SIZE.x, POSITION.y + correctedHeight));
	// ����ƉE���̍��W���X�N���[�����W�ɕϊ�
	Vector2 leftTop = Vector2(ndcLT.x * rect.right, ndcLT.y * rect.bottom);
	Vector2 rightBottom = Vector2(ndcRB.x * rect.right, ndcRB.y * rect.bottom);
	// �}�E�X�̍��W���摜�͈͓̔��ɂ���Ȃ�true��Ԃ�
	if (leftTop.x <= position.x && position.x <= rightBottom.x && leftTop.y <= position.y && position.y <= rightBottom.y)return true;
	// �����蔻��Ȃ��Ȃ�false��Ԃ�
	return false;
}
