/*
	@file	PlayScene.cpp
	@brief	��ʓI�ȃV�[���N���X
*/
#include <pch.h>
#include "PlayScene.h"

PlayScene::PlayScene(IScene::SceneID sceneID)
	: m_pCommonResources{}
	, m_debugCamera{}
	, m_pFixedCamera{}
	, m_projectionGame{}
	, m_projectionControll{}
	, m_time{ 0.0f }
	, m_isChangeScene{}
	, m_viewPortGame{}
	, m_viewPortControll{}
	, m_nowSceneID{ sceneID }// ���݂̃V�[��ID

{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;
	CreateCamera();
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// �e��r���[�|�[�g���쐬����
	CreateViewports();
	// �}�E�X���쐬����
	m_pMouse = std::make_unique<MyMouse>();
	// �}�E�X������������
	m_pMouse->Initialize(m_pCommonResources);
	// �}�E�X�Ƀr���[�|�[�g��ݒ�
	m_pMouse->SetViewport(m_viewPortControll);
	// �}�b�v����
	m_pCSVMap = std::make_unique<CSVMap>(m_pCommonResources);
	// CSV�}�b�v��ǂݍ���
	m_pCSVMap->LoadMap("Resources/Map/test.csv");
	// CSV�A�C�e�����쐬����
	m_pCSVItem = std::make_unique<CSVItem>(m_pCommonResources);
	// CSV�A�C�e����ǂݍ���
	m_pCSVItem->LoadItem("Resources/Item/test.csv");
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

}

void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;
	//// �f�o�b�O�J�����̍X�V
	//m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// �Œ�J�����̍X�V
	m_pFixedCamera->Update();
	// �}�E�X�̍X�V
	m_pMouse->Update(elapsedTime);
	// �����ʂ̔w�i�̍X�V
	m_pUIBack->Update(elapsedTime);
	// CSV�A�C�e���̍X�V
	m_pCSVItem->Update(elapsedTime);

	// �p�l���̍X�V
	m_pPanel->Update(elapsedTime);
	// ���̃^�C���̍X�V
	m_pNextTiles->Update(elapsedTime);
	// �~�j�L�����̍X�V
	Vector3 position(0.0f, 0.0f, 0.0f);
	Quaternion angle(Quaternion::Identity);
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	// ���_���J�E���^�[�Ɍ��݂̃��_������ݒ�
	m_pMedalCounter->SetCollectedMedalCount(m_pCSVItem->GetCollectedMedals());

	// ���_���J�E���^�[�̍X�V
	m_pMedalCounter->Update(elapsedTime);
}
void PlayScene::Render()
{
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();

	// --- ����: �Q�[����ʗp�r���[�|�[�g ---
	context->RSSetViewports(1, &m_viewPortGame);

	// �����ŃQ�[����ʂ�`��
	// �r���[�s����擾
	m_view = m_pFixedCamera->GetViewMatrix();
	//m_view = m_debugCamera->GetViewMatrix();
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


	// --- �f�o�b�O���i��j ---
	// �r���[�|�[�g�����̐ݒ�ɖ߂�
	const auto& viewPort = m_pCommonResources->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewPort);
	// ���_���J�E���^�[�̕`��
	m_pMedalCounter->Render();

	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("Use ViewPort.");
}

void PlayScene::Finalize()
{
}

IScene::SceneID PlayScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::STAGESELECT;// �Q�[���I�[�o�[�V�[����
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;// �������Ȃ�
}

//---------------------------------------------------------
// �J�����A�r���[�s��A�ˉe�s����쐬����
//---------------------------------------------------------
void PlayScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �o�̓T�C�Y���擾����
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	//// �f�o�b�O�J�������쐬����
	//m_debugCamera = std::make_unique<mylib::DebugCamera>();
	//m_debugCamera->Initialize(rect.right * 0.7f, rect.bottom);
	// �Œ�J�������쐬����
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right * 0.7f), rect.bottom);

	// �ˉe�s����쐬����
	m_projectionGame = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.7f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
	m_projectionControll = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right * 0.3f) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}

void PlayScene::CreateViewports()
{
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// --- ����: �Q�[����ʗp�r���[�|�[�g ---
	D3D11_VIEWPORT viewportLeft = {};
	viewportLeft.TopLeftX = 0;
	viewportLeft.TopLeftY = 0;
	viewportLeft.Width = (FLOAT)rect.right * 0.7f; // ��: ��7��
	viewportLeft.Height = (FLOAT)rect.bottom;
	viewportLeft.MinDepth = 0.0f;
	viewportLeft.MaxDepth = 1.0f;
	m_viewPortGame = viewportLeft;

	// --- �E��: �����ʗp�r���[�|�[�g ---
	D3D11_VIEWPORT viewportRight = {};
	viewportRight.TopLeftX = (FLOAT)rect.right * 0.7f;
	viewportRight.TopLeftY = 0;
	viewportRight.Width = (FLOAT)rect.right * 0.3f; // ��: �E3��
	viewportRight.Height = (FLOAT)rect.bottom;
	viewportRight.MinDepth = 0.0f;
	viewportRight.MaxDepth = 1.0f;
	m_viewPortControll = viewportRight;
}
