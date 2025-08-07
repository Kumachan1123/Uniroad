/*
*	@file    KumachiLib.h
*	@brief   ����̃��C�u�����̃w�b�_�[�t�@�C��
*	@details C++14�ŕW�����ڂ���Ă��Ȃ��֐��₻�̑��֗��Ȋ֐��̃e���v���[�g�Ȃǂ��܂Ƃ߂�����
*/
#pragma once

// �W�����C�u����
#include <cmath>
#include <algorithm>
#include <random>
#include <type_traits> 
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/Screen/Screen.h"
/*
*	@brief   2D�x�N�g���̐��`���
*	@details 2D�x�N�g���̐��`��Ԃ��s���֐�
*	@param   start �J�n�_
*	@param   end �I���_
*	@param   t ��ԌW�� (0.0f <= t <= 1.0f)
*	@return  ��Ԃ��ꂽ�x�N�g��
*/
template <typename T>
inline T Lerp(const T& start, const T& end, float t)
{
	// ���`���
	return start + t * (end - start);
}
/*
*	@brief   2D�x�N�g���̃N�����v
*	@details 2D�x�N�g���̃N�����v���s���֐�
*	@param   value �N�����v����l
*	@param   min �ŏ��l
*	@param   max �ő�l
*	@return  �N�����v���ꂽ�l
*/
template <typename T>
inline T Clamp(T value, T min, T max)
{
	// �N�����v
	return (value < min) ? min : (value > max) ? max : value;
}
/*
*	@brief   2D�x�N�g���̃X�t�F���J�����
*	@details 2D�x�N�g���̃X�t�F���J����Ԃ��s���֐�
*	@param   start �J�n�_
*	@param   end �I���_
*	@param   t ��ԌW�� (0.0f <= t <= 1.0f)
*	@return  ��Ԃ��ꂽ�x�N�g��
*/
template <typename T>
inline T Slerp(const T& start, const T& end, float t)
{
	// ����
	float dot = start.Dot(end);
	// �N�����v
	dot = Clamp(dot, -1.0f, 1.0f);
	// �p�x
	float theta = std::acos(dot) * t;
	// ���΃x�N�g��
	T relative = end - start * dot;
	// ���K��
	relative.Normalize();
	// �X�t�F���J�����
	return ((start * std::cos(theta)) + (relative * std::sin(theta)));
}
/*
*	@brief   2D�x�N�g���̊p�x���v�Z
*	@details 2D�x�N�g���̊p�x���v�Z����֐�
*	@param   eye ���_
*	@param   target �^�[�Q�b�g
*	@return  �p�x
*/
inline float CalculateAngle(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target)
{
	// �x�N�g���̍���
	DirectX::SimpleMath::Vector3 direction = target - eye;
	// �x�N�g���𐳋K��
	direction.Normalize();
	// �A�[�N�^���W�F���g
	float angle = std::atan2(direction.x, direction.z);
	// �p�x
	return angle;
}
/*
*	@brief   ����A������B�Ɍ������ē���(�����͖���)
*	@details ����A������B�Ɍ������ē������߂̃x�N�g�����v�Z����֐�
*	@param   A ����A�̈ʒu
*	@param   B ����B�̈ʒu
*	@param   maxSpeed �ő呬�x
*/
inline DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& A,
	const DirectX::SimpleMath::Vector3& B,
	float maxSpeed)
{
	// �����x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 desired = B - A;
	// �����x�N�g���𐳋K��
	desired.Normalize();
	// �ő呬�x
	desired *= maxSpeed;
	// �����𖳎�
	desired.y = 0;
	// �ڕW�ʒu
	return desired;
}

/*
*	@brief   �����^�p�̃����_���Ȕ{���𐶐�����
*	@details �����^�p�̃����_���Ȕ{���𐶐�����֐�
*	@param   min �ŏ��l
*	@param   max �ő�l
*	@return  �����_���Ȕ{��
*/
inline int GenerateRandomMultiplier(int min, int max)
{
	// ����������
	std::random_device rd;
	// �����Z���k�E�c�C�X�^�@
	std::mt19937 gen(rd());
	// ��l���z
	std::uniform_int_distribution<int> dis(min, max);
	// �����_���Ȕ{���𐶐�
	return dis(gen);
}

/*
*	@brief   ���������_�^�p�̃����_���Ȕ{���𐶐�����
*	@details ���������_�^�p�̃����_���Ȕ{���𐶐�����֐�
*	@param   min �ŏ��l
*	@param   max �ő�l
*	@return  �����_���Ȕ{��
*/
inline float GenerateRandomMultiplier(float min, float max)
{
	// ����������
	std::random_device rd;
	// �����Z���k�E�c�C�X�^�@
	std::mt19937 gen(rd());
	// ��l���z
	std::uniform_real_distribution<float> dis(min, max);
	// �����_���Ȕ{���𐶐�
	return dis(gen);
}

