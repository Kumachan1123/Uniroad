/*
*	@file StageGate.h
*	@brief �X�e�[�W�Q�[�g�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

class StageGate
{
public:
	// �A�N�Z�T
	// ���W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
public:
	// public�֐�
	// �R���X�g���N�^
	StageGate(CommonResources* resources);
	// �f�X�g���N�^
	~StageGate();
	// ������
	void Initialize();
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �I��
	void Finalize();
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���f���|�C���^�[
	DirectX::Model* m_pModel;
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;

};