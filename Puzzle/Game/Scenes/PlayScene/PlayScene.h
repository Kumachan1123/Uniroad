/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
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
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/CSVItem/CSVItem.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
#include "Game/ControllScreen/UIBack/UIBack.h"
#include "Game/ControllScreen/NextTiles/NextTiles.h"
#include "Game/ControllScreen/Panel/Panel.h"
#include "Game/Mouse/Mouse.h"
#include "Game/ControllScreen/MedalCounter/MedalCounter.h"
#include "Game/Scenes/PlayScene/ResultAnimation/ResultAnimation.h"
#include "KumachiLib/Easing/Easing.h"
#include "Game/Scenes/PlayScene/ResultUI/ResultUI.h"
#include "Game/MainScreen/SpeedUpButton/SpeedUpButton.h"
#include "Game/Fade/Fade.h"
#include "Game/Sky/Sky.h"

// �O���錾
class CommonResources;
class CSVItem;
class MiniCharacterBase;
class NextTiles;
class Panel;
class MedalCounter;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

// ��ʓI�ȃV�[���N���X
class PlayScene : public IScene
{
public:
	// �A�N�Z�T
	// �V�[��ID���擾����
	SceneID GetNextSceneID() const override;
	// �X�e�[�W�ԍ����擾����
	int GetStageNumber() const override { return m_stageNumber; }
	// �X�e�[�W�ԍ���ݒ肷��
	void SetStageNumber(int stageNumber) override { m_stageNumber = stageNumber; }
public:
	// public�֐�
	// �R���X�g���N�^
	PlayScene(IScene::SceneID sceneID);
	// �f�X�g���N�^
	~PlayScene()override;
	// ������
	void Initialize(CommonResources* resources) override;
	// �X�V
	void Update(float elapsedTime)override;
	// �`��
	void Render()override;
	// �I��
	void Finalize()override;
private:
	// private�֐�
	// �J�����Ɋւ���ݒ������
	void CreateCamera();
	// �e��r���[�|�[�g��ݒ肷��
	void CreateViewports();
	// �f�o�b�O������\��
	void DrawDebugString();
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�b�O�J����
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// �Œ�J����(�v���C���j
	std::unique_ptr<FixedCamera> m_pFixedCameraPlay;
	// �Œ�J����(���U���g�p)
	std::unique_ptr<FixedCamera> m_pFixedCameraResult;
	// CSV�}�b�v
	std::unique_ptr<CSVMap> m_pCSVMap;
	// CSV�A�C�e��
	std::unique_ptr<CSVItem> m_pCSVItem;
	// �~�j�L�����x�[�X
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// �����ʂ̔w�i
	std::unique_ptr<UIBack> m_pUIBack;
	// �p�l��
	std::unique_ptr<Panel> m_pPanel;
	// ���̃^�C��
	std::unique_ptr<NextTiles> m_pNextTiles;
	// �}�E�X
	std::unique_ptr<MyMouse> m_pMouse;
	// ���_���J�E���^�[
	std::unique_ptr<MedalCounter> m_pMedalCounter;
	// ���ʃA�j���[�V����
	std::unique_ptr<ResultAnimation> m_pResultAnimation;
	// ����UI
	std::unique_ptr<ResultUI> m_pResultUI;
	// �X�s�[�h�A�b�v�{�^��
	std::unique_ptr<SpeedUpButton> m_pSpeedUpButton;
	// �t�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// ��
	std::unique_ptr<Sky> m_pSky;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �ˉe�s��(�Q�[����ʗp)
	DirectX::SimpleMath::Matrix m_projectionGame;
	// �ˉe�s��(�����ʗp)
	DirectX::SimpleMath::Matrix m_projectionControll;
	// �ˉe�s��(���U���g�p)
	DirectX::SimpleMath::Matrix m_projectionResult;
	// ���݂̃V�[��ID
	IScene::SceneID m_nowSceneID;
	// �Q�[����ʗp�r���[�|�[�g�@
	D3D11_VIEWPORT m_viewPortGame;
	// ����p�r���[�|�[�g
	D3D11_VIEWPORT m_viewPortControll;
	// ����
	float m_time;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �X�e�[�W�ԍ�
	int m_stageNumber;
	// �V�[���`�F���W�J�E���g
	float m_sceneChangeCount;
};
