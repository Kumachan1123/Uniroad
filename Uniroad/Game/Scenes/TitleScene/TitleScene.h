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
#include "Game/Scenes/TitleScene/TitleButton/TitleButton.h"
#include "Game/Fade/Fade.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterTitle/MiniCharacterTitle.h"
#include "Game/Scenes/TitleScene/TitleAnimationState/TitleAnimationState.h"
#include "Game/Sky/Sky.h"
#include "Game/Scenes/TitleScene/Road/Road.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// �O���錾
class CommonResources;
class MiniCharacterBase;

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
	// private�萔
	// �t�F�[�h�J�n����
	static const float FADE_START_TIME;
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���̃V�[��ID
	IScene::SceneID m_nextSceneID;
	// �J����
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// �f�o�b�O�J����
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// ���S
	std::unique_ptr<TitleLogo> m_pTitleLogo;
	// �{�^��
	std::unique_ptr<TitleButton> m_pTitleButton;
	// �t�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �~�j�L�����x�[�X
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// ��
	std::unique_ptr<Sky> m_pSky;
	// ��
	std::unique_ptr<Road> m_pRoad;

	// �V���h�E�}�b�v���C�g
	std::unique_ptr<ShadowMapLight> m_pShadowMapLight;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// ����
	float m_time;
};