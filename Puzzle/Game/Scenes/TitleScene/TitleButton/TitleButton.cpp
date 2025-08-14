/*
*	@file TitleButton.cpp
*	@brief �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̎����t�@�C��
*/
#include "pch.h"
#include "TitleButton.h"
// �ŏI�I�ȍ��W�̒�`
const std::vector<DirectX::SimpleMath::Vector2> TitleButton::POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.88f, 0.657f), // �Q�[���J�n�{�^���̈ʒu
	DirectX::SimpleMath::Vector2(0.88f, 0.8f), // �ݒ胁�j���[�{�^���̈ʒu
	DirectX::SimpleMath::Vector2(0.88f, 0.925f)  // �Q�[���I���{�^���̈ʒu
};
// �ŏI�I�ȃT�C�Y�̒�`
const std::vector<DirectX::SimpleMath::Vector2> TitleButton::SIZES =
{
	DirectX::SimpleMath::Vector2(0.2f, 0.09f), // �Q�[���J�n�{�^���̃T�C�Y
	DirectX::SimpleMath::Vector2(0.2f, 0.07f), // �ݒ胁�j���[�{�^���̃T�C�Y
	DirectX::SimpleMath::Vector2(0.2f, 0.07f)  // �Q�[���I���{�^���̃T�C�Y
};
// ����������Ă��Ȃ���Ԃ̃C���f�b�N�X
const int TitleButton::NONE_BUTTON_INDEX = -1;



