/*
*	@file StageGate.h
*	@brief �X�e�[�W�Q�[�g�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// �X�e�[�W�Q�[�g�N���X
class StageGate
{
public:
	// �A�N�Z�T
	// ���W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// �V���h�E�}�b�v���C�g���擾����
	ShadowMapLight* GetShadowMapLight() const { return m_pShadowMapLight; }
	// �V���h�E�}�b�v���C�g��ݒ肷��
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) { m_pShadowMapLight = shadowMapLight; }

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
	// �V���h�E�}�b�v���C�g
	ShadowMapLight* m_pShadowMapLight;
	// ���f���|�C���^�[
	DirectX::Model* m_pModel;
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;

};