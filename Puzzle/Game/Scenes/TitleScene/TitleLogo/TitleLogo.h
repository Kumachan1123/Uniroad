/*
*	@file TitleLogo.h
*	@brief �^�C�g�����S�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"
#include "Game/Interface/IImage.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
//#include "KumachiLib/AnimationStep/AnimationStep.h"
#include "KumachiLib/Animaiton/Animation.h"

// �O���錾
class CommonResources;

// �^�C�g�����S�N���X
class TitleLogo : public IImage
{
public:
	// �A�N�Z�T
	// ���S�̈ʒu���擾
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ���S�̈ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// ���S�̃T�C�Y���擾
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// ���S�̃T�C�Y��ݒ�
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public�֐�
	// �R���X�g���N�^
	TitleLogo();
	// �f�X�g���N�^
	~TitleLogo();
	// ������
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V
	void Update(float elapsedTime)override;
	// �摜��\��
	void Render()override;
private:
	// private�֐�
	// �A�j���[�V�����V�[�P���X���쐬
	void CreateAnimationSequence();
private:
	// private�萔
	// �\���ʒu�i����j
	static const DirectX::SimpleMath::Vector2 POSITION;
	// �\���T�C�Y
	static const DirectX::SimpleMath::Vector2 SIZE;
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �摜
	std::unique_ptr<Image> m_pImage;
	// �A�j���[�V����
	std::unique_ptr<Animation> m_pAnimation;
	// ���S�̈ʒu
	DirectX::SimpleMath::Vector2 m_position;
	// ���S�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_size;
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// ���S�̋�`
	Rect m_logoRect;
	// �A�j���[�V�����i�s�x
	float m_animationTime;
	// �A�j���[�V�����̃t�F�[�Y
	int m_animationPhase;
	// ���݂̃A�j���[�V�����X�e�b�v
	size_t m_currentStep;
	// �A�j���[�V�����X�e�b�v����
	float m_animStepTime;
	// �A�j���[�V�����V�[�P���X
	std::vector<AnimationStep> m_animSequence;
};