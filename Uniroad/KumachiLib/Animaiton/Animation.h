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
	// �A�N�Z�T
	// �V�[�P���X��i�߂�
	void AdvanceSequence();
	// �S�A�j���[�V�������I���������Ƃ��m�F
	bool IsAnimationFinished() const { return m_currentStep >= m_animSequence.size() - 1; }
	// ���̃A�j���[�V�����t�F�[�Y���擾
	size_t GetAnimationPhase() const { return m_currentStep; }
	// �o�^�����A�j���[�V�����V�[�P���X�̐����擾
	size_t GetAnimationSequenceCount() const { return m_animSequence.size() - 1; }
	// �ꎞ��~�ƍĊJ
	void Pause() { m_isPaused = true; }
	void Resume() { m_isPaused = false; }
	// �ꎞ��~�����ǂ������擾
	bool IsPaused() const { return m_isPaused; }
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
	// �ꎞ��~
	bool m_isPaused;
	// ���݂̃A�j���[�V�����X�e�b�v
	size_t m_currentStep;
	// �A�j���[�V�����X�e�b�v����
	float m_animStepTime;
	// �A�j���[�V�����V�[�P���X
	std::vector<AnimationStep> m_animSequence;
};