/*
*	@file StageSelect.h
*	@brief �X�e�[�W�Z���N�g�V�[���̃X�e�[�W�Z���N�g�N���X
*/
#pragma once

// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// �X�e�[�W�Z���N�g���f���N���X
class StageSelect
{
public:
	// �A�N�Z�T
	// �V���h�E�}�b�v���C�g���擾����
	ShadowMapLight* GetShadowMapLight() const { return m_pShadowMapLight; }
	// �V���h�E�}�b�v���C�g��ݒ肷��
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) { m_pShadowMapLight = shadowMapLight; }
public:
	// public�֐�
	// �R���X�g���N�^
	StageSelect(CommonResources* resources);
	// �f�X�g���N�^
	~StageSelect();
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
	// �V���h�E�}�b�v���C�g
	ShadowMapLight* m_pShadowMapLight;
};