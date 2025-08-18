/*
*	@file FixedCamera.h
*	@brief �Œ�J�����N���X
*	@details ����̈ʒu�ƕ����������A��ɂ��̈ʒu����V�[����`�悷��
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/Interface/ICamera.h"

// �Œ�J�����N���X
class FixedCamera : public ICamera
{
public:
	// �A�N�Z�T
	// �r���[�s��i�J�����s��j���擾����
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const  override { return m_view; }
	// �r���[�s��i�J�����s��j��ݒ肷��
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view)  override { m_view = view; }
	// �ˉe�s����擾����
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const  override { return m_projection; }
	// �ˉe�s���ݒ肷��
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj)  override { m_projection = proj; }
	// ���_���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() const  override { return m_eye; }
	// ���_��ݒ肷��
	void SetEyePosition(const DirectX::SimpleMath::Vector3& eye)  override { m_eye = eye; }
	// �����_���擾����
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const  override { return m_target; }
	// �����_��ݒ肷��
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target)  override { m_target = target; }
	// �J�����̓��̕������擾����
	const DirectX::SimpleMath::Vector3& GetUpPosition() const  override { return m_up; }
	// �J�����̋������擾����
	const DirectX::SimpleMath::Vector3& GetCameraDistance() const { return m_cameraDistance; }
	// �J�����̋�����ݒ肷��
	void SetCameraDistance(const DirectX::SimpleMath::Vector3& distance) { m_cameraDistance = distance; }
public:
	// public�ȃ����o�֐�
	// �R���X�g���N�^
	FixedCamera();
	// �f�X�g���N�^
	~FixedCamera() = default;
	// ����������
	void Initialize(int screenWidth, int screenHeight);
	// �X�V����
	void Update();
private:
	// �r���[�s����v�Z����
	void CalculateViewMatrix();
	// ���΃X�P�[�����v�Z����
	void CalculateRerativeScale(int windowWidth, int windowHeight);
private:
	// �f�t�H���g�ŃJ�����������_����ǂ̂��炢����Ă��邩
	static const float DEFAULT_CAMERA_DISTANCE;
	// ���΃X�P�[��
	float m_sx, m_sy;
	// �������ꂽ�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// ���_
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �J�����̓��̕���
	DirectX::SimpleMath::Vector3 m_up;
	// �J�����̋���
	DirectX::SimpleMath::Vector3 m_cameraDistance;
};