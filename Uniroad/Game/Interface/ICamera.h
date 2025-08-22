/*
*	@file ICamera.h
*	@brief �J�����̃C���^�[�t�F�[�X
*/
#pragma once
// �J�����̃C���^�[�t�F�[�X
class ICamera
{
public:
	// �A�N�Z�T
	// �r���[�s����擾
	virtual const DirectX::SimpleMath::Matrix& GetViewMatrix() const = 0;
	// �r���[�s���ݒ�
	virtual void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) = 0;
	// �ˉe�s����擾
	virtual const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const = 0;
	// �ˉe�s���ݒ�
	virtual void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj) = 0;
	// ���_���擾
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const = 0;
	// ���_��ݒ�
	virtual void SetEyePosition(const DirectX::SimpleMath::Vector3& eye) = 0;
	// �����_���擾
	virtual const DirectX::SimpleMath::Vector3& GetTargetPosition() const = 0;
	// �����_��ݒ�
	virtual void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) = 0;
	// �J�����̓��̕������擾
	virtual const DirectX::SimpleMath::Vector3& GetUpPosition() const = 0;
public:
	// public�ȃ����o�֐�
	// �f�X�g���N�^
	virtual ~ICamera() = default;
	// ����������
	virtual void Initialize(int screenWidth, int screenHeight) = 0;
	// �X�V����
	virtual void Update() = 0;
private:
	// private�֐�
	// �r���[�s����v�Z����
	virtual void CalculateViewMatrix() = 0;
	// ���΃X�P�[�����v�Z����
	virtual void CalculateRerativeScale(int windowWidth, int windowHeight) = 0;
};