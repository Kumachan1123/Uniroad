/*
	@file	IScene.h
	@brief	�V�[���̃C���^�[�t�F�C�X�N���X
*/
#pragma once

// �O���錾
class CommonResources;

// �V�[���̃C���^�[�t�F�C�X�N���X
class IScene
{
public:
	// �V�[���h�c
	enum class SceneID : unsigned int
	{
		NONE,
		STAGESELECT,
		PLAY,

	};


public:
	// public�����o�֐�
	// �f�X�g���N�^
	virtual ~IScene() = default;
	// ������
	virtual void Initialize(CommonResources* resources) = 0;
	// �X�V
	virtual void Update(float elapsedTime) = 0;
	// �`��
	virtual void Render() = 0;
	// �I��
	virtual void Finalize() = 0;
	// ���̃V�[��ID���擾����
	virtual SceneID GetNextSceneID() const = 0;
	// �X�e�[�W�ԍ����擾����
	virtual int GetStageNumber() const = 0;
	// �X�e�[�W�ԍ���ݒ肷��
	virtual void SetStageNumber(int stageNumber) = 0;
};
