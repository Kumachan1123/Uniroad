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
	// ���S���쐬����
	m_pTitleLogo = std::make_unique<TitleLogo>();
	// ���S������������
	m_pTitleLogo->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
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
	// �r���[�s����擾
	m_view = m_pFixedCamera->GetViewMatrix();
	// ���S���X�V
	m_pTitleLogo->Update(elapsedTime);
}
/*
*	@brief �`�揈��
*	@details �^�C�g���V�[���̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleScene::Render()
{
	// ���S��`�悷��
	m_pTitleLogo->Render();
}
/*
*	@brief �V�[��ID���擾����
*	@details �^�C�g���V�[���̎��̃V�[��ID���擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
void TitleScene::Finalize()
{
}

/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		// �Q�[���I�[�o�[�V�[����
		return IScene::SceneID::STAGESELECT;
	}
	// �V�[���ύX���Ȃ��ꍇ�������Ȃ�
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
	// �ˉe�s����쐬����
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
