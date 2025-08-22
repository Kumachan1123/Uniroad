/*
*	@file BackButton.h
*	@brief �X�e�[�W�Z���N�g��ʂ̃{�^�����Ǘ�����N���X�̃w�b�_�[�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <vector>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
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

// �X�e�[�W�Z���N�g��ʂ̃{�^���N���X
class BackButton : public IButton
{
public:
	// �A�N�Z�T
	// �{�^���������ꂽ���ǂ������擾
	bool IsPressed() const override { return m_isPressed; }
	// �{�^���������ꂽ���ǂ�����ݒ�
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
	int GetHitButtonIndex()const { return (int)(m_hitButtonIndex); }
	// �����ꂽ�{�^���̔ԍ����擾
	int GetPressedButtonIndex() const { return m_pressedButtonIndex; }
public:
	// public�֐�
	// �R���X�g���N�^
	BackButton();
	// �f�X�g���N�^
	~BackButton();
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
	// �萔�o�b�t�@���X�V
	void UpdateConstantBuffer()override;
public:
	// public�萔
	// �{�^����������Ă��Ȃ���Ԃ̃C���f�b�N�X
	static const int NONE_BUTTON_INDEX;
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
	// �^�C�g���ɖ߂�{�^��
	std::unique_ptr<Button> m_pBackButton;
	// �{�^���z��
	std::vector<std::unique_ptr<Button>> m_buttons;
	// �A�j���[�V����
	std::unique_ptr<Animation> m_pAnimation;
	// �{�^���̋�`
	std::vector<Rect> m_buttonRects;
	// �萔�o�b�t�@
	SpriteSheetBuffer m_spriteSheetBuffer;
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
	// ���������{�^���̔ԍ�
	int m_hitButtonIndex;
	// �����ꂽ�{�^���̔ԍ�
	int m_pressedButtonIndex;
	// �{�^���������ꂽ�t���O
	bool m_isPressed;
	// �e�{�^���̃z�o�[���̊g�嗦
	std::vector<float> m_hoverScales;

};