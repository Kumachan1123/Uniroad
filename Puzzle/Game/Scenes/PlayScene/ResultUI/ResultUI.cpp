/*
*	@file ResultUI.cpp
*	@brief ����UI�N���X
*/
#include "pch.h"
#include "ResultUI.h"
// �����ȃ��j���[�C���f�b�N�X
const int ResultUI::INVALID_MENU_INDEX = -1;
// �ʏ�̕\���ʒu�␳�l(�S��ʊ�j
const float ResultUI::DISPLAY_OFFSET = -400.0f;
// �{�^���̈ړ��ɂ����鎞��
const float ResultUI::MOVE_DURATION = 0.3f;

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
	, m_prevWindowWidth(0) // �O�̃E�B���h�E�̕�
	, m_prevWindowHeight(0) // �O�̃E�B���h�E�̍���
	, m_stageNum(0) // �X�e�[�W�ԍ�
	, m_buttonCount(0) // ���������{�^���̐�
	, m_menuIndex(INVALID_MENU_INDEX) // ���j���[�̃C���f�b�N�X
	, m_num(SceneID::NONE) // �V�[��ID
	, m_time(0.0f) // ����
	, m_hit(false) // �q�b�g�t���O
	, m_enable(false) // ���̃N���X���L�����ǂ���
	, m_canPress(false) // �{�^���������邩�ǂ���
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
	// �E�B���h�E�T�C�Y��ݒ�
	m_windowHeight = height;
	m_windowWidth = width;
	// �i�s�x��������
	m_easeTimers.clear();
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
	// �E�B���h�E�n���h�����擾
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�T�C�Y��ݒ�
	GetScreenRect(hwnd, m_windowWidth, m_windowHeight);
	// ����ƃ{�^�����ω����̓��T�C�Y�������ɍ��W�𓯊�
	size_t btnCount = m_pButtons.size();
	// �{�^���̐����ς�����ꍇ�@
	if (m_prevPositions.size() != btnCount)
	{
		// �O�̍��W�ƖڕW���W�A�C�[�W���O�^�C�}�[�����T�C�Y
		m_prevPositions.resize(btnCount);
		m_targetPositions.resize(btnCount);
		m_easeTimers.resize(btnCount, 1.0f);
		// �{�^���̐��������[�v
		for (size_t i = 0; i < btnCount; ++i)
		{
			// �{�^���̍��W��������
			m_prevPositions[i] = m_pButtons[i]->GetPosition();
			// �{�^���̖ڕW���W��������
			m_targetPositions[i] = m_pButtons[i]->GetPosition();
			// �C�[�W���O�^�C�}�[��������
			m_easeTimers[i] = 1.0f;
		}
	}
	// �E�B���h�E�T�C�Y���ς�������ǂ������`�F�b�N
	bool resized = (m_windowWidth != m_prevWindowWidth) || (m_windowHeight != m_prevWindowHeight);
	// �E�B���h�E�T�C�Y���ς�����ꍇ�̓��T�C�Y�������s��
	if (resized)
	{
		// ���T�C�Y���̏������Ăяo��
		OnResize();
		// �O�̃E�B���h�E�T�C�Y���X�V
		m_prevWindowWidth = m_windowWidth;
		m_prevWindowHeight = m_windowHeight;
		// ���T�C�Y���̓C�[�W���O����return
		return;
	}

	// �{�^���̐��������[�v
	for (size_t i = 0; i < btnCount; ++i)
	{
		// �E�B���h�E�T�C�Y��ݒ�
		m_pButtons[i]->SetWindowSize(m_windowWidth, m_windowHeight);
		// �ڕW���W����
		Vector2 newTarget;
		// �E�B���h�E�T�C�Y�ɉ����ă{�^���̈ʒu�ƃX�P�[���𒲐�
		// �E�B���h�E���[�h
		if (m_windowWidth == Screen::WIDTH)
		{
			// �{�^���̍��W���X�V
			if (m_pButtons.size() == 1)
				newTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				newTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET / 2) + (i * DISPLAY_OFFSET), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			// �{�^���̃X�P�[�����X�V
			m_pButtons[i]->SetScale(Vector2::One / 2.0f);
		}
		// �S���
		else
		{
			// �{�^���̍��W���X�V
			if (m_pButtons.size() == 1)
				newTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				newTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET)+(i * DISPLAY_OFFSET * 2), float(m_windowHeight / 2 - DISPLAY_OFFSET / 2.0f));
			// �{�^���̃X�P�[�����X�V
			m_pButtons[i]->SetScale(Vector2::One);
		}
		// �ڕW���W���ς������C�[�W���O�����Z�b�g
		if ((m_targetPositions[i] - newTarget).LengthSquared() > 0.001f)
		{
			// �O�̍��W���X�V
			m_prevPositions[i] = m_pButtons[i]->GetPosition();
			// �ڕW���W���X�V
			m_targetPositions[i] = newTarget;
			// �C�[�W���O�^�C�}�[�����Z�b�g
			m_easeTimers[i] = 0.0f;
			// �{�^���������Ȃ�����
			m_canPress = false;
		}
		// �C�[�W���O��K�p
		if (m_easeTimers[i] < 1.0f)
		{
			// �o�ߎ��Ԃ��C�[�W���O�^�C�}�[�ɉ��Z
			m_easeTimers[i] += elapsedTime / MOVE_DURATION;
			// �C�[�W���O�^�C�}�[��1.0f�𒴂�����
			if (m_easeTimers[i] > 1.0f)
			{
				// �C�[�W���O�^�C�}�[��1.0f�𒴂��Ȃ��悤�ɐ���
				m_easeTimers[i] = 1.0f;
				// �{�^����������悤�ɂ���
				m_canPress = true;
			}

		}
		// �C�[�W���O��K�p���ă{�^���̈ʒu���X�V
		float t = Easing::EaseInCubic(m_easeTimers[i]);
		// �O�̍��W�ƖڕW���W���Ԃ��ă{�^���̈ʒu���X�V
		Vector2 pos = Vector2::Lerp(m_prevPositions[i], m_targetPositions[i], t);
		// �{�^���̈ʒu��ݒ�
		m_pButtons[i]->SetPosition(pos);
		// ���Ԃ����Z
		m_pButtons[i]->SetTime(m_pButtons[i]->GetTime() + elapsedTime);
		// �{�^�����q�b�g�������ǂ������`�F�b�N
		if (m_pButtons[i]->IsHit(mousePos) && m_canPress)
		{
			// �q�b�g�t���O�𗧂Ă�
			m_hit = true;
			// �{�^���̃X�P�[�����X�V
			m_pButtons[i]->SetScale(m_pButtons[i]->GetScale() * 1.2f);
			// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			m_menuIndex = static_cast<int>(i);
			// �q�b�g�����烋�[�v�𔲂���
			break;
		}
	}
	// �q�b�g�����Ȃ�I���C���f�b�N�X�𖳌��l�ɐݒ�
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;
	// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_canPress)
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
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("ResultUI:Hit = %s", m_hit ? "true" : "false");
	debugString->AddString("ResultUI:MenuIndex = %d", m_menuIndex);
	debugString->AddString("ResultUI:CanPress = %s", m_canPress ? "true" : "false");
	for (unsigned int i = 0; i < m_easeTimers.size(); i++)
		debugString->AddString("ResultUI:EaseTimer[%d] = %f", i, m_easeTimers[i]);
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
	using namespace DirectX::SimpleMath;
	// UI�I�u�W�F�N�g�̐���
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// �w��摜��UI�쐬
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UI�̎�ނɉ����ĕۑ�
	if (type == UIType::BUTTON)m_pButtons.push_back(std::move(userInterface));
	// --- �C�[�W���O�p�f�[�^�������� ---
	Vector2 pos = m_pButtons.back()->GetPosition();
	m_prevPositions.push_back(pos);   // �J�n�ʒu
	m_targetPositions.push_back(pos); // �ڕW�ʒu
	m_easeTimers.push_back(0.0f);     // �ŏ��͓��B�ς�
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
	using namespace KumachiLib;
	// �E�B���h�E�n���h�����擾
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�T�C�Y��ݒ�
	GetScreenRect(hwnd, m_windowWidth, m_windowHeight);
	// �O�̃E�B���h�E�T�C�Y�ɂ������l��ݒ�
	m_prevWindowHeight = m_windowHeight;
	m_prevWindowWidth = m_windowWidth;
	// �Q�[���N���A�����̃X�e�[�W�ԍ����X�e�[�W���ȓ��Ȃ玟�̃X�e�[�W�ɔ�ׂ�悤�ɂ���
	if (gameClear)	m_stageNum++;
	// ���^���Ă���X�e�[�W�̐����擾
	int fileCount = FileCounter::CountFilesInFolder("Resources/Map/", ".csv");
	// �������W���`(�E�B���h�E���[�h�)
	Vector2 initialPosition = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
	// �����T�C�Y���`(�E�B���h�E���[�h�)
	Vector2 initialScale = Vector2::One / 2.0f;
	// �S��ʃ��[�h�Ȃ�
	if (m_windowWidth == Screen::WIDTH_BUTTON)
	{
		// �������W���Ē�`
		initialPosition = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 2.0f);
		// �����T�C�Y���Ē�`
		initialScale = Vector2::One;
	}
	// �u�X�e�[�W�Z���N�g�ցv�{�^����ǉ�
	Add("ToStageSelect", initialPosition, initialScale, ANCHOR::MIDDLE_CENTER, UIType::BUTTON);
	// �ݒu�����{�^�����J�E���g
	m_buttonCount++;
	// ���̃X�e�[�W�����݂��Ă��ăQ�[���N���A�Ȃ�
	if (m_stageNum < fileCount && gameClear)
	{
		// �u���̃X�e�[�W�ցv�{�^����ǉ�
		Add("ToNextStage", initialPosition, initialScale, ANCHOR::MIDDLE_CENTER, UIType::BUTTON);
		// �ݒu�����{�^�����J�E���g
		m_buttonCount++;
	}
	// �Q�[���I�[�o�[�Ȃ�
	else if (gameOver)
	{
		// �u���g���C�v�{�^����ǉ�
		Add("ReTry", initialPosition, initialScale, ANCHOR::MIDDLE_CENTER, UIType::BUTTON);
		// �ݒu�����{�^�����J�E���g
		m_buttonCount++;
	}
	// �ŏI�X�e�[�W�Ȃ���Z�����X�e�[�W�ԍ���߂�
	if (m_stageNum >= fileCount)
		m_stageNum--;
}

