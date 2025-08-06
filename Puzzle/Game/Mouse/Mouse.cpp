/*
*	@file Mouse.cpp
*	@brief ����ȃr���[�|�[�g�ȉ�ʂŎg���}�E�X�N���X
*/
#include <pch.h>
#include "Mouse.h"
/*
*	@brief �R���X�g���N�^
*	@details �}�E�X�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
MyMouse::MyMouse()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_position(0.0f, 0.0f) // �}�E�X�̈ʒu
	, m_viewPortControll() // �r���[�|�[�g�̐���
	, m_renderWidth(0.0f) // �E�B���h�E�̕��i�s�N�Z���P�ʁj
	, m_renderHeight(0.0f) // �E�B���h�E�̍����i�s�N�Z���P�ʁj
	, m_vp_left_UI(0.0f) // �r���[�|�[�g����X�i�_���𑜓x��j
	, m_vp_top_UI(0.0f) // �r���[�|�[�g����Y�i�_���𑜓x��j
	, m_vp_width_UI(0.0f) // �r���[�|�[�g���i�_���𑜓x��j
	, m_vp_height_UI(0.0f) // �r���[�|�[�g�����i�_���𑜓x��j
	, m_isMouseDrag(false) // �}�E�X�h���b�O�t���O
	, m_hit(false) // �q�b�g�t���O
	, m_hitNewTile(false) // �q�b�g�t���O(�V�����o�Ă����^�C��)
	, m_hitNewTileIndex(-1) // �������Ă���V�����o�Ă����^�C���̔ԍ�(-1�͓������Ă��Ȃ����Ƃ�����)
	, m_hitPanelIndex(-1) // �������Ă���p�l���̔ԍ�(-1�͓������Ă��Ȃ����Ƃ�����)
	, m_hitPanelRowIndex(-1) // �������Ă���p�l���̍s�ԍ�(-1�͓������Ă��Ȃ����Ƃ�����)
	, m_hitPanelColIndex(-1) // �������Ă���p�l���̗�ԍ�(-1�͓������Ă��Ȃ����Ƃ�����)
	, m_prevLeftButton(false) // �O�t���[���̍��{�^�����
	, m_leftReleased(false) // ���{�^���������ꂽ���ǂ���
{
}
/*
*	@brief �f�X�g���N�^
*	@details �}�E�X�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
MyMouse::~MyMouse()
{
	// �������Ȃ�
}

/*
*	@brief ������
*	@details �}�E�X�N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void MyMouse::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ۑ�
	m_pCommonResources = resources;
}
/*
*	@brief �X�V
*	@details �}�E�X�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MyMouse::Update(const float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime); // �o�ߎ��Ԃ͎g�p���Ȃ�
	using namespace DirectX::SimpleMath;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �E�B���h�E�n���h�����擾
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�T�C�Y�擾
	RECT rect;
	// �N���C�A���g�̈�T�C�Y���擾
	GetClientRect(hwnd, &rect);
	// �E�B���h�E�̕��i�s�N�Z���P�ʁj
	m_renderWidth = static_cast<float>(rect.right);
	// �E�B���h�E�̍����i�s�N�Z���P�ʁj
	m_renderHeight = static_cast<float>(rect.bottom);
	// �r���[�|�[�g�ݒ� 
	D3D11_VIEWPORT viewportRight = {};
	// �r���[�|�[�g�̍���X���W�i��ʕ���70%�ʒu�j
	viewportRight.TopLeftX = m_renderWidth * 0.7f;
	// �r���[�|�[�g�̍���Y���W�i�ŏ㕔�j
	viewportRight.TopLeftY = 0;
	// �r���[�|�[�g�̕��i��ʕ���30%�j
	viewportRight.Width = m_renderWidth * 0.3f;
	// �r���[�|�[�g�̍����i��ʍ����S�́j
	viewportRight.Height = m_renderHeight;
	// �ŏ��[�x
	viewportRight.MinDepth = 0.0f;
	// �ő�[�x
	viewportRight.MaxDepth = 1.0f;
	// �r���[�|�[�g���������o�ϐ��ɕۑ�
	m_viewPortControll = viewportRight;
	// �r���[�|�[�g����X
	float vp_left = m_viewPortControll.TopLeftX;
	// �r���[�|�[�g����Y
	float vp_top = m_viewPortControll.TopLeftY;
	// �r���[�|�[�g��
	float vp_width = m_viewPortControll.Width;
	// �r���[�|�[�g����
	float vp_height = m_viewPortControll.Height;

	// UI�̘_���𑜓x 
	// �_���𑜓x�̕�
	constexpr float logicalWidth = 1920.0f;
	// �_���𑜓x�̍���
	constexpr float logicalHeight = 1080.0f;
	// �}�E�X���W��_���𑜓x��ɃX�P�[�����O
	// �_���𑜓x��̃}�E�XX���W
	float mouseX_UI = mouseState.x * (logicalWidth / m_renderWidth);
	// �_���𑜓x��̃}�E�XY���W
	float mouseY_UI = mouseState.y * (logicalHeight / m_renderHeight);
	// �r���[�|�[�g�̘_���𑜓x��ł̍��W�E�T�C�Y���v�Z
	// �_���𑜓x��̃r���[�|�[�g����X
	m_vp_left_UI = vp_left * (logicalWidth / m_renderWidth);
	// �_���𑜓x��̃r���[�|�[�g����Y
	m_vp_top_UI = vp_top * (logicalHeight / m_renderHeight);
	// �_���𑜓x��̃r���[�|�[�g��
	m_vp_width_UI = vp_width * (logicalWidth / m_renderWidth);
	// �_���𑜓x��̃r���[�|�[�g����
	m_vp_height_UI = vp_height * (logicalHeight / m_renderHeight);
	// �}�E�X���W���r���[�|�[�g�����[�J�����W�ɕϊ�
	m_position = Vector2(mouseX_UI - m_vp_left_UI, mouseY_UI - m_vp_top_UI);

	// �����ꂽ������
	m_leftReleased = (!mouseState.leftButton) && m_prevLeftButton;
	// �Ō�ɁA�O�t���[���̏�Ԃ��X�V���Ă���
	m_prevLeftButton = mouseState.leftButton;
	//// ---�f�o�b�O�\��---
	//const auto debugString = m_pCommonResources->GetDebugString();
	//debugString->AddString("isInside: %s",
	//	(mouseX_UI >= m_vp_left_UI) && (mouseX_UI < m_vp_left_UI + m_vp_width_UI)
	//	&& (mouseY_UI >= m_vp_top_UI) && (mouseY_UI < m_vp_top_UI + m_vp_height_UI)
	//	? "true" : "false"); // �}�E�X���r���[�|�[�g���ɂ��邩
	//debugString->AddString("Inside ViewPort Mouse Position: (%f, %f)", m_position.x, m_position.y); // �r���[�|�[�g���}�E�X���W
	//debugString->AddString("PanelPosition:%f,%f", GetPanelPosition().x, GetPanelPosition().y);// �������Ă���p�l���̈ʒu
	//debugString->AddString("PanelRow:%i, PanelCol:%i", GetHitPanelRowIndex(), GetHitPanelColIndex());// �������Ă���p�l���̍s�Ɨ�̃C���f�b�N�X
	//debugString->AddString("HitNextTilePosition: %f, %f", GetNewTilePosition().x, GetNewTilePosition().y);// �������Ă���V�����^�C���̈ʒu
	//debugString->AddString("DragFlag:%s", m_isMouseDrag ? "true" : "false");// �h���b�O�t���O�̏��
	//debugString->AddString("hitPanel:%i", GetHitPanelIndex());// �������Ă���p�l���̃C���f�b�N�X
	//debugString->AddString("hitNextTile:%i", GetHitNewTileIndex());// �������Ă���V�����o�Ă����^�C���̃C���f�b�N�X
	//debugString->AddString("LeftReleased: %s", m_leftReleased ? "true" : "false");// ���{�^���������ꂽ���ǂ���
}
