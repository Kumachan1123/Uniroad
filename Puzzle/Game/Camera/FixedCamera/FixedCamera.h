/*
*	@file FixedCamera.h
*	@brief �Œ�J�����N���X
*	@details ����̈ʒu�ƕ����������A��ɂ��̈ʒu����V�[����`�悷��
*/
#pragma once
// �Œ�J�����N���X
class FixedCamera
{
public:
	// �A�N�Z�T
	// �r���[�s��i�J�����s��j���擾����
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// ���_���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }
	// ���_��ݒ肷��
	void SetEyePosition(const DirectX::SimpleMath::Vector3& eye) { m_eye = eye; }
	// �����_���擾����
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// �����_��ݒ肷��
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) { m_target = target; }
	// �J�����̓��̕������擾����
	const DirectX::SimpleMath::Vector3& GetUpPosition() const { return m_up; }
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
	// ���_
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �J�����̓��̕���
	DirectX::SimpleMath::Vector3 m_up;
	// �J�����̋���
	DirectX::SimpleMath::Vector3 m_cameraDistance;
};