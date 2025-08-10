/*
*	@file	TitleScene.h
*	@brief	�^�C�g���V�[���N���X
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
// ����w�b�_�[�t�@�C��
#include "Game/SceneManager/IScene.h"
#include "Game/CommonResources/CommonResources.h"
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/Scenes/TitleScene/TitleLogo/TitleLogo.h"

// �O���錾
class CommonResources;

// �^�C�g���V�[���N���X
class TitleScene : public IScene
{
public:
	// �A�N�Z�T
	// �X�e�[�W�ԍ����擾����
	int GetStageNumber() const override { return 0; }
	// �X�e�[�W�ԍ���ݒ肷��
	void SetStageNumber(int stageNumber) override { stageNumber; }
public:
	// public�֐�
	// �R���X�g���N�^
	TitleScene(IScene::SceneID sceneID);
	// �f�X�g���N�^
	~TitleScene() override;
	// ������
	void Initialize(CommonResources* resources) override;
	// �X�V
	void Update(float elapsedTime) override;
	// �`��
	void Render() override;
	// �I��
	void Finalize() override;
	// �V�[��ID���擾����
	SceneID GetNextSceneID() const override;

private:
	// private�֐�
	// �J�����Ɋւ���ݒ������
	void CreateCamera();

private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���̃V�[��ID
	IScene::SceneID m_nextSceneID;
	// �J����
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// ���S
	std::unique_ptr<TitleLogo> m_pTitleLogo;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
};