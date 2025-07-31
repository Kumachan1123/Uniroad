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
	// �����_���擾����
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// �J�����̓��̕������擾����
	const DirectX::SimpleMath::Vector3& GetUpPosition() const { return m_up; }
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
	//// �ψʂ��v������
	//void Motion(int x, int y);
	// �r���[�s����v�Z����
	void CalculateViewMatrix();
	// ���΃X�P�[�����v�Z����
	void CalculateRerativeScale(int windowWidth, int windowHeight);
private:
	// �f�t�H���g�ŃJ�����������_����ǂ̂��炢����Ă��邩
	static const float DEFAULT_CAMERA_DISTANCE;
	//// ����]�i�x����]�j
	//float m_yAngle, m_yTmp;
	//// �c��]�i�w����]�j
	//float m_xAngle, m_xTmp;
	//// �}�E�X�Ńh���b�O���ꂽ���W
	//int m_x, m_y;
	// ���΃X�P�[��
	float m_sx, m_sy;
	// �������ꂽ�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//// �X�N���[���z�C�[���l
	//int m_scrollWheelValue;
	// ���_
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �J�����̓��̕���
	DirectX::SimpleMath::Vector3 m_up;
};