/*
*	@file Animation.h
*	@brief �A�j���[�V�����N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/AnimationStep/AnimationStep.h"

// �A�j���[�V�����N���X
class Animation
{
public:
	// public�֐�
	// �R���X�g���N�^
	Animation();
	// �f�X�g���N�^
	~Animation() = default;
	// �A�j���[�V�����V�[�P���X���쐬
	void CreateAnimationSequence(const AnimationStep& sequence);
	// �A�j���[�V�������X�V
	void Update(float elapsedTime);
private:
	// private�ϐ�
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