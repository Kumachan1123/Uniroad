/*
*	@file	SceneManager.h
*	@brief	�V�[���}�l�[�W���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "IScene.h"
#include "Game/Scenes/StageSelectScene/StageSelectScene.h"
#include "Game/Scenes/PlayScene/PlayScene.h"

// �O���錾
class CommonResources;

// �V�[���}�l�[�W���N���X
class SceneManager
{

public:
	// public�����o�֐�
	// �R���X�g���N�^
	SceneManager();
	// �f�X�g���N�^
	~SceneManager();
	// ������
	void Initialize(CommonResources* resources);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �I��
	void Finalize();
private:
	// private�����o�֐�
	// �V�[���ύX
	void ChangeScene(IScene::SceneID sceneID);
	// �V�[���쐬
	void CreateScene(IScene::SceneID sceneID);
	// �V�[���폜
	void DeleteScene();
	// �V�[��ID�擾
	IScene::SceneID GetSceneID() const { return m_nowSceneID; }
	// �V�[��ID�ݒ�
	void SetSceneID(IScene::SceneID sceneID) { m_nowSceneID = sceneID; }
private:
	// private�����o�ϐ�
	// ���݂̃V�[��
	std::unique_ptr<IScene> m_pCurrentScene;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
	// �v���C�V�[���ɃX�e�[�W�ԍ���n�����߂̕ϐ�
	int m_stageNumber;

};
