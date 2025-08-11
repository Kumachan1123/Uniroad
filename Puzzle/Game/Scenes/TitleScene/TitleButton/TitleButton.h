/*
*	@file TitleButton.h
*	@brief �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̃w�b�_�[�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <vector>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Button/Button.h"
#include "Game/Interface/IButton.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
#include "KumachiLib/Animaiton/Animation.h"

// �O���錾
class CommonResources;

// �^�C�g���V�[���̃{�^���N���X
class TitleButton : public IButton
{
public:
	// �A�N�Z�T
	// �X�s�[�h�A�b�v�{�^���������ꂽ���ǂ������擾
	bool IsPressed() const override { return m_isPressed; }
	// �X�s�[�h�A�b�v�{�^���������ꂽ���ǂ�����ݒ�
	void SetPressed(bool isPressed) override { m_isPressed = isPressed; }
	// ���W���擾
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ���W��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// �T�C�Y���擾
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// �T�C�Y��ݒ�
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
	// �q�b�g�����{�^���̃C���f�b�N�X���擾
	int GetHitButtonIndex() const;

public:
	// public�֐�
	// �R���X�g���N�^
	TitleButton();
	// �f�X�g���N�^
	~TitleButton();
	// ������ 
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V����
	void Update(float elapsedTime)override;
	// �`�悷��
	void Render()override;
private:
	// private�֐�
	// �A�j���[�V�����V�[�P���X���쐬
	void CreateAnimationSequence();
private:
	// private�萔
	// �\���ʒu�i����j
	static const std::vector<DirectX::SimpleMath::Vector2> POSITIONS;
	// �\���T�C�Y
	static const std::vector<DirectX::SimpleMath::Vector2> SIZES;
private:
	// private�ϐ�
	// ���ʃ��\�[�X 
	CommonResources* m_pCommonResources;
	// �Q�[���J�n�{�^��
	std::unique_ptr<Button> m_pStartButton;
	// �ݒ胁�j���[�{�^��
	std::unique_ptr<Button> m_pSettingButton;
	// �Q�[���I���{�^��
	std::unique_ptr<Button> m_pExitButton;
	// �{�^���z��
	std::vector<std::unique_ptr<Button>> m_buttons;
	// �A�j���[�V����
	std::unique_ptr<Animation> m_pAnimation;
	// �{�^���̋�`
	std::vector<Rect> m_buttonRects;
	// ���W
	DirectX::SimpleMath::Vector2 m_position;
	// �T�C�Y
	DirectX::SimpleMath::Vector2 m_size;
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// �����蔻��
	std::vector<bool> m_isHit;
	// �{�^���������ꂽ�t���O
	bool m_isPressed;

};