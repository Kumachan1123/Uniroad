/*
*	@file Button.cpp
*	@brief �{�^���̑匳�ƂȂ�N���X
*/
#include "pch.h"
#include "Button.h"
/*
*	@brief �R���X�g���N�^
*	@details �{�^���̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Button::Button()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pImage(std::make_unique<Image>()) // �摜�ւ̃|�C���^
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
	// �摜�̏�����
	m_pImage->Initialize(m_pCommonResources, width, height);
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
*	@brief �摜��\��
*	@details �{�^���̉摜��\������
*	@param buttonRect �{�^���̋�`
*	@param frameIndex �A�j���[�V�����̃R�}�ԍ�
*	@param frameCols �摜�̗�
*	@param frameRows �摜�̍s��
*	@return �Ȃ�
*/
void Button::Render(const Rect& buttonRect, int frameIndex, int frameCols, int frameRows)
{
	// �摜��`��
	m_pImage->DrawQuad(buttonRect, frameIndex, frameCols, frameRows);

}
/*
*	@brief �����蔻��
*	@details �}�E�X�̍��W�ƃ{�^���̋�`���r���ē����蔻����s��
*	@param mousePosition �}�E�X�̍��W
*	@param buttonRect �{�^���̋�`
*	@return �����蔻�肪�����true�A�Ȃ����false
*/
bool Button::Hit(const DirectX::SimpleMath::Vector2& mousePosition, const Rect& buttonRect)
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
	// ���S���W������E�E�����W�֕ϊ�
	float left = buttonRect.position.x - buttonRect.size.x / 2.0f;
	float top = buttonRect.position.y - correctedHeight / 2.0f;
	float right = left + buttonRect.size.x;
	float bottom = top + correctedHeight;
	// �X�N���[�����W�֕ϊ�
	Vector2 leftTop = Vector2(left * rect.right, top * rect.bottom);
	Vector2 rightBottom = Vector2(right * rect.right, bottom * rect.bottom);
	// �}�E�X�̍��W���摜�͈͓̔��ɂ���Ȃ�true��Ԃ�
	if (leftTop.x <= mousePosition.x && mousePosition.x <= rightBottom.x && leftTop.y <= mousePosition.y && mousePosition.y <= rightBottom.y)
		return true;
	// �����蔻��Ȃ��Ȃ�false��Ԃ�
	return false;
}
