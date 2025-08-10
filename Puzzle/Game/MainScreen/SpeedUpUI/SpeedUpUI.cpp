/*
*	@file SpeedUpUI.cpp
*	@brief �X�s�[�h�A�b�v�{�^���N���X
*/
#include "pch.h"
#include "SpeedUpUI.h"
// �\���ʒu�i����j�̒�`
const DirectX::SimpleMath::Vector2 SpeedUpUI::POSITION = DirectX::SimpleMath::Vector2(0.665f, 0.07f);
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
	, m_isHit(false) // �����蔻��t���O
	, m_isPressed(false) // �{�^���������ꂽ�t���O
	, m_frameRows(2) // �摜�̍s��
	, m_frameCols(1) // �摜�̗�
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
	// �{�^���̍쐬
	m_pButton = std::make_unique<Button>();
	// �V�F�[�_�[�p�X��n��
	m_pButton->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	m_pButton->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// �摜��ݒ�
	m_pButton->SetTexture(resources->GetTextureManager()->GetTexture("SpeedUP"));
	// �{�^���̏�����
	m_pButton->Initialize(resources, width, height);
	// �{�^���̋�`��ݒ�
	m_buttonRect.position = POSITION;
	m_buttonRect.size = SIZE;
}
/*
*	@brief �X�V
*	@details �X�s�[�h�A�b�vUI�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void SpeedUpUI::Update(float elapsedTime)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �����蔻����s��
	m_isHit = m_pButton->Hit(mousePos, m_buttonRect);
	// �}�E�X���������č��N���b�N���ꂽ�牟���ꂽ��Ԃ��g�O��
	if (m_isHit && MouseClick::IsLeftMouseButtonPressed(mouseState))m_isPressed = !m_isPressed;
}
/*
*	@brief �`��
*	@details �X�s�[�h�A�b�vUI�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpeedUpUI::Render()
{
	// �{�^���̕`��
	m_pButton->DrawQuad(POSITION.x, POSITION.y, SIZE.x, SIZE.y, m_isPressed, m_frameCols, m_frameRows);
}
