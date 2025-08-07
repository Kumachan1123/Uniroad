/*
*	@file	PlayScene.cpp
*	@brief	�v���C�V�[���N���X
*/
#include <pch.h>
#include "PlayScene.h"
/*
*	@brief �R���X�g���N�^
*	@details �v���C�V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
PlayScene::PlayScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_debugCamera(nullptr) // �f�o�b�O�J�����ւ̃|�C���^
	, m_pFixedCameraPlay(nullptr) // �Œ�J�����ւ̃|�C���^
	, m_projectionGame() // �Q�[����ʗp�̎ˉe�s��
	, m_projectionControll() // �����ʗp�̎ˉe�s��
	, m_time(0.0f) // �o�ߎ���
	, m_sceneChangeCount(0.0f) // �V�[���ύX�J�E���g
	, m_isChangeScene(false) // �V�[���ύX�t���O
	, m_viewPortGame() // �Q�[����ʗp�̃r���[�|�[�g
	, m_viewPortControll() // �����ʗp�̃r���[�|�[�g
	, m_nowSceneID(sceneID)// ���݂̃V�[��ID
	, m_stageNumber(-1) // �X�e�[�W�ԍ�
{}
/*
*	@brief �f�X�g���N�^
*	@details �v���C�V�[���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayScene::~PlayScene() {}
/*
*	@brief ������
*	@details �v���C�V�[���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void PlayScene::Initialize(CommonResources* resources)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �e�J�������쐬
	CreateCamera();
	// �e��r���[�|�[�g���쐬����
	CreateViewports();
	// �f�o�C�X���\�[�X���擾
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// �}�E�X���쐬����
	m_pMouse = std::make_unique<MyMouse>();
	// �}�E�X������������
	m_pMouse->Initialize(m_pCommonResources);
	// �}�E�X�Ƀr���[�|�[�g��ݒ�
	m_pMouse->SetViewport(m_viewPortControll);
	// �X�e�[�W�ԍ��𕶎���ɂ���
	std::string stagePath = std::to_string(m_stageNumber);
	// �}�b�v����
	m_pCSVMap = std::make_unique<CSVMap>(m_pCommonResources);
	// CSV�}�b�v��ǂݍ���
	m_pCSVMap->LoadMap("Resources/Map/" + stagePath + "_map.csv");
	// CSV�A�C�e�����쐬����
	m_pCSVItem = std::make_unique<CSVItem>(m_pCommonResources);
	// CSV�A�C�e����ǂݍ���
	m_pCSVItem->LoadItem("Resources/Item/" + stagePath + "_item.csv");
	// �~�j�L�������쐬����
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// �~�j�L�����x�[�X��CSV�}�b�v��ݒ�
	m_pMiniCharacterBase->SetCSVMap(m_pCSVMap.get());
	// �~�j�L�����x�[�X��CSV�A�C�e����ݒ�
	m_pMiniCharacterBase->SetCSVItem(m_pCSVItem.get());
	// �~�j�L����������������
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// �~�j�L�����x�[�X�Ƀ~�j�L�������A�^�b�`
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacter>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));
	// �����ʂ̔w�i���쐬����
	m_pUIBack = std::make_unique<UIBack>(m_pCommonResources);
	// �����ʂ̔w�i������������
	m_pUIBack->Create(deviceResources);
	// �p�l�����쐬����
	m_pPanel = std::make_unique<Panel>(m_pCSVMap->GetMaxCol(), m_pCSVMap->GetMaxRow());
	// �p�l���Ƀ}�E�X��ݒ�
	m_pPanel->SetMouse(m_pMouse.get());
	// �p�l���Ƀ}�b�v����n��
	m_pPanel->SetCSVMap(m_pCSVMap.get());
	// �p�l���ɃA�C�e������n��
	m_pPanel->SetCSVItem(m_pCSVItem.get());
	// �p�l��������������
	m_pPanel->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// ���̃^�C�����쐬����
	m_pNextTiles = std::make_unique<NextTiles>();
	// ���̃^�C���Ƀ}�E�X��ݒ�
	m_pNextTiles->SetMouse(m_pMouse.get());
	// ���̃^�C���Ƀ}�b�v����n��
	m_pNextTiles->SetCSVMap(m_pCSVMap.get());
	// ���̃^�C��������������
	m_pNextTiles->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// �~�j�L�����x�[�X�Ɏ��̃^�C����ݒ�
	m_pMiniCharacterBase->SetNextTiles(m_pNextTiles.get());
	// ���_���J�E���^�[���쐬����
	m_pMedalCounter = std::make_unique<MedalCounter>();
	// ���_���J�E���^�[������������
	m_pMedalCounter->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// ���ʃA�j���[�V�������쐬����
	m_pResultAnimation = std::make_unique<ResultAnimation>();
	// ���ʃA�j���[�V����������������
	m_pResultAnimation->Initialize(m_pCommonResources);
	// ����UI���쐬����
	m_pResultUI = std::make_unique<ResultUI>();
	// ����UI������������
	m_pResultUI->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// ����UI�ɃX�e�[�W�ԍ���n��
	m_pResultUI->SetStageNum(m_stageNumber);
	// �X�s�[�h�A�b�v�{�^�����쐬����
	m_pSpeedUpUI = std::make_unique<SpeedUpUI>();
	// �X�s�[�h�A�b�v�{�^��������������
	m_pSpeedUpUI->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
}
/*
*   @brief �X�V����
*   @details �v���C�V�[���̑S�ẴQ�[���I�u�W�F�N�g�EUI�̍X�V
*   @param elapsedTime �O�t���[������̌o�ߎ���
*	@return �Ȃ�
*/
void PlayScene::Update(float elapsedTime)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// �o�ߎ��Ԃ����Z
	m_time += elapsedTime;
	// �X�s�[�h�A�b�v�{�^���̍X�V
	m_pSpeedUpUI->Update(elapsedTime);
	// �X�s�[�h�A�b�v�{�^���������ꂽ�ꍇ�A�Q�[�����o�ߎ��Ԃ�{�ɂ���
	float inGameTime = m_pSpeedUpUI->IsPressed() ? elapsedTime * 10.0f : elapsedTime;
	// ���ʃA�j���[�V�������L���Ȃ烊�U���g�p�̃J�����ɐ؂�ւ��ď����������s��
	if (m_pResultAnimation->IsAnimationEnable())
	{
		// �X�s�[�h�A�b�v�{�^���������I�ɃI�t�ɂ���
		m_pSpeedUpUI->SetPressed(false);
		// ���U���gUI�������ȏꍇ
		if (!m_pResultUI->IsEnable() && m_pResultAnimation->IsAnimationEnd())
		{
			// �L���ɂ���
			m_pResultUI->SetEnable(true);
			// ���ʂ̐ݒ�
			m_pResultUI->SetResult(m_pMiniCharacterBase->IsGameOver(), m_pMiniCharacterBase->IsGameClear());
		}
		// ���U���g�p�Œ�J�����̍X�V
		m_pFixedCameraResult->Update();
		// �r���[�s����擾
		m_view = m_pFixedCameraResult->GetViewMatrix();
		// ���ʃA�j���[�V�����̍X�V
		m_pResultAnimation->Update(elapsedTime);
		// ����UI�̍X�V
		m_pResultUI->Update(elapsedTime);
		// �Q�[���N���A�Ȃ�ȉ��̏������s��
		if (m_pMiniCharacterBase->IsGameClear())
		{
			// �J�����̈ʒu�����炩�ɕς���
			m_pFixedCameraResult->SetCameraDistance(Vector3(0.0f, 1.75f, 10.0f));
			Vector3 targetPos = m_pMiniCharacterBase->GetCameraPosition();
			// �J�����̃^�[�Q�b�g�ʒu���~�j�L�����̃J�����ʒu�ɐݒ�
			m_pFixedCameraResult->SetTargetPosition(Vector3(targetPos.x, targetPos.y + 3.0f, targetPos.z));
			// �J�����̍��W���X�V
			m_pFixedCameraResult->SetEyePosition(m_pMiniCharacterBase->GetCameraPosition() + m_pFixedCameraResult->GetCameraDistance());
			// ���̃X�e�[�W�ԍ����擾
			m_stageNumber = m_pResultUI->GetStageNum();
		}
	}
	// ���U���g�łȂ��ꍇ�͒ʏ�̍X�V���s��
	else
	{
		// �v���C��ʗp�Œ�J�����̍X�V
		m_pFixedCameraPlay->Update();
		// �r���[�s����擾
		m_view = m_pFixedCameraPlay->GetViewMatrix();
		// �}�E�X�̍X�V
		m_pMouse->Update(inGameTime);
		// �����ʂ̔w�i�̍X�V
		m_pUIBack->Update(inGameTime);
		// �p�l���̍X�V
		m_pPanel->Update(inGameTime);
		// ���̃^�C���̍X�V
		m_pNextTiles->Update(inGameTime);
		// ���_���J�E���^�[�Ɍ��݂̃��_������ݒ�
		m_pMedalCounter->SetCollectedMedalCount(m_pCSVItem->GetCollectedMedals());
		// ���_���J�E���^�[�̍X�V
		m_pMedalCounter->Update(inGameTime);
	}
	// CSV�A�C�e���̍X�V
	m_pCSVItem->Update(inGameTime);
	// �~�j�L�����̍X�V
	m_pMiniCharacterBase->Update(inGameTime, Vector3::Zero, Quaternion::Identity);
	// ���ʃA�j���[�V�����Ɍ��ʂ�n��
	m_pResultAnimation->SetResult(m_pMiniCharacterBase->IsGameOver(), m_pMiniCharacterBase->IsGameClear());
	// �A�j���[�V�������I�������V�[���ύX
	if (m_pResultAnimation->IsAnimationEnd() && m_pResultUI->GetSceneNum() > -1)m_isChangeScene = true;
}
/*
*	@brief �`�揈��
*	@details �v���C�V�[���̑S�ẴQ�[���I�u�W�F�N�g�EUI�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::Render()
{
	// �f�o�C�X�R���e�L�X�g���擾
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �o�̓T�C�Y���擾
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// ���ʃA�j���[�V�������L���łȂ��ꍇ�i�v���C���j�͓�̃r���[�|�[�g�ɕ����ĕ`��
	if (!m_pResultAnimation->IsAnimationEnable())
	{
		// --- ����: �Q�[����ʗp�r���[�|�[�g ---
		context->RSSetViewports(1, &m_viewPortGame);
		// �����ŃQ�[����ʂ�`��
		// CSV�}�b�v�̕`��
		m_pCSVMap->Render(m_view, m_projectionGame);
		// CSV�A�C�e���̕`��
		m_pCSVItem->Render(m_view, m_projectionGame);
		// �~�j�L�����̕`��
		m_pMiniCharacterBase->Render(m_view, m_projectionGame);
		// --- �E��: �����ʗp�r���[�|�[�g ---
		context->RSSetViewports(1, &m_viewPortControll);
		// �����ʂ̔w�i��`��
		m_pUIBack->Render();
		// �p�l��(�^�C��)��`��
		m_pPanel->DrawTiles();
		// �ݒu�ς݃^�C����`��
		m_pNextTiles->DrawPlacedTiles();
		// �p�l��(�A�C�e��)��`��
		m_pPanel->DrawItems();
		// �ݒu���̃^�C����`��
		m_pNextTiles->Render();
		// �r���[�|�[�g�����̐ݒ�ɖ߂�
		const auto& screenViewport = m_pCommonResources->GetDeviceResources()->GetScreenViewport();
		context->RSSetViewports(1, &screenViewport);
		// ���_���J�E���^�[�̕`��
		m_pMedalCounter->Render();
		// �X�s�[�h�A�b�v�{�^���̕`��
		m_pSpeedUpUI->Render();
	}
	// ���ʃA�j���[�V�������L���ȏꍇ�͈�̃r���[�|�[�g�ł̕`��
	if (m_pResultAnimation->IsAnimationEnable())
	{
		// ���U���g�p�Œ�J�����̃r���[�s����擾
		m_pCSVMap->Render(m_view, m_projectionResult);
		// CSV�A�C�e���̕`��
		m_pCSVItem->Render(m_view, m_projectionResult);
		// �~�j�L�����̕`��
		m_pMiniCharacterBase->Render(m_view, m_projectionResult);
		// ���ʃA�j���[�V�����̕`��
		m_pResultAnimation->Render();
		// ����UI�̕`��
		if (m_pResultAnimation->IsAnimationEnd())m_pResultUI->Render();
	}

}
/*
*	@brief �I��
*	@details �v���C�V�[���N���X�̏I�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::Finalize() {}
/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		auto sceneID = m_pResultUI->GetSceneNum();
		switch (sceneID)
		{
		case ResultUI::REPLAY: // ���v���C�I��
			// ���v���C��ʂ�
			return IScene::SceneID::PLAY;
		case ResultUI::SELECT_STAGE:// �X�e�[�W�Z���N�g�I��
			// �X�e�[�W�Z���N�g��ʂ�
			return IScene::SceneID::STAGESELECT;
		}

	}
	// �V�[���ύX���Ȃ��ꍇ�������Ȃ�
	return IScene::SceneID::NONE;
}
/*
*	@brief �J�������쐬����
*	@details �v���C���̌Œ�J�����ƃ��U���g�p�̌Œ�J�������쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �o�̓T�C�Y���擾����
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// �v���C���̌Œ�J�������쐬����
	m_pFixedCameraPlay = std::make_unique<FixedCamera>();
	m_pFixedCameraPlay->Initialize((int)(rect.right * 0.7f), rect.bottom);
	// ���U���g�p�Œ�J�������쐬����
	m_pFixedCameraResult = std::make_unique<FixedCamera>();
	m_pFixedCameraResult->Initialize(rect.right, rect.bottom);
	// �ˉe�s��(�Q�[����ʗp)���쐬����
	m_projectionGame = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.7f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
	// �ˉe�s��(�����ʗp)���쐬����
	m_projectionControll = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.3f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
	// �ˉe�s��(���U���g�p)���쐬����
	m_projectionResult = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief �r���[�|�[�g���쐬����
*	@details �v���C�V�[���̃r���[�|�[�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::CreateViewports()
{
	// �o�̓T�C�Y���擾����
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// --- ����: �Q�[����ʗp�r���[�|�[�g�̐ݒ� ---
	D3D11_VIEWPORT viewportLeft = {};
	// ����̍��W��ݒ�
	viewportLeft.TopLeftX = 0;
	viewportLeft.TopLeftY = 0;
	// ���ƍ�����ݒ�
	viewportLeft.Width = (FLOAT)rect.right * 0.7f; // ��7��
	viewportLeft.Height = (FLOAT)rect.bottom;
	// �[�x�͈̔͂�ݒ�
	viewportLeft.MinDepth = 0.0f;
	viewportLeft.MaxDepth = 1.0f;
	// �Q�[����ʗp�r���[�|�[�g��ݒ�
	m_viewPortGame = viewportLeft;
	// --- �E��: �����ʗp�r���[�|�[�g�̐ݒ� ---
	D3D11_VIEWPORT viewportRight = {};
	// ����̍��W��ݒ�
	viewportRight.TopLeftX = (FLOAT)rect.right * 0.7f;
	viewportRight.TopLeftY = 0;
	// ���ƍ�����ݒ�
	viewportRight.Width = (FLOAT)rect.right * 0.3f; // �E3��
	viewportRight.Height = (FLOAT)rect.bottom;
	// �[�x�͈̔͂�ݒ�
	viewportRight.MinDepth = 0.0f;
	viewportRight.MaxDepth = 1.0f;
	// �����ʗp�r���[�|�[�g��ݒ�
	m_viewPortControll = viewportRight;
}
/*
*	@brief �f�o�b�O�������`�悷��
*	@details �v���C�V�[���̃f�o�b�O�������`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayScene::DrawDebugString()
{
	// �f�o�b�O��������擾
	const auto debugString = m_pCommonResources->GetDebugString();
	// �Q�[���I�[�o�[��Q�[���N���A�̏�Ԃ��f�o�b�O������ɒǉ�
	debugString->AddString("IsGameOver:%s", m_pMiniCharacterBase->IsGameOver() ? "true" : "false");
	debugString->AddString("IsGameClear:%s", m_pMiniCharacterBase->IsGameClear() ? "true" : "false");
	// �J�����̈ʒu�Ɣ�ʑ̍��W���f�o�b�O������ɒǉ�
	debugString->AddString("CameraEye:%f,%f,%f", m_pFixedCameraPlay->GetEyePosition().x, m_pFixedCameraPlay->GetEyePosition().y, m_pFixedCameraPlay->GetEyePosition().z);
	debugString->AddString("CameraTarget:%f,%f,%f", m_pFixedCameraPlay->GetTargetPosition().x, m_pFixedCameraPlay->GetTargetPosition().y, m_pFixedCameraPlay->GetTargetPosition().z);
	// ���������{�^��
	debugString->AddString("HitButtonIndex:%i", m_pResultUI->GetSceneNum());
	debugString->AddString("StageNum:%i", m_stageNumber);
}