/*
*	@brief   ���̓��m���Փ˂����牟���߂�(���̓��m)
*	@details ���̓��m�̏Փ˔�����s���A�����߂��������s���֐�
*	@param   A ����A
*	@param   B ����B
*	@return  �����߂��ꂽ�ʒu
*/
inline DirectX::SimpleMath::Vector3 CheckHitOtherObject(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
{
	using namespace DirectX::SimpleMath;
	// �����߂�����
	// A�̒��S��B�̒��S�Ƃ̍����x�N�g��
	Vector3 diffVector = A.Center - B.Center;
	// �����x�N�g���̒������v�Z
	float distance = diffVector.Length();
	// A�̔��a��B�̔��a�̍��v���v�Z
	float sumRadius = A.Radius + B.Radius;
	// A��B�ɂ߂荞�񂾋���
	float penetrationDistance = sumRadius - distance;
	// �����x�N�g���𐳋K��
	diffVector.Normalize();
	// �����߂��x�N�g�����v�Z����
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// �����߂��x�N�g�����g�p���āAA�̍��W��A�̃R���C�_�[���W���X�V�i���ۂɉ����߂��j
	Vector3 newPosition = A.Center + pushBackVec;
	// A�̍��W���X�V����
	A.Center = newPosition;
	// �X�V���ꂽA�̍��W��Ԃ�
	return newPosition;
}
/*
*	@brief   �I�u�W�F�N�g���m���Փ˂����牟���߂�(���E���Ƌ��E�{�b�N�X�̏ꍇ�j
*	@details ���E���Ƌ��E�{�b�N�X�̏Փ˔�����s���A�����߂��������s���֐�
*	@param   A ����A
*	@param   B �{�b�N�XB
*	@return  �����߂��ꂽ�ʒu
*/
inline DirectX::SimpleMath::Vector3 CheckHitWall(DirectX::BoundingSphere A, const DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	// �����߂��x�N�g�����v�Z
	Vector3 pushBackVec = Vector3::Zero;
	// �{�b�N�X�̍ł��߂��_
	Vector3 closestPoint;
	// �e���ŃN�����v���āA�ł��߂��ʒu���擾
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));// X��
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));// Y��
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));// Z��
	// ���̂̒��S�ƍł��߂��_�̃x�N�g����
	Vector3 diffVector = A.Center - closestPoint;
	// �������v�Z
	float distance = diffVector.Length();
	// �Փ˂��Ă��Ȃ��ꍇ�͌��̈ʒu��Ԃ�
	if (distance > A.Radius)return A.Center;
	// ���������̂̔��a��菬�����ꍇ�͉����߂�����
	// �����߂��ʂ��v�Z (���K�����ĉ����߂��x�N�g�����쐬)
	float penetrationDistance = A.Radius - distance;
	// �����x�N�g���𐳋K��
	diffVector.Normalize();
	// �����߂��x�N�g�����v�Z
	pushBackVec = diffVector * penetrationDistance;
	// �����߂��x�N�g�����g�p���āAA�̍��W���X�V
	Vector3 newPosition = A.Center + pushBackVec;
	// ���̂̈ʒu���X�V
	A.Center = newPosition;
	// �X�V���ꂽ�ʒu��Ԃ�
	return newPosition;
}

/*
*	@brief   UV���W��NDC���W�ɕϊ�
*	@details UV���W�𐳋K���f�o�C�X���W(NDC)�ɕϊ�����֐�
*	@param   uv UV���W
*	@return  NDC���W
*/
inline DirectX::SimpleMath::Vector2 ToNDC(const DirectX::SimpleMath::Vector2& uv)
{
	return DirectX::SimpleMath::Vector2(uv.x * 2.0f - 1.0f, 1.0f - uv.y * 2.0f);
}

/*
*	@brief   ��ʍ��W���擾����
*	@details ��ʃ��[�h���ς���Ă����v�Ȃ悤�ɓ��I�Ɏ󂯎�邽�߂̊֐�
*	@param   hwnd �E�B���h�E�n���h��
*	@param   width �E�B���h�E�̕�(�����Ŏ󂯎���Đݒ肵�Ă���Ԃ��j
*	@param   height �E�B���h�E�̍���(�����Ŏ󂯎���Đݒ肵�Ă���Ԃ��j
*	@return  �Ȃ�
*/
inline void GetScreenRect(const HWND hwnd, int& width, int& height)
{
	// RECT�\���̂�������
	RECT rect = { 0, 0, width, height };
	// �E�B���h�E�̃N���C�A���g�̈�̃T�C�Y���擾
	if (GetClientRect(hwnd, &rect))
	{
		// ���ƍ�����ݒ�
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
}