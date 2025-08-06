/*
*	@file ResultUI.cpp
*	@brief ����UI�N���X
*/
#include "pch.h"
#include "ResultUI.h"
// �����ȃ��j���[�C���f�b�N�X
const int ResultUI::INVALID_MENU_INDEX = -1;// �����ȃ��j���[�C���f�b�N�X
/*
*	@brief �R���X�g���N�^
*	@details ����UI�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
ResultUI::ResultUI()
	: m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X
	, m_windowWidth(0)// �E�B���h�E�̕�
	, m_windowHeight(0) // �E�B���h�E�̍���
	, m_stageNum(0) // �X�e�[�W�ԍ�
	, m_menuIndex(INVALID_MENU_INDEX) // ���j���[�̃C���f�b�N�X
	, m_num(SceneID::NONE) // �V�[��ID
	, m_time(0.0f) // ����
	, m_hit(false) // �q�b�g�t���O
	, m_enable(false) // ���̃N���X���L�����ǂ���
{
	// �Ȃ�
}
/*
*	@brief �f�X�g���N�^
*	@details ����UI�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
ResultUI::~ResultUI()
{
	// �Ȃ�
}
/*
*	@brief ������
*	@details ����UI�̏��������s��
*	@param resources ���ʃ��\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void ResultUI::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �E�B���h�E��
	m_windowWidth = width;
	// �E�B���h�E����
	m_windowHeight = height;

}
/*
*	@brief �X�V
*	@details ����UI�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ResultUI::Update(const float elapsedTime)
{
	// �L���t���O�������Ă��Ȃ��ꍇ�͉������Ȃ�
	if (!m_enable)return;
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// ���Ԃ̍X�V
	m_time += elapsedTime;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �����Ƀq�b�g�������ǂ�����������
	m_hit = false;
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �{�^���̐��������[�v
	for (int i = 0; i < m_pButtons.size(); i++)
	{	// �E�B���h�E�n���h�����擾
		const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
		// �E�B���h�E�T�C�Y�擾
		RECT rect;
		// �N���C�A���g�̈�T�C�Y���擾
		GetClientRect(hwnd, &rect);
		// �E�B���h�E�̕��i�s�N�Z���P�ʁj
		m_windowWidth = static_cast<int>(rect.right);
		// �E�B���h�E�̍����i�s�N�Z���P�ʁj
		m_windowHeight = static_cast<int>(rect.bottom);
		// �E�B���h�E�T�C�Y��ݒ�
		m_pButtons[i]->SetWindowSize(m_windowWidth, m_windowHeight);
		// �E�B���h�E�T�C�Y�ɉ����ă{�^���̈ʒu�ƃX�P�[���𒲐�
		if (m_windowWidth == Screen::WIDTH)
		{
			// �{�^���̍��W���X�V
			if (m_pButtons.size() == 1)
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) + 100));
			else
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2) - (200) + (i * 200 * 2), float(m_windowHeight / 2) + 100));
			// �{�^���̃X�P�[�����X�V
			m_pButtons[i]->SetScale(Vector2(0.5f, 0.5f));
		}
		else
		{
			// �{�^���̍��W���X�V
			if (m_pButtons.size() == 1)
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) + 100));
			else
				m_pButtons[i]->SetPosition(Vector2(float(m_windowWidth / 2) - (400) + (i * 400 * 2), float(m_windowHeight / 2 + 200)));
			// �{�^���̃X�P�[�����X�V
			m_pButtons[i]->SetScale(Vector2(1.0f, 1.0f));
		}

		// ���Ԃ����Z
		m_pButtons[i]->SetTime(m_pButtons[i]->GetTime() + elapsedTime);
		// �{�^�����q�b�g�������ǂ������`�F�b�N
		if (m_pButtons[i]->IsHit(mousePos))
		{
			// �q�b�g�t���O�𗧂Ă�
			m_hit = true;
			// �O��I�������{�^���ƈႤ�ꍇ��SE�Đ��t���O�𗧂Ă�i��������j
			//if ((int(m_menuIndex)) != i) m_isSEPlay = false;
			//// SE���Đ�����Ă��Ȃ��ꍇ
			//if (!m_isSEPlay)
			//{
			//	// �I�����̍Đ�
			//	m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
			//	// �Đ��t���O�𗧂Ă�
			//	m_isSEPlay = true;
			//}
			// �{�^���̃X�P�[�����X�V
			m_pButtons[i]->SetScale(m_pButtons[i]->GetScale() * 1.2f);
			// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			m_menuIndex = i;
			// �q�b�g�����烋�[�v�𔲂���
			break;
		}
	}
	// �q�b�g�����Ȃ�I���C���f�b�N�X�𖳌��l�ɐݒ�
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;
	// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	if (MouseClick::IsLeftMouseButtonPressed(mouseState))
		m_num = static_cast<SceneID>(m_menuIndex);
}
/*
*	@brief �`��
*	@details ����UI�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ResultUI::Render()
{
	// �L���t���O�������Ă��Ȃ��ꍇ�͉������Ȃ�
	if (!m_enable)return;
	// �{�^������ĕ`��
	for (unsigned int i = 0; i < m_pButtons.size(); i++)m_pButtons[i]->Render();
}
/*
*	@brief �{�^����ǉ�
*	@details �w�肳�ꂽ�L�[�ƈʒu�A�X�P�[���A�A���J�[��UI��ǉ�����
*	@param key �摜�̃L�[
*	@param position �ʒu
*	@param scale �X�P�[��
*	@param anchor �A���J�[
*	@param type UI�̎��
*	@return �Ȃ�
*/
void ResultUI::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor,
	UIType type)
{
	// UI�I�u�W�F�N�g�̐���
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// �w��摜��UI�쐬
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UI�̎�ނɉ����ĕۑ�
	if (type == UIType::BUTTON)m_pButtons.push_back(std::move(userInterface));

}
/*
*	@brief �V�[�����猋�ʂ��󂯎��
*	@details �Q�[���I�[�o�[��Q�[���N���A�̌��ʂ��󂯎��AUI�̕\����؂�ւ���
*	@param gameOver �Q�[���I�[�o�[���ǂ���
*	@param gameClear �Q�[���N���A���ǂ���
*	@return �Ȃ�
*/
void ResultUI::SetResult(bool gameOver, bool gameClear)
{
	UNREFERENCED_PARAMETER(gameOver); // �Q�[���I�[�o�[�͎g�p���Ȃ�
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// �Q�[���N���A�Ȃ玟�̃X�e�[�W�ɔ�ׂ�悤�ɂ���
	if (gameClear)m_stageNum++;
	// ���̃X�e�[�W�����݂��Ă��ăQ�[���N���A�Ȃ�
	if (m_stageNum < FileCounter::CountFilesInFolder("Resources/Map/", ".csv") && gameClear)
	{
		// �{�^����ǉ�
		Add("ToNextStage"
			, Vector2(Screen::CENTER_X_BUTTON - 400, Screen::CENTER_Y_BUTTON + 200)
			, Vector2(1.0f)
			, KumachiLib::ANCHOR::MIDDLE_CENTER
			, UIType::BUTTON);
	}
	// �Q�[���I�[�o�[�Ȃ�
	else if (gameOver)
	{
		// �{�^����ǉ�
		Add("ReTry"
			, Vector2(Screen::CENTER_X_BUTTON - 400, Screen::CENTER_Y_BUTTON + 200)
			, Vector2(1.0f)
			, KumachiLib::ANCHOR::MIDDLE_CENTER
			, UIType::BUTTON);
	}
	// �{�^����ǉ�
	Add("ToStageSelect"
		, Vector2(Screen::CENTER_X_BUTTON + 400, Screen::CENTER_Y_BUTTON + 200)
		, Vector2(1.0f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::BUTTON);

}