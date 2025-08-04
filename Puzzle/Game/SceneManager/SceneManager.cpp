/*
*	@file	SceneManager.cpp
*	@brief	�V�[���}�l�[�W���N���X
*/
#include <pch.h>
#include "SceneManager.h"
/*
*	@brief �R���X�g���N�^
*	@details �V�[���}�l�[�W���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SceneManager::SceneManager()
	: m_pCurrentScene{}// ���݂̃V�[��
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_stageNumber{ 0 }// �X�e�[�W�ԍ�
	, m_nowSceneID{ IScene::SceneID::NONE }// ���݂̃V�[��ID
{
}
/*
*	@brief �f�X�g���N�^
*	@details �V�[���}�l�[�W���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return	�Ȃ�
*/
SceneManager::~SceneManager() { Finalize(); }
/*
*	@brief ����������
*	@details �V�[���}�l�[�W���N���X�̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void SceneManager::Initialize(CommonResources* resources)
{
	// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	assert(resources);
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �^�C�g���V�[���ɕύX
	ChangeScene(IScene::SceneID::STAGESELECT);
}
/*
*	@brief �X�V����
*	@details �V�[���}�l�[�W���N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void SceneManager::Update(float elapsedTime)
{
	// ���݂̃V�[�����X�V
	m_pCurrentScene->Update(elapsedTime);
	// ���̃V�[��ID��NONE�̏ꍇ�͂����ŏ������I���
	if (m_pCurrentScene->GetNextSceneID() == IScene::SceneID::NONE) return;
	// �V�[����ύX����Ƃ�
	ChangeScene(m_pCurrentScene->GetNextSceneID());
}
/*
*	@brief �`�悷��
*	@details �V�[���}�l�[�W���N���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::Render()
{
	// ���݂̃V�[����`�悷��
	m_pCurrentScene->Render();
}
/*
*	@brief �I������
*	@details �V�[�����폜����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::Finalize()
{
	// ���݂̃V�[�����폜����
	DeleteScene();
}

/*
*	@brief �V�[����ύX����
*	@details ���̃V�[���������ĐV�����V�[�����쐬����
*	@param sceneID �V�����V�[����ID
*	@return �Ȃ�
*/
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{

	// �V�[�����폜����
	DeleteScene();
	// �V�����V�[�����쐬����
	CreateScene(sceneID);
}

/*
*	@brief �V�[�����쐬����
*	@details �V�����V�[�����쐬����
*	@param sceneID �V�����V�[����ID
*	@return �Ȃ�
*/
void SceneManager::CreateScene(IScene::SceneID sceneID)
{

	// ���݂̃V�[����nullptr�ł��邱�Ƃ��m�F
	assert(m_pCurrentScene == nullptr);
	// �V�[��ID�ɂ���ď����𕪂���
	switch (sceneID)
	{
	case IScene::SceneID::STAGESELECT:
		m_pCurrentScene = std::make_unique<StageSelectScene>(sceneID);
		break;
	case IScene::SceneID::PLAY:
		m_pCurrentScene = std::make_unique<PlayScene>(sceneID);
		m_pCurrentScene->SetStageNumber(m_stageNumber); // �X�e�[�W�ԍ���ݒ�
		break;
		/*case IScene::SceneID::TITLE:
			m_currentScene = std::make_unique<TitleScene>(sceneID);
			break;
		case IScene::SceneID::SETTING:
			m_currentScene = std::make_unique<SettingScene>(sceneID);
			break;
		case IScene::SceneID::STAGESELECT:
			m_currentScene = std::make_unique<StageSelectScene>(sceneID);
			break;
		case IScene::SceneID::PLAY:
			m_currentScene = std::make_unique<PlayScene>(sceneID);

			break;
		case IScene::SceneID::CLEAR:
		case IScene::SceneID::GAMEOVER:

			m_currentScene = std::make_unique<ResultScene>(sceneID);
			break;*/
	default:
		assert(!"SceneManager::CreateScene::�V�[���������݂��܂���I");
		// no break
	}


	m_pCurrentScene->Initialize(m_pCommonResources);
	SetSceneID(sceneID);
}
/*
*	@brief �V�[�����폜����
*	@details ���݂̃V�[�����폜����(�Z���N�g�V�[���̏ꍇ�͏����O�ɃX�e�[�W�ԍ���ۑ�����)
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::DeleteScene()
{
	// �X�e�[�W�Z���N�g�V�[���̏ꍇ�́A�X�e�[�W�ԍ����}�l�[�W���[�ɓn��
	if (m_nowSceneID == IScene::SceneID::STAGESELECT)m_stageNumber = m_pCurrentScene->GetStageNumber();
	// ���݂̃V�[�������Z�b�g����
	m_pCurrentScene.reset();
}


