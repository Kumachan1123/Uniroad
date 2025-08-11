/*
*	@file Animation.cpp
*	@brief �A�j���[�V�����N���X�̎����t�@�C��
*/
#include "pch.h"
#include "Animation.h"

/*
*	@brief �R���X�g���N�^
*	@details �A�j���[�V�����N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Animation::Animation()
	: m_animationTime(0.0f) // �A�j���[�V��������
	, m_currentStep(0) // ���݂̃A�j���[�V�����X�e�b�v
	, m_animStepTime(0.0f) // �A�j���[�V�����X�e�b�v����
	, m_isPaused(false) // �ꎞ��~�t���O
{
}
/*
*	@brief �A�j���[�V�����V�[�P���X���쐬
*	@details �A�j���[�V�����V�[�P���X���쐬����
*	@param sequence �A�j���[�V�����X�e�b�v�̃V�[�P���X
*	@return �Ȃ�
*/
void Animation::CreateAnimationSequence(const AnimationStep& sequence)
{
	// �V�����V�[�P���X��ǉ�
	m_animSequence.push_back(sequence);
}
/*
*	@brief �A�j���[�V�������X�V
*	@details �A�j���[�V�����̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Animation::Update(float elapsedTime)
{
	// �ꎞ��~���Ȃ�Ή������Ȃ�
	if (m_isPaused) return;
	// �A�j���[�V�����V�[�P���X�����쐬�Ȃ�ΏI��
	if (m_currentStep >= m_animSequence.size()) return;
	// �A�j���[�V�������Ԃ��X�V
	m_animStepTime += elapsedTime;
	// �A�j���[�V�����X�e�b�v�̍X�V
	float duration = m_animSequence[m_currentStep].duration;
	// �i�s�x���v�Z
	float t = duration > 0.0f ? std::min(m_animStepTime / duration, 1.0f) : 0.0f;
	// �A�j���[�V���������s
	m_animSequence[m_currentStep].updateFunc(t);
	// �A�j���[�V�������I��������
	if (duration > 0.0f && m_animStepTime >= duration)
	{
		// ���̃X�e�b�v�֐i��
		m_currentStep++;
		// �A�j���[�V�����X�e�b�v���Ԃ����Z�b�g
		m_animStepTime = 0.0f;
	}
}
/*
*	@brief �A�j���[�V�����V�[�P���X��i�߂�
*	@details �O������A�j���[�V�����V�[�P���X��i�߂�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Animation::AdvanceSequence()
{
	if (m_currentStep + 1 >= m_animSequence.size())return;
	// ���̃X�e�b�v�֐i��
	m_currentStep++;
	// �A�j���[�V�����X�e�b�v���Ԃ����Z�b�g
	m_animStepTime = 0.0f;
}