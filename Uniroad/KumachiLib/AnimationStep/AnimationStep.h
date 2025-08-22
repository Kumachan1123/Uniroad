/*
*	@file AnimationStep.h
*	@brief �C�[�W���O�֐��ɂ��A�j���[�V�����X�e�b�v�̃w�b�_�[�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <functional>
struct AnimationStep
{
	// ���o�̒���
	float duration;
	// �i�s�x
	std::function<void(float t)> updateFunc;
};