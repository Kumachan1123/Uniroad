/*
*	@file BackButton.cpp
*	@brief �߂�{�^���̏������`����N���X
*/
#include "pch.h"
#include "BackButton.h"
// �ŏI�I�ȍ��W�̒�`
const std::vector<DirectX::SimpleMath::Vector2> BackButton::POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.0f, 0.99f), // �Q�[���J�n�{�^���̈ʒu
};
// �ŏI�I�ȃT�C�Y�̒�`
const std::vector<DirectX::SimpleMath::Vector2> BackButton::SIZES =
{
	DirectX::SimpleMath::Vector2(0.4f, 0.18f), // �Q�[���J�n�{�^���̃T�C�Y
};
// ����������Ă��Ȃ���Ԃ̃C���f�b�N�X
const int BackButton::NONE_BUTTON_INDEX = -1;

/*
*	@brief �R���X�g���N�^
*	@details �߂�{�^���̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
BackButton::BackButton()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pBackButton(std::make_unique<Button>()) // �^�C�g���ɖ߂�{�^���ւ̃|�C���^
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
*	@details �߂�{�^���̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
BackButton::~BackButton()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �߂�{�^���̃|�C���^�����Z�b�g
	m_pBackButton.reset();
	// �A�j���[�V�����̃|�C���^�����Z�b�g
	m_pAnimation.reset();
	// �{�^���z����N���A
	m_buttons.clear();
	// �{�^���̋�`���N���A
	m_buttonRects.clear();
	// �����蔻��t���O���N���A
	m_isHit.clear();
}
/*
*	@brief ������
*	@details �߂�{�^���̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void BackButton::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �摜��ݒ�
	m_pBackButton->SetTexture(resources->GetTextureManager()->GetTexture("ToTitle"));
	// �Q�[���I���{�^����ǉ�
	m_buttons.push_back(std::move(m_pBackButton));
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
*	@brief �X�V����
*	@details �߂�{�^���̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BackButton::Update(float elapsedTime)
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
	for (int i = 0; i < m_buttons.size(); i++)	if (m_isHit[i])m_hitButtonIndex = i;
	// �}�E�X���������č��N���b�N���ꂽ��
	if (m_pAnimation->IsPaused() && m_hitButtonIndex > NONE_BUTTON_INDEX)
	{
		// ���N���b�N���ꂽ��
		if (MouseClick::IsLeftMouseButtonPressed(mouseState))
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
*	@brief �`�悷��
*	@details �߂�{�^���̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BackButton::Render()
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
*	@details �߂�{�^���̃A�j���[�V�����V�[�P���X���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BackButton::CreateAnimationSequence()
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �e�{�^���̒x��
	const std::vector<float> DELAYS = { 0.0f, 0.15f, 0.3f };
	// �t�F�[�Y1: �ҋ@
	m_pAnimation->CreateAnimationSequence({
			0.001f,// �ҋ@����
			[this](float) {
			// 0�œ��������ꏊ�ƃT�C�Y�ŌŒ�
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				m_buttonRects[i].position = Vector2(-1.5f + i * 0.5f, POSITIONS[i].y + 0.0f);
			}
	} });
	// �t�F�[�Y2: �ړ� 
	// �e�{�^���̈ړ��ɂ����鎞��
	const float START_DURATION = 0.25f;
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + START_DURATION, // �S�̂̉��o����
		[this, DELAYS, START_DURATION](float globalT) {
			// globalT��0�`1�ŃV�[�P���X�S�̂ɑΉ�����i�s�x
			float totalTime = globalT * (DELAYS.back() + START_DURATION); // ���ۂ̌o�ߕb��
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// �e�{�^���̐i�s�x
				float t = (totalTime - DELAYS[i]) / START_DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// ���
				m_buttonRects[i].position = Vector2::Lerp(Vector2(-1.5f + i * 0.1f, POSITIONS[i].y + 0.0f), POSITIONS[i], easing);
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
	// �e�{�^���̈ړ��ɂ����鎞��
	const float END_DURATION = 1.0f;
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + END_DURATION, // �S�̂̉��o����
		[this, DELAYS, END_DURATION](float globalT) {
			// globalT��0�`1�ŃV�[�P���X�S�̂ɑΉ�����i�s�x
			float totalTime = globalT * (DELAYS.back() + END_DURATION);
			for (size_t i = 0; i < m_buttonRects.size(); i++)
			{
				// �e�{�^���̐i�s�x
				float t = (totalTime - DELAYS[i]) / END_DURATION;
				t = Clamp(t, 0.0f, 1.0f);
				float easing = Easing::EaseInOutCubic(t);
				// ���
				m_buttonRects[i].position = Vector2::Lerp(POSITIONS[i], Vector2(-1.5f + i  , POSITIONS[i].y + 0.0f), easing);
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
				m_buttonRects[i].position = Vector2(-1.5f + i, POSITIONS[i].y + 0.0f);
				m_buttonRects[i].size = SIZES[i];
			}
	} });
}
/*
*	@brief �萔�o�b�t�@���X�V
*	@details �߂�{�^���̒萔�o�b�t�@���X�V����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BackButton::UpdateConstantBuffer()
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
