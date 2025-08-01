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
	, m_isMiniCharacterMove(false) // �~�j�L�������ړ��������ǂ���
	, m_stageNumber(-1) // �X�e�[�W�ԍ�
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
	// �X�e�[�W�Z���N�g���쐬����
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// �X�e�[�W�Z���N�g������������
	m_pStageSelect->Initialize();
	// ���ʂ��쐬����
	m_pPlaneArea = std::make_unique<PlaneArea>(m_pCommonResources);
	// ���_�ݒ�
	for (int i = 0; i < FileCounter::CountFilesInFolder("Resources/Map/", ".csv"); ++i)
	{
		// ���S���W���v�Z
		Vector3 center(4.0f * (float)i, 0.5f, 2.0f);
		// ����
		float width = 2.0f;
		// ���s��
		float depth = 2.0f;
		// ���ʂ̒��_���쐬
		std::vector<Vector3> vertices = CreatePlaneVertices(center, width, depth, center.y);
		// ���ʂɒ��_�z���o�^
		m_pPlaneArea->AddPlane(vertices);
		// ���ʂ̒��S���W��o�^����
		m_pPlaneArea->AddPlanePosition(center);
		// ���ʂ̐F��Ԃɐݒ�
		m_pPlaneArea->SetPlaneColor(Color(1, 0, 0));
		// �X�e�[�W�̓�������쐬����
		m_pStageGates.push_back(std::make_unique<StageGate>(m_pCommonResources));
		// �X�e�[�W�̓����������������
		m_pStageGates.back()->Initialize();
		// �X�e�[�W�̓�����̈ʒu��ݒ�
		m_pStageGates.back()->SetPosition(center);
	}
	// ���ʂ�����������
	m_pPlaneArea->Initialize();
	// �~�j�L�������쐬����
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// �~�j�L�����x�[�X��CSV�}�b�v��ݒ�
	m_pMiniCharacterBase->SetCSVMap(nullptr);
	// �~�j�L�����x�[�X��CSV�A�C�e����ݒ�
	m_pMiniCharacterBase->SetCSVItem(nullptr);
	// �~�j�L�����x�[�X�Ɏ��̃^�C����ݒ�
	m_pMiniCharacterBase->SetNextTiles(nullptr);
	// �~�j�L�����ɕ��ʂ�ݒ�
	m_pMiniCharacterBase->SetPlaneArea(m_pPlaneArea.get());
	// �~�j�L����������������
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// �~�j�L�����x�[�X�Ƀ~�j�L�������A�^�b�`
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterSelectStage>(m_pMiniCharacterBase.get(), Vector3(0.0f, -0.5f, 0.0f), 0.0f));

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
	// �g���b�L���O�J�����ɒǏ]�Ώۂ̍��W��ݒ�
	m_pTrackingCamera->SetTargetPosition(m_pMiniCharacterBase->GetCameraPosition());
	// �g���b�L���O�J�����̍X�V
	m_pTrackingCamera->Update();
	// �f�o�b�O�J�����̍X�V
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	//#ifndef DEBUG
	//	const auto KeyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	//	if (KeyState.Space)
	//		m_view = m_pTrackingCamera->GetViewMatrix();
	//	else
	//		m_view = m_pFixedCamera->GetViewMatrix();
	//
	//#endif
		// �r���[�s����擾
	m_view = m_pTrackingCamera->GetViewMatrix();
	// ���W��������
	Vector3 position(0.0f, -0.5f, -1.75f);
	// �p�x��������
	Quaternion angle(Quaternion::Identity);
	// �~�j�L�����̍X�V
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	// ���ʂɃr���[�s���n��
	m_pPlaneArea->SetView(m_view);
	// ���ʂɎˉe�s���n��
	m_pPlaneArea->SetProjection(m_projection);
	// ���ʂ��X�V
	m_pPlaneArea->Update(elapsedTime);
	// �X�e�[�g�̓�������X�V
	for (auto& gate : m_pStageGates)gate->Update(elapsedTime);
	// �}�E�X�X�e�[�g���擾
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// ���N���b�N�����m
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_pPlaneArea->GetHitPlaneIndex() > -1)
	{
		// �X�e�[�W�ԍ����擾
		m_stageNumber = m_pPlaneArea->GetHitPlaneIndex();
		// �V�[���J��
		m_isChangeScene = true;
	}
	// �����I�΂�Ă���Ȃ�ړ��t���O�𗧂Ă�
	if (m_pPlaneArea->GetHitPlaneIndex() > -1)
	{
		m_pMiniCharacterBase->SetMoving(true);
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
	using namespace DirectX::SimpleMath;
	//auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �X�e�[�W�Z���N�g�̕`��
	m_pStageSelect->Render(m_view, m_projection);
	// �X�e�[�W�̓�����̕`��
	for (auto& gate : m_pStageGates)gate->Render(m_view, m_projection);
	// �~�j�L�����̕`��
	m_pMiniCharacterBase->Render(m_view, m_projection);
	// ���ʂ̕`��
	m_pPlaneArea->Render();
	// --- �f�o�b�O���---
	const auto debugString = m_pCommonResources->GetDebugString();
	// ���C�Ɠ�����������\��
	debugString->AddString("Ray Hit:%s", m_pPlaneArea->IsHitPlane() ? "true" : "false");
	// ���C�Ɠ�����������\��
	debugString->AddString("HitRayIndex:%i", m_pPlaneArea->GetHitPlaneIndex());
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
	// �f�o�b�O�J�������쐬����
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// �Œ�J�������쐬����
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);
	// �g���b�L���O�J�������쐬����
	m_pTrackingCamera = std::make_unique<TrackingCamera>();
	m_pTrackingCamera->Initialize((int)(rect.right), rect.bottom);
	// �ˉe�s����쐬����
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief ���ʂ̒��S���W����ɂS�̒��_��ݒ肷��
*	@details ���ʂ̒��S���W����ɁA���Ɖ��s�����w�肵�ĂS�̒��_��ݒ肷��
*	@param center ���ʂ̒��S���W
*	@param width ���ʂ̕�
*	@param depth ���ʂ̉��s��
*	@param y ���ʂ̍����i�f�t�H���g��0.5f�j
*	@return �S�̒��_�̍��W���i�[�����x�N�g��
*/
std::vector<DirectX::SimpleMath::Vector3> StageSelectScene::CreatePlaneVertices(const DirectX::SimpleMath::Vector3& center, float width, float depth, float y) const
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// ���ʂ̒��_���v�Z����
	float halfW = width / 2.0f;
	// ���ʂ̉��s���̔������v�Z����
	float halfD = depth / 2.0f;
	return {
		Vector3(center.x - halfW, y, center.z - halfD), // ����
		Vector3(center.x + halfW, y, center.z - halfD), // �E��
		Vector3(center.x + halfW, y, center.z + halfD), // �E��
		Vector3(center.x - halfW, y, center.z + halfD)  // ����
	};
}
