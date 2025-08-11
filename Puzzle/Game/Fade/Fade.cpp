/*
*	@file Fade.cpp
*	@brief �t�F�[�h�N���X�̎����t�@�C��
*/
#include "pch.h"
#include "Fade.h"

// �t�F�[�h�ŏ��l
const float Fade::FADE_MIN = -1.01f;
// �t�F�[�h�ő�l
const float Fade::FADE_MAX = 1.01f;
// �t�F�[�h���x
const float Fade::FADE_SPEED = 1.5f;
// �t�F�[�h�����l
const float Fade::FADE_INIT = -0.75f;
// �t�F�[�h�̊��炩��
const float Fade::FADE_SMOOTHNESS = 0.01f;
// ���W���`
const DirectX::SimpleMath::Vector2 Fade::POSITION(1.0f, 1.0f);
// �T�C�Y���`
const DirectX::SimpleMath::Vector2 Fade::SIZE(2.0f, 2.0f);

/*
*	@brief �R���X�g���N�^
*	@details �t�F�[�h�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Fade::Fade()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pImage(std::make_unique<Image>()) // �摜�ւ̃|�C���^
	, m_position(POSITION) // �t�F�[�h�̈ʒu
	, m_size(SIZE) // �t�F�[�h�̃T�C�Y
	, m_fadeTime{ FADE_INIT }// �t�F�[�h����
	, m_fadeState{ FadeState::None }// �t�F�[�h���
{
}
/*
*	@brief �f�X�g���N�^
*	@details �t�F�[�h�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Fade::~Fade()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief ������
*	@details �t�F�[�h�N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Fade::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �摜���쐬
	m_pImage = std::make_unique<Image>();
	// �V�F�[�_�[�p�X��n��
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Fade/VS_Fade.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Fade/PS_Fade.cso");
	// �摜��ݒ�
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Fade"));
	// �V�F�[�_�[�o�b�t�@�T�C�Y��ݒ�
	m_pImage->SetShaderBufferSize(sizeof(FadeBuffer));
	// �摜�̏�����
	m_pImage->Initialize(m_pCommonResources, width, height);
	// ��`��ݒ�
	m_rect.position = POSITION;
	m_rect.size = SIZE;
}
/*
*	@brief �X�V
*	@details �t�F�[�h�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Fade::Update(float elapsedTime)
{
	// �t�F�[�h�C��
	FadeIn(elapsedTime);
	// �t�F�[�h�A�E�g
	FadeOut(elapsedTime);
	// �萔�o�b�t�@���X�V
	UpdateConstantBuffer();
}
/*
*	@brief �摜��\��
*	@details �t�F�[�h�N���X�̉摜��\������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Fade::Render()
{
	// �t�F�[�h�`��
	m_pImage->DrawQuadWithBuffer(m_rect, m_fadeBuffer);
}
/*
*	@brief �萔�o�b�t�@���X�V
*	@details �t�F�[�h�N���X�̒萔�o�b�t�@���X�V����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Fade::UpdateConstantBuffer()
{
	// �t�F�[�h�̊��炩����ݒ�
	m_fadeBuffer.smoothness = FADE_SMOOTHNESS;
	// �t�F�[�h�̐i�s�x��ݒ�
	m_fadeBuffer.fadeAmount = m_fadeTime;
}
/*
*	@brief �t�F�[�h�A�E�g
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::FadeOut(float elapsedTime)
{
	// �t�F�[�h�A�E�g���Ȃ�
	if (m_fadeState == FadeState::FadeOut)
	{
		// ���Ԃ��v�Z
		m_fadeTime -= elapsedTime * FADE_SPEED;
		// �t�F�[�h�𐧌�
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// �t�F�[�h���ŏ��l�ɂȂ�����t�F�[�h�A�E�g�I��
		if (m_fadeTime <= FADE_MIN)
			m_fadeState = FadeState::FadeOutEnd;
	}
}
/*
*	@brief �t�F�[�h�C��
*	@param elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Fade::FadeIn(float elapsedTime)
{
	// �t�F�[�h�C�����Ȃ�
	if (m_fadeState == FadeState::FadeIn)
	{
		// ���Ԃ��v�Z
		m_fadeTime += elapsedTime * FADE_SPEED;
		// �t�F�[�h�𐧌�
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// �t�F�[�h���ő�l�ɂȂ�����t�F�[�h�C���I��
		if (m_fadeTime >= FADE_MAX)
			m_fadeState = FadeState::FadeInEnd;
	}
}
