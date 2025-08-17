/*
*	@file TitleScene.cpp
*	@brief �^�C�g���V�[���N���X
*/
#include "pch.h"
#include "TitleScene.h"
/*
*	@brief �R���X�g���N�^
*	@details �^�C�g���V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
TitleScene::TitleScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pFixedCamera(nullptr) // �J�����ւ̃|�C���^
	, m_pMiniCharacterBase(nullptr) // �~�j�L�����x�[�X�ւ̃|�C���^
	, m_view() // �r���[�s��
	, m_projection() // �ˉe�s��
	, m_isChangeScene(false) // �V�[���ύX�t���O
	, m_nextSceneID(sceneID) // ���̃V�[��ID
{
}
/*
*	@brief �f�X�g���N�^
*	@details �^�C�g���V�[���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleScene::~TitleScene()
{
	// �������Ȃ�
}
/*
*	@brief ������
*	@details �^�C�g���V�[���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void TitleScene::Initialize(CommonResources* resources)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// ����쐬����
	m_pSky = std::make_unique<Sky>(m_pCommonResources);
	// �������������
	m_pSky->Initialize();
	// ��̈ʒu��ݒ�
	m_pSky->SetPosition(Vector3(0.0f, 40.0f, 0.0f));
	// ��̃X�P�[����ݒ�
	m_pSky->SetScale(Vector3(0.2f));
	// ��̉�]���x��ݒ�
	m_pSky->SetRotationSpeed(1.0f);
	// ���H���쐬����
	m_pRoad = std::make_unique<Road>(m_pCommonResources);
	// ���H������������
	m_pRoad->Initialize();
	// ���S���쐬����
	m_pTitleLogo = std::make_unique<TitleLogo>();
	// ���S������������
	m_pTitleLogo->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// �{�^�����쐬����
	m_pTitleButton = std::make_unique<TitleButton>();
	// �{�^��������������
	m_pTitleButton->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// �t�F�[�h���쐬����
	m_pFade = std::make_unique<Fade>();
	// �t�F�[�h������������
	m_pFade->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// �t�F�[�h�C���Ɉڍs
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// �~�j�L�������쐬����
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// �~�j�L����������������
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// �~�j�L�����x�[�X�Ƀ~�j�L�������A�^�b�`
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterTitle>(m_pMiniCharacterBase.get(), Vector3(-10.0f, -0.45f, 0.0f), 0.0f));
	// �~�j�L�����̃A�j���[�V�����X�e�[�g��ݒ�
	m_pMiniCharacterBase->SetTitleAnimationState(NONE);

	// �J�������쐬����
	CreateCamera();
}
/*
*	@brief �X�V����
*	@details �^�C�g���V�[���̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void TitleScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �Œ�J�����̍X�V
	m_pFixedCamera->Update();
	// �f�o�b�O�J�������X�V����
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// �r���[�s����擾
	m_view = m_debugCamera->GetViewMatrix();
	// �J�����̈ʒu�𒲐�
	m_pFixedCamera->SetCameraDistance(Vector3(0.0f, 1.8f, 5.0f));
	Vector3 targetPos = m_pMiniCharacterBase->GetCameraPosition();
	// �J�����̃^�[�Q�b�g�ʒu���~�j�L�����̃J�����ʒu�ɐݒ�
	m_pFixedCamera->SetTargetPosition(Vector3(targetPos.x, targetPos.y + 2.5f, targetPos.z));
	// �J�����̍��W���X�V
	m_pFixedCamera->SetEyePosition(m_pMiniCharacterBase->GetCameraPosition() + m_pFixedCamera->GetCameraDistance());
	// ���S���X�V
	m_pTitleLogo->Update(elapsedTime);
	// �{�^�����X�V
	m_pTitleButton->Update(elapsedTime);
	// �t�F�[�h�̍X�V
	m_pFade->Update(elapsedTime);
	// ��̍X�V
	m_pSky->Update(elapsedTime);
	// ���H�̍X�V
	m_pRoad->Update(elapsedTime);

	// ���W��������
	Vector3 position(Vector3(0, 0, 0));
	// Y����90����]
	Quaternion angle(Quaternion::CreateFromAxisAngle(Vector3::UnitY, XMConvertToRadians(90.0f)));
	// �~�j�L�����̍X�V
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	// �t�F�[�h�C�����I�������
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// �^�C�g���A�j���[�V�������J�n
		m_pMiniCharacterBase->SetTitleAnimationState(START);
		// �t�F�[�h��Ԃ��Ȃ���
		m_pFade->SetState(Fade::FadeState::None);
	}
	// �{�^���������ꂽ�ꍇ
	if (m_pTitleButton->IsPressed())
	{
		// �~�j�L�����̃^�C�g���A�j���[�V�������I����Ԃɂ���
		m_pMiniCharacterBase->SetTitleAnimationState(CONTINUE);
		// �����ꂽ�������Z�b�g
		m_pTitleButton->SetPressed(false);
	}
	// �~�j�L�����̃^�C�g���A�j���[�V�������I����ԂȂ�
	if (m_pMiniCharacterBase->GetTitleAnimationState() == END)
	{
		// �t�F�[�h�A�E�g�Ɉڍs
		m_pFade->SetState(Fade::FadeState::FadeOut);
		// �~�j�L�����̃^�C�g���A�j���[�V���������Z�b�g
		m_pMiniCharacterBase->SetTitleAnimationState(NONE);
	}
	// �t�F�[�h�A�E�g���������Ă�����A�V�[���J�ڃt���O�𗧂Ă�
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
}
/*
*	@brief �`�揈��
*	@details �^�C�g���V�[���̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleScene::Render()
{
	// ���`�悷��
	m_pSky->Render(m_view, m_projection);
	// ���H��`�悷��
	m_pRoad->Render(m_view, m_projection);
	// �~�j�L�����̕`��
	m_pMiniCharacterBase->Render(m_view, m_projection);

	// �ȉ��A2D�`��̂��̂�`�悷��
	// ���S��`�悷��
	m_pTitleLogo->Render();
	// �{�^����`�悷��
	m_pTitleButton->Render();
	// �t�F�[�h��`�悷��
	m_pFade->Render();
}
/*
*	@brief �V�[��ID���擾����
*	@details �^�C�g���V�[���̎��̃V�[��ID���擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
void TitleScene::Finalize()
{
	// �~�j�L�����̏I��
	if (m_pMiniCharacterBase) m_pMiniCharacterBase->Finalize();
}

/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// �V�[���ύX���Ȃ��Ȃ炷���߂�
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	switch (m_pTitleButton->GetPressedButtonIndex())
	{
	case 0: // �Q�[���J�n�{�^���������ꂽ�ꍇ
		// �X�e�[�W�Z���N�g��
		return IScene::SceneID::STAGESELECT;
		break;
	case 1: // �ݒ胁�j���[�{�^���������ꂽ�ꍇ
		// �ݒ胁�j���[��
		return IScene::SceneID::TITLE;
		break;
	case 2: // �Q�[���I���{�^���������ꂽ�ꍇ
		// �A�v���P�[�V�������I������
		PostQuitMessage(0);
		break;
	}
	// �X�e�[�W�Z���N�g��
	return IScene::SceneID::NONE;

}

/*
*	@brief �J�������쐬����
*	@details �^�C�g���V�[���p�̃J�������쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �o�̓T�C�Y���擾����
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// �Œ�J�������쐬����
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// �ˉe�s����쐬����
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(60.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);

}
