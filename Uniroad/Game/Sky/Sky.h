/*
*	@file Sky.h
*	@brief ��̕`����s���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <memory>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

// �O���錾
class CommonResources;

// ��̕`����s���N���X
class Sky
{
public:
	// �A�N�Z�T
	// ���W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// �X�P�[�����擾����
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// �X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// ��]���x���擾����
	float GetRotationSpeed() const { return m_rotationSpeed; }
	// ��]���x��ݒ肷��
	void SetRotationSpeed(float rotationSpeed) { m_rotationSpeed = rotationSpeed; }

public:
	// public�֐�
	// �R���X�g���N�^
	Sky(CommonResources* resources);
	// �f�X�g���N�^
	~Sky();
	// ������
	void Initialize();
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �I��
	void Finalize();

private:
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// ��̈ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ��̉�]�p
	DirectX::SimpleMath::Quaternion m_rotation;
	// ��̃X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// ��̃��f���ւ̃|�C���^
	DirectX::Model* m_pModel;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�}�g���b�N�X
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����}�g���b�N�X
	DirectX::SimpleMath::Matrix m_proj;
	// ����
	float m_time;
	// ��]���x
	float m_rotationSpeed;
	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
};