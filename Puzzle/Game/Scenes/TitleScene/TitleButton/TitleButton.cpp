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
		// �{�^���̏�����
		button->Initialize(resources, width, height);
	}
	// �{�^���̋�`��ݒ�
	for (size_t i = 0; i < m_buttons.size(); ++i)
	{
		// �{�^���̈ʒu�ƃT�C�Y��ݒ�
		Rect buttonRect;
		buttonRect.position = POSITIONS[i];
		buttonRect.size = SIZES[i];
		// �{�^���̈ʒu�ƃT�C�Y��z��ɓo�^
		m_buttonRects.push_back(buttonRect);
		// �����蔻��t���O��������
		m_isHit.push_back(false);
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
	// �{�^���̐����[�v
	for (size_t i = 0; i < m_buttons.size(); ++i)
	{
		// �����蔻����s��
		m_isHit[i] = m_buttons[i]->Hit(mousePos, m_buttonRects[i]);
		// �}�E�X���������č��N���b�N���ꂽ�牟���ꂽ��Ԃ��g�O��
		if (m_isHit[i] && MouseClick::IsLeftMouseButtonPressed(mouseState))m_isPressed = !m_isPressed;

	}
	// �A�j���[�V�������X�V
	m_pAnimation->Update(elapsedTime);
	// �{�^�����X�V
	for (const auto& button : m_buttons)button->Update(elapsedTime);
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
	for (size_t i = 0; i < m_buttons.size(); ++i)
		m_buttons[i]->Render(m_buttonRects[i], 0, m_frameCols, m_frameRows);

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
			for (size_t i = 0; i < m_buttonRects.size(); ++i)
			{
				m_buttonRects[i].position = Vector2(2.5f + i * 0.5f, POSITIONS[i].y);
			}
	} });
	// �t�F�[�Y2: �ړ��k��
	const float DURATION = 0.5f; // �e�{�^���̈ړ��ɂ����鎞��
	m_pAnimation->CreateAnimationSequence({
		DELAYS.back() + DURATION, // �S�̂̉��o����
		[this, DELAYS, DURATION](float globalT) {
			// globalT��0�`1�ŃV�[�P���X�S�̂ɑΉ�����i�s�x
			float totalTime = globalT * (DELAYS.back() + DURATION); // ���ۂ̌o�ߕb��
			for (size_t i = 0; i < m_buttonRects.size(); ++i)
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
	// �ŏI�Î~�t�F�[�Y�p�̔ԕ�
	m_pAnimation->CreateAnimationSequence({
			0.0f, // ����
			[this](float) {
			// �ŏI�Î~�ʒu�ƃT�C�Y�ɐݒ�
			// �{�^���̐����[�v
			for (size_t i = 0; i < m_buttonRects.size(); ++i)
			{
				// �ŏI�I�Ȉʒu�ɐݒ�
				m_buttonRects[i].position = POSITIONS[i];
				// �ŏI�I�ȃT�C�Y�ɐݒ�
				m_buttonRects[i].size = SIZES[i];
			}
	} });
}
/*
*	@brief �q�b�g�����{�^���̃C���f�b�N�X���擾
*	@details �^�C�g���V�[���̃{�^�����Ǘ�����N���X�ŁA�q�b�g�����{�^���̃C���f�b�N�X���擾����
*	@param �Ȃ�
*	@return �q�b�g�����{�^���̃C���f�b�N�X�B�q�b�g���Ă��Ȃ��ꍇ��-1��Ԃ�
*/
int TitleButton::GetHitButtonIndex() const
{
	// �q�b�g�����{�^���̃C���f�b�N�X��Ԃ�
	for (size_t i = 0; i < m_isHit.size(); ++i)if (m_isHit[i])return static_cast<int>(i);
	// �q�b�g�����{�^�����Ȃ��ꍇ��-1��Ԃ�
	return -1;
}