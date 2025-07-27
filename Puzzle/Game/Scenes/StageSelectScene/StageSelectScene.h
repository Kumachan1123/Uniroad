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
#include "Game/Camera/FixedCamera/FixedCamera.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBase/MiniCharacterBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterSelectStage/MiniCharacterSelectStage.h"
#include "Game/Scenes/StageSelectScene/StageSelect/StageSelect.h"
#include "Game/Scenes/StageSelectScene/PlaneArea/PlaneArea.h"

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
	// �J�����Ɋւ���ݒ������
	void CreateCamera();
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �J����
	std::unique_ptr<FixedCamera> m_pFixedCamera;
	// �~�j�L�����x�[�X
	std::unique_ptr<MiniCharacterBase> m_pMiniCharacterBase;
	// �X�e�[�W�Z���N�g
	std::unique_ptr<StageSelect> m_pStageSelect;
	// CSV�}�b�v
	std::unique_ptr<CSVMap> m_pCSVMap;
	// ����
	std::unique_ptr<PlaneArea> m_pPlaneArea;
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
};

