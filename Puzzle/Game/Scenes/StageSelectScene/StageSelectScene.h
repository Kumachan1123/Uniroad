/*
*	@file StageSelectScene.h
*	@brief �X�e�[�W�Z���N�g�V�[���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <memory>
// DirectX
#include <DeviceResources.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/SceneManager/IScene.h"
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/MouseClick/MouseClick.h"
#include "KumachiLib/FileCounter/FileCounter.h"
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/Camera/TrackingCamera/TrackingCamera.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterSelectStage/MiniCharacterSelectStage.h"
#include "Game/Scenes/StageSelectScene/StageSelect/StageSelect.h"
#include "Game/Scenes/StageSelectScene/PlaneArea/PlaneArea.h"
#include "Game/Scenes/StageSelectScene/StageGate/StageGate.h"
#include "Game/Sky/Sky.h"
#include "Game/Fade/Fade.h"

// �O���錾
class CommonResources;
class MiniCharacterBase;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

// �X�e�[�W�Z���N�g�V�[���N���X
class StageSelectScene : public IScene
{
public:
	// �A�N�Z�T
	// �X�e�[�W�ԍ����擾����
	int GetStageNumber() const override { return m_stageNumber; }
	// �X�e�[�W�ԍ���ݒ肷��
	void SetStageNumber(int stageNumber) override { m_stageNumber = stageNumber; }
public:
	// public�֐�
	// �R���X�g���N�^
	StageSelectScene(IScene::SceneID sceneID);
	// �f�X�g���N�^
	~StageSelectScene() override;
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
	// ���ʂ̒��S���W����ɂS�̒��_��ݒ肷��
	std::vector<DirectX::SimpleMath::Vector3> CreatePlaneVertices(const DirectX::SimpleMath::Vector3& center, float width, float depth, float y = 0.5f) const;
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �Œ�J����
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// �g���b�L���O�J����
	std::unique_ptr<TrackingCamera> m_pTrackingCamera;
	// �f�o�b�O�J����
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	// �~�j�L�����x�[�X
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// �X�e�[�W�Z���N�g
	std::unique_ptr<StageSelect> m_pStageSelect;
	// �X�e�[�W�̓����
	std::vector<std::unique_ptr<StageGate>> m_pStageGates;
	// ����
	std::unique_ptr<PlaneArea> m_pPlaneArea;
	// ��
	std::unique_ptr<Sky> m_pSky;
	// �t�F�[�h
	std::unique_ptr<Fade> m_pFade;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �X�e�[�W�ԍ�
	int m_stageNumber;
	// ���̃V�[��ID
	IScene::SceneID m_nextSceneID;
	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	// �O���b�h��
	std::unique_ptr<mylib::GridFloor> m_pGridFloor;
	// �~�j�L�����̈ړ��t���O
	bool m_isMiniCharacterMove;
};

