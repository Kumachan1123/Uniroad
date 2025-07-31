/*
*	@file TrackingCamera.cpp
*	@brief �Ǐ]�J�����N���X�̎���
*/
#include "pch.h"
#include "TrackingCamera.h"
// �f�t�H���g�̃J���������E�����E�p�x
const float TrackingCamera::DEFAULT_CAMERA_DISTANCE = 12.5f;
const float TrackingCamera::DEFAULT_CAMERA_HEIGHT = 9.0f;
const float TrackingCamera::DEFAULT_CAMERA_ANGLE = 30.0f;
/*
*	@brief �R���X�g���N�^
*	@details �Ǐ]�J�����N���X�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
TrackingCamera::TrackingCamera()
	: m_eye(0.0f, 0.0f, DEFAULT_CAMERA_DISTANCE) // ���_��������
	, m_target(0.0f, 0.0f, 0.0f) // �����_�����_�ɐݒ�
	, m_up(0.0f, 1.0f, 0.0f) // �������Y���������ɐݒ�
	, m_sx(0.0f), m_sy(0.0f)// ���΃X�P�[����������
	, m_view(DirectX::SimpleMath::Matrix::Identity) // �r���[�s���P�ʍs��ŏ�����
	, m_direction(0.0f, 0.0f, 0.0f) // �J�����̌����Ă��������������
	, m_cameraDistance(DEFAULT_CAMERA_DISTANCE) // �J�����̋�����������
	, m_cameraHeight(DEFAULT_CAMERA_HEIGHT) // �J�����̍�����������
	, m_cameraAngle(DEFAULT_CAMERA_ANGLE) // �J�����̊p�x�iY����]�j��������
{
}
/*
*	@brief ����������
*	@details �Œ�J�����N���X�̏�����
*	@param screenWidth ��ʂ̕�
*	@param screenHeight ��ʂ̍���
*	@return �Ȃ�
*/
void TrackingCamera::Initialize(int screenWidth, int screenHeight)
{
	// ���΃X�P�[�����v�Z
	CalculateRerativeScale(screenWidth, screenHeight);
	// �I�n�W�̏����l�́i0,0,0�j�ɐݒ�
	SetTargetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
	// �r���[�s����v�Z
	CalculateViewMatrix();
}
/*
*	@brief �X�V����
*	@details �Œ�J�����N���X�̍X�V
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TrackingCamera::Update()
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
void TrackingCamera::CalculateViewMatrix()
{
	using namespace DirectX::SimpleMath;
	// 0.0 �` 1.0�͈̔͂Łu�ǂꂾ�������Ǐ]���邩�v�����߂�
	float followSpeed = 0.1f; // 0.05�`0.2���炢�Œ���

	// �^�[�Q�b�g��X���W
	float targetX = m_target.x;
	// �J�����̌���X���W
	float currentX = m_eye.x;

	// Lerp�i���`��ԁj�ŏ������߂Â���
	float newX = currentX + (targetX - currentX) * followSpeed;

	// Y/Z�͌Œ�
	float camY = m_cameraHeight;
	float camZ = m_cameraDistance;

	// eye��target���X�V�itarget��X�����Ǐ]�j
	m_eye = DirectX::SimpleMath::Vector3(newX, camY, camZ);
	m_target = DirectX::SimpleMath::Vector3(newX, 0.0f, 0.0f);

	// �r���[�s����Čv�Z
	m_up = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
	m_direction = (m_target - m_eye);
	m_direction.Normalize();
}
/*
*	@brief ���΃X�P�[�����v�Z����
*	@details �E�B���h�E�̕��ƍ����Ɋ�Â��đ��΃X�P�[�����v�Z����
*	@param windowWidth �E�B���h�E�̕�
*	@param windowHeight �E�B���h�E�̍���
*	@return �Ȃ�
*/
void TrackingCamera::CalculateRerativeScale(int windowWidth, int windowHeight)
{
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	m_sx = 1.0f / static_cast<float>(windowWidth);
	m_sy = 1.0f / static_cast<float>(windowHeight);
}
