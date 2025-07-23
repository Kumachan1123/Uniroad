/*
*	@file StageSelectScene.cpp
*	@brief �X�e�[�W�I���V�[���N���X
*/
#include "pch.h"
#include "StageSelectScene.h"

/*
*	@brief �R���X�g���N�^
*	@details �X�e�[�W�I���V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
StageSelectScene::StageSelectScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pFixedCamera(nullptr) // �J�����ւ̃|�C���^
	, m_pMiniCharacterBase(nullptr) // �~�j�L�����x�[�X�ւ̃|�C���^
	, m_view() // �r���[�s��
	, m_projection() // �ˉe�s��
	, m_isChangeScene(false) // �V�[���ύX�t���O
	, m_stageNumber(0) // �X�e�[�W�ԍ�
	, m_nextSceneID(sceneID) // ���̃V�[��ID
{
}
/*
*	@brief �f�X�g���N�^
*	@details �X�e�[�W�I���V�[���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
StageSelectScene::~StageSelectScene()
{
	// �������Ȃ�
}

/*
*	@brief ������
*	@details �X�e�[�W�I���V�[���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void StageSelectScene::Initialize(CommonResources* resources)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �J�������쐬����
	CreateCamera();
	// �f�o�C�X���\�[�X���擾����
	//const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// �e��r���[�|�[�g���쐬����
	CreateViewports();
	//// �}�b�v����
	//m_pCSVMap = std::make_unique<CSVMap>(m_pCommonResources);
	//// CSV�}�b�v��ǂݍ���
	//m_pCSVMap->LoadMap("Resources/Map/Title.csv");
	// �~�j�L�������쐬����
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// �~�j�L�����x�[�X��CSV�}�b�v��ݒ�
	m_pMiniCharacterBase->SetCSVMap(nullptr);
	// �~�j�L�����x�[�X��CSV�A�C�e����ݒ�
	m_pMiniCharacterBase->SetCSVItem(nullptr);
	// �~�j�L�����x�[�X�Ɏ��̃^�C����ݒ�
	m_pMiniCharacterBase->SetNextTiles(nullptr);
	// �~�j�L����������������
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// �~�j�L�����x�[�X�Ƀ~�j�L�������A�^�b�`
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterSelectStage>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));
	// �X�e�[�W�Z���N�g���쐬����
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// �X�e�[�W�Z���N�g������������
	m_pStageSelect->Initialize();
}
/*
*	@brief �X�V
*	@details �X�e�[�W�I���V�[���N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void StageSelectScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �Œ�J�����̍X�V
	m_pFixedCamera->Update();
	// �~�j�L�����̍X�V
	Vector3 position(0.0f, 0.0f, 0.0f);
	Quaternion angle(Quaternion::Identity);
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	const auto& keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	if (keyState.Space)
	{
		// �X�y�[�X�L�[�������ꂽ��V�[���ύX�t���O�𗧂Ă�
		m_isChangeScene = true;
	}
}

/*
*	@brief �`��
*	@details �X�e�[�W�I���V�[���N���X�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::Render()
{
	//const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �r���[�s����擾
	m_view = m_pFixedCamera->GetViewMatrix();
	//// CSV�}�b�v�̕`��
	//m_pCSVMap->Render(m_view, m_projection);
	// �X�e�[�W�Z���N�g�̕`��
	m_pStageSelect->Render(m_view, m_projection);
	// �~�j�L�����̕`��
	m_pMiniCharacterBase->Render(m_view, m_projection);
	// --- �f�o�b�O���i��j ---
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("Use ViewPort.");
}

/*
*	@brief �I��
*	@details �X�e�[�W�I���V�[���N���X�̏I�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::Finalize()
{
}


/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		// �Q�[���I�[�o�[�V�[����
		return IScene::SceneID::PLAY;
	}
	// �V�[���ύX���Ȃ��ꍇ�������Ȃ�
	return IScene::SceneID::NONE;
}

/*
*	@brief �J�����Ɋւ���ݒ������
*	@details �X�e�[�W�I���V�[���N���X�̃J�����ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::CreateCamera()
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
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);

	// �ˉe�s����쐬����
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief �e��r���[�|�[�g��ݒ肷��
*	@details �X�e�[�W�I���V�[���N���X�̃r���[�|�[�g�ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::CreateViewports()
{
}


