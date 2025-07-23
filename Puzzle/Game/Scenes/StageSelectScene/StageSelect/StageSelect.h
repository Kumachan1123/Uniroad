/*
*	@file StageSelect.h
*	@brief �X�e�[�W�Z���N�g�V�[���̃X�e�[�W�Z���N�g�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

class StageSelect
{
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

};