/*
*	@file TrackingCamera.h
*	@brief �Ǐ]�J�����N���X
*	@details ����̕����������A��ɑΏۂ�ǂ������V�[����`�悷��
*/
#pragma once
// �Ǐ]�J�����N���X
class TrackingCamera
{
public:
	// �A�N�Z�T
	// �r���[�s��i�J�����s��j���擾����
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// ���_���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }
	// �����_���擾����
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// �����_��ݒ肷��
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) { m_target = target; }
	// �J�����̓��̕������擾����
	const DirectX::SimpleMath::Vector3& GetUpPosition() const { return m_up; }
	// �J�����̌����Ă���������擾����
	const DirectX::SimpleMath::Vector3& GetDirection() const { return m_direction; }
	// �J�����̋������擾����
	float GetCameraDistance() const { return m_cameraDistance; }
	// �J�����̍������擾����
	float GetCameraHeight() const { return m_cameraHeight; }
	// �J�����̊p�x�iY����]�j���擾����	
	float GetCameraAngle() const { return m_cameraAngle; }
	// �J�����̃p�����[�^��ݒ肷��
	void SetCameraParameters(float distance, float height, float angle)
	{
		m_cameraDistance = distance;
		m_cameraHeight = height;
		m_cameraAngle = angle;
	}
public:
	// public�ȃ����o�֐�
	// �R���X�g���N�^
	TrackingCamera();
	// �f�X�g���N�^
	~TrackingCamera() = default;
	// ����������
	void Initialize(int screenWidth, int screenHeight);
	// �X�V����
	void Update();

private:
	// private�֐�
	// �r���[�s����v�Z����
	void CalculateViewMatrix();
	// ���΃X�P�[�����v�Z����
	void CalculateRerativeScale(int windowWidth, int windowHeight);
private:
	// �f�t�H���g�l
	// �J�����������_����ǂ̂��炢����Ă��邩
	static const float DEFAULT_CAMERA_DISTANCE;
	// �J�����̍���
	static const float DEFAULT_CAMERA_HEIGHT;
	// �J�����̊p�x�iY����]�j
	static const float DEFAULT_CAMERA_ANGLE;
private:
	// private�����o�ϐ�
	// ���΃X�P�[��
	float m_sx, m_sy;
	// �������ꂽ�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// ���_
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �J�����̓��̕���
	DirectX::SimpleMath::Vector3 m_up;
	// �J�����̌����Ă������
	DirectX::SimpleMath::Vector3 m_direction;
	// �J�����̃p�����[�^
	// �J�����̋���
	float m_cameraDistance;
	// �J�����̍���
	float m_cameraHeight;
	// �J�����̊p�x�iY����]�j
	float m_cameraAngle;
};