/*
*	@brief �R���X�g���N�^
*	@details �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleButton::TitleButton()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pStartButton(std::make_unique<Button>()) // �Q�[���J�n�{�^���ւ̃|�C���^
	, m_pSettingButton(std::make_unique<Button>()) // �ݒ胁�j���[�{�^���ւ̃|�C���^
	, m_pExitButton(std::make_unique<Button>()) // �Q�[���I���{�^���ւ̃|�C���^
	, m_pAnimation(std::make_unique<Animation>()) // �A�j���[�V�����ւ̃|�C���^
	, m_isPressed(false) // �{�^���������ꂽ���ǂ���
	, m_isHit(false) // �����蔻��t���O
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // �{�^���̈ʒu
	, m_size(DirectX::SimpleMath::Vector2(0.2f, 0.1f)) // �{�^���̃T�C�Y
	, m_frameRows(1) // �摜�̍s��
	, m_frameCols(1) // �摜�̗�
	, m_pressedButtonIndex(-1) // �����ꂽ�{�^���̔ԍ�
	, m_hitButtonIndex(-1) // ���������{�^���̔ԍ�
{

}
/*
*	@brief �f�X�g���N�^
*	@details �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleButton::~TitleButton()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief ������
*	@details �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void TitleButton::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �摜��ݒ�
	m_pStartButton->SetTexture(resources->GetTextureManager()->GetTexture("StartButton"));
	// �Q�[���J�n�{�^����ǉ�
	m_buttons.push_back(std::move(m_pStartButton));
	// �摜��ݒ�
	m_pSettingButton->SetTexture(resources->GetTextureManager()->GetTexture("SettingButton"));
	// �ݒ胁�j���[�{�^����ǉ�
	m_buttons.push_back(std::move(m_pSettingButton));
	// �摜��ݒ�
	m_pExitButton->SetTexture(resources->GetTextureManager()->GetTexture("GameEndButton"));
	// �Q�[���I���{�^����ǉ�
	m_buttons.push_back(std::move(m_pExitButton));
	// �{�^���̐����[�v
	for (auto& button : m_buttons)
	{
		// �V�F�[�_�[�p�X��n��
		button->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
		button->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
		// �V�F�[�_�[�o�b�t�@�T�C�Y��ݒ�
		button->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
		// �{�^���̏�����
		button->Initialize(resources, width, height);
	}
	// �{�^���̋�`��ݒ�
	for (size_t i = 0; i < m_buttons.size(); i++)
	{
		// �{�^���̈ʒu�ƃT�C�Y��ݒ�
		Rect buttonRect;
		buttonRect.position = POSITIONS[i];
		buttonRect.size = SIZES[i];
		// �{�^���̈ʒu�ƃT�C�Y��z��ɓo�^
		m_buttonRects.push_back(buttonRect);
		// �����蔻��t���O��������
		m_isHit.push_back(false);
		// �z�o�[���̊g�嗦��������
		m_hoverScales.push_back(1.0f);
	}
	// �A�j���[�V�����V�[�P���X���쐬
	CreateAnimationSequence();
}
/*
*	@brief �X�V
*	@details �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void TitleButton::Update(float elapsedTime)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �z�o�[���̊g�嗦���`
	const float SCALE_ON = 1.125f;
	// �z�o�[���Ă��Ȃ��Ƃ��̊g�嗦���`
	const float SCALE_OFF = 1.0f;
	// ��ԌW��
	const float SCALE_SPEED = 8.0f;
	// ���������{�^���̔ԍ���������
	m_hitButtonIndex = -1;
	// �{�^���̐����[�v
	for (int i = 0; i < m_buttons.size(); i++)
	{
		// �����蔻����s��
		m_isHit[i] = m_buttons[i]->Hit(mousePos, m_buttonRects[i]);
		// �X�P�[���̃^�[�Q�b�g�l
		float target = m_isHit[i] ? SCALE_ON : SCALE_OFF;
		// �X���[�Y�ɕ��
		m_hoverScales[i] += (target - m_hoverScales[i]) * (1.0f - expf(-SCALE_SPEED * elapsedTime));
	}
	// �}�E�X�����������{�^���̔ԍ���ݒ�
	for (int i = 0; i < m_buttons.size(); i++)
		if (m_isHit[i])m_hitButtonIndex = i;
	// �}�E�X���������č��N���b�N���ꂽ��
	if (MouseClick::IsLeftMouseButtonPressed(mouseState) && m_pAnimation->IsPaused() && m_hitButtonIndex > NONE_BUTTON_INDEX)
	{
		// �N���b�N�ōĊJ
		m_pAnimation->Resume();
		// �A�j���[�V�����V�[�P���X��i�߂�
		m_pAnimation->AdvanceSequence();
		// �����ꂽ�{�^���̔ԍ���ݒ�
		m_pressedButtonIndex = m_hitButtonIndex;
		// �{�^���������ꂽ�t���O�𗧂Ă�
		m_isPressed = true;
	}
	// �A�j���[�V�����t�F�[�Y��3�i�ړ����j�ŁA�A�j���[�V�������ꎞ��~���Ă��Ȃ��ꍇ�͈ꎞ��~����
	if (m_pAnimation->GetAnimationPhase() == 2 && !m_pAnimation->IsPaused()) m_pAnimation->Pause();
	// �A�j���[�V�������X�V
	m_pAnimation->Update(elapsedTime);
	// �{�^�����X�V
	for (const auto& button : m_buttons)button->Update(elapsedTime);
	// �萔�o�b�t�@���X�V
	UpdateConstantBuffer();
}
/*
*	@brief �`��
*	@details �^�C�g���V�[���̃{�^�����Ǘ�����N���X�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleButton::Render()
{
	// �{�^����`��
	for (size_t i = 0; i < m_buttons.size(); i++)
	{
		// ��`�̒�`
		Rect rect = m_buttonRects[i];
		// �X�P�[�����f
		rect.size = SIZES[i] * m_hoverScales[i];
		// �`��
		m_buttons[i]->DrawQuadWithBuffer(rect, m_spriteSheetBuffer);
	}
#ifdef _DEBUG
	// �f�o�b�O������`��
	const auto& debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("AnimationPhase:%i", m_pAnimation->GetAnimationPhase());
	debugString->AddString("AllAnimationSequenceCount:%i", m_pAnimation->GetAnimationSequenceCount());
	debugString->AddString("AnimationPaused:%s", m_pAnimation->IsPaused() ? "true" : "false");
	debugString->AddString("PressedButtonIndex:%i", m_pressedButtonIndex);
	debugString->AddString("HitButtonIndex:%i", m_hitButtonIndex);
#endif
}
/*
*	@brief �A�j���[�V�����V�[�P���X���쐬
*	@details �^�C�g���V�[���̃{�^���̃A�j���[�V�����V�[�P���X���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleButton::CreateAnimationSequence()
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �e�{�^���̒x��
	const std::vector<float> DELAYS = { 0.0f, 0.15f, 0.3f };
	// �t�F�[�Y1: �ҋ@
	m_pAnimation->CreateAnimationSequence({
			2.5f,// �ҋ@����
			[this](float) {
			// 0�œ��������ꏊ�ƃT�C�Y�ŌŒ�
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				m_buttonRects[i].position = Vector2(2.5f + i * 0.5f, POSITIONS[i].y);
			}
	} });
	// �t�F�[�Y2: �ړ� 
	const float DURATION = 1.0f; // �e�{�^���̈ړ��ɂ����鎞��
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + DURATION, // �S�̂̉��o����
		[this, DELAYS, DURATION](float globalT) {
			// globalT��0�`1�ŃV�[�P���X�S�̂ɑΉ�����i�s�x
			float totalTime = globalT * (DELAYS.back() + DURATION); // ���ۂ̌o�ߕb��
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// �e�{�^���̐i�s�x
				float t = (totalTime - DELAYS[i]) / DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// ���
				m_buttonRects[i].position = Vector2::Lerp(Vector2(2.5f + i * 0.1f, POSITIONS[i].y), POSITIONS[i], easing);
			}
		}
		});
	// �t�F�[�Y3: �Œ� 
	m_pAnimation->CreateAnimationSequence({
			0.0f, // ����
			[this](float) {
			// �ŏI�Î~�ʒu�ƃT�C�Y�ɐݒ�
			// �{�^���̐����[�v
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// �ŏI�I�Ȉʒu�ɐݒ�
				m_buttonRects[i].position = POSITIONS[i];
				// �ŏI�I�ȃT�C�Y�ɐݒ�
				m_buttonRects[i].size = SIZES[i];
			}
	} });
	// �t�F�[�Y4: �ړ��i���̏ꏊ�ցj
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + DURATION, // �S�̂̉��o����
		[this, DELAYS, DURATION](float globalT) {
			// globalT��0�`1�ŃV�[�P���X�S�̂ɑΉ�����i�s�x
			float totalTime = globalT * (DELAYS.back() + DURATION);
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// �e�{�^���̐i�s�x
				float t = (totalTime - DELAYS[i]) / DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// ���
				m_buttonRects[i].position = Vector2::Lerp(POSITIONS[i], Vector2(1.5f + i  , POSITIONS[i].y), easing);
			}
		}
		});
	// �t�F�[�Y5: �ҋ@
	m_pAnimation->CreateAnimationSequence({
			0.0f, // ����
			[this](float) {
			// 0�œ��������ꏊ�ƃT�C�Y�ŌŒ�
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				m_buttonRects[i].position = Vector2(1.5f + i, POSITIONS[i].y);
				m_buttonRects[i].size = SIZES[i];
			}
	} });
}
/*
*	@brief �萔�o�b�t�@���X�V
*	@details �^�C�g���V�[���̃{�^���̒萔�o�b�t�@���X�V����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleButton::UpdateConstantBuffer()
{
	// ���O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �萔�o�b�t�@���X�V
	// ���[���h�s���P�ʍs��ɐݒ�
	m_spriteSheetBuffer.matWorld = Matrix::Identity;
	// �r���[�s���P�ʍs��ɐݒ�
	m_spriteSheetBuffer.matView = Matrix::Identity;
	// �v���W�F�N�V�����s���P�ʍs��ɐݒ�
	m_spriteSheetBuffer.matProj = Matrix::Identity;
	// �A�j���[�V�����̃R�}��ݒ�
	m_spriteSheetBuffer.count = Vector4(0.0f);
	// ������ݒ�
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// ����ݒ�
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}
