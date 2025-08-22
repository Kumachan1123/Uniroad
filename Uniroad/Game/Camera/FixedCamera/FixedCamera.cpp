/*
*	@file FixedCamera.cpp
*	@brief �Œ�J�����N���X
*	@details ����̈ʒu�ƕ����������A��ɂ��̈ʒu����V�[����`�悷��
*/
#include <pch.h>
#include "FixedCamera.h"
const float FixedCamera::DEFAULT_CAMERA_DISTANCE = 12.5f;
/*
*	@brief �R���X�g���N�^
*	@details �Œ�J�����N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
FixedCamera::FixedCamera()
	: m_eye(0.0f, 0.0f, DEFAULT_CAMERA_DISTANCE) // ���_��������
	, m_target(0.0f, 0.0f, 0.0f)// �����_�����_�ɐݒ�
	, m_up(0.0f, 1.0f, 0.0f) // �������Y���������ɐݒ�
	, m_sx(0.0f), m_sy(0.0f)// ���΃X�P�[����������
	, m_cameraDistance(DEFAULT_CAMERA_DISTANCE) // �J�����̋������f�t�H���g�l�ɐݒ�
	, m_view(DirectX::SimpleMath::Matrix::Identity) // �r���[�s���P�ʍs��ŏ�����
{
}
/*
*	@brief ����������
*	@details �Œ�J�����N���X�̏�����
*	@param screenWidth ��ʂ̕�
*	@param screenHeight ��ʂ̍���
*	@return �Ȃ�
*/
void FixedCamera::Initialize(int screenWidth, int screenHeight)
{
	using namespace DirectX::SimpleMath;
	// ���΃X�P�[�����v�Z
	CalculateRerativeScale(screenWidth, screenHeight);
	// �J�����̋������f�t�H���g�l�ɐݒ�
	m_cameraDistance = Vector3(DEFAULT_CAMERA_DISTANCE);
	// �r���[�s����v�Z
	CalculateViewMatrix();
	// ���_���f�t�H���g�̈ʒu�ɐݒ�
	// ���_��ݒ�
	Vector3 eye(0.0f, 1.0f, 1.0f);
	eye *= m_cameraDistance; // �f�t�H���g�̃J����������K�p
	m_eye = eye;
}
/*
*	@brief �X�V����
*	@details �Œ�J�����N���X�̍X�V
*	@param �Ȃ�
*	@return �Ȃ�
*/
void FixedCamera::Update()
{
	// �r���[�s����Z�o����
	CalculateViewMatrix();
}
/*
*	@brief �r���[�s����v�Z����
*	@details �Œ�J�����̃r���[�s����v�Z����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void FixedCamera::CalculateViewMatrix()
{
	using namespace DirectX::SimpleMath;


	// �������Y���������ɐݒ�
	Vector3 up(0.0f, 1.0f, 0.0f);


	// �������ݒ�
	m_up = up;
	// �r���[�s����v�Z����
	m_view = Matrix::CreateLookAt(m_eye, m_target, up);
}
/*
*	@brief ���΃X�P�[�����v�Z����
*	@details �E�B���h�E�̕��ƍ����Ɋ�Â��đ��΃X�P�[�����v�Z����
*	@param windowWidth �E�B���h�E�̕�
*	@param windowHeight �E�B���h�E�̍���
*	@return �Ȃ�
*/
void FixedCamera::CalculateRerativeScale(int windowWidth, int windowHeight)
{
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	m_sx = 1.0f / static_cast<float>(windowWidth);
	m_sy = 1.0f / static_cast<float>(windowHeight);
}