/*
*	@brief ��ʃ��T�C�Y���̏���
*	@details ��ʂ̃��T�C�Y�ɉ�����UI�̈ʒu��T�C�Y�𒲐�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ResultUI::OnResize()
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	using namespace DirectX::SimpleMath;
	size_t btnCount = m_pButtons.size();

	for (size_t i = 0; i < btnCount; ++i)
	{
		// �E�B���h�E�T�C�Y�ɉ����ĖڕW���W���Čv�Z
		Vector2 snapTarget;
		// �E�B���h�E�T�C�Y���S��ʃ��[�h�Ȃ�
		if (m_windowWidth == Screen::WIDTH)
		{
			if (btnCount == 1)
				snapTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				snapTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET / 2) + (i * DISPLAY_OFFSET), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			m_pButtons[i]->SetScale(Vector2::One / 2.0f);
		}
		// �E�B���h�E�T�C�Y���S��ʃ��[�h�łȂ��Ȃ�
		else
		{
			if (btnCount == 1)
				snapTarget = Vector2(float(m_windowWidth / 2), float(m_windowHeight / 2) - DISPLAY_OFFSET / 4.0f);
			else
				snapTarget = Vector2(float(m_windowWidth / 2) - (DISPLAY_OFFSET)+(i * DISPLAY_OFFSET * 2), float(m_windowHeight / 2 - DISPLAY_OFFSET / 2.0f));
			m_pButtons[i]->SetScale(Vector2::One);
		}
		// �O�̍��W�ƖڕW���W�A�C�[�W���O�^�C�}�[���X�V
		m_prevPositions[i] = snapTarget;
		m_targetPositions[i] = snapTarget;
		m_easeTimers[i] = 1.0f;
		m_pButtons[i]->SetPosition(snapTarget);
	}
}