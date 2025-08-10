/*
*	@file TitleLogo.cpp
*	@brief �^�C�g�����S�N���X
*/
#include "pch.h"
#include "TitleLogo.h"

// ���W���`
const DirectX::SimpleMath::Vector2 TitleLogo::POSITION(0.125f, 0.125f);
// �T�C�Y���`
const DirectX::SimpleMath::Vector2 TitleLogo::SIZE(0.55f / 2.5, 0.35f / 2.5);
/*
*	@brief �R���X�g���N�^
*	@details �^�C�g�����S�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleLogo::TitleLogo()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pImage(std::make_unique<Image>()) // �摜�ւ̃|�C���^
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // ���S�̈ʒu
	, m_size(DirectX::SimpleMath::Vector2(0.55f, 0.35f)) // ���S�̃T�C�Y
	, m_frameRows(1) // �摜�̍s��
	, m_frameCols(1) // �摜�̗�
	, m_animationPhase(0) // �A�j���[�V�����t�F�[�Y
	, m_animationTime(0.0f) // �A�j���[�V��������
	, m_currentStep(0) // ���݂̃A�j���[�V�����X�e�b�v
	, m_animStepTime(0.0f) // �A�j���[�V�����X�e�b�v����
{
}
/*
*	@brief �f�X�g���N�^
*	@details �^�C�g�����S�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleLogo::~TitleLogo()
{
	// ���ʃ��\�[�X�ւ̃|�C���^��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
}
/*
*	@brief ������
*	@details �^�C�g�����S�N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void TitleLogo::Initialize(CommonResources* resources, int width, int height)
{
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �摜���쐬
	m_pImage = std::make_unique<Image>();
	// �V�F�[�_�[�p�X��n��
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// �摜��ݒ�
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Title"));
	// �摜�̏�����
	m_pImage->Initialize(m_pCommonResources, width, height);
	// ��`��ݒ�
	m_logoRect.position = POSITION;
	m_logoRect.size = SIZE;
	// �A�j���[�V�����V�[�P���X���쐬
	CreateAnimationSequence();
}
/*
*	@brief �X�V
*	@details �^�C�g�����S�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void TitleLogo::Update(float elapsedTime)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �A�j���[�V�����V�[�P���X�����쐬�Ȃ�ΏI��
	if (m_currentStep >= m_animSequence.size()) return;
	// �A�j���[�V�������Ԃ��X�V
	m_animStepTime += elapsedTime;
	// �A�j���[�V�����X�e�b�v�̍X�V
	float duration = m_animSequence[m_currentStep].duration;
	// �i�s�x���v�Z
	float t = duration > 0.0f ? std::min(m_animStepTime / duration, 1.0f) : 0.0f;
	// �A�j���[�V���������s
	m_animSequence[m_currentStep].updateFunc(t);
	// �A�j���[�V�������I��������
	if (duration > 0.0f && m_animStepTime >= duration)
	{
		// ���̃X�e�b�v�֐i��
		m_currentStep++;
		// �A�j���[�V�����X�e�b�v���Ԃ����Z�b�g
		m_animStepTime = 0.0f;
	}
}
/*
*	@brief �摜��\��
*	@details �^�C�g�����S�̉摜��\������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleLogo::Render()
{
	// �摜��`��
	m_pImage->DrawQuad(m_logoRect, 0, m_frameCols, m_frameRows);
}
/*
*	@brief �A�j���[�V�����V�[�P���X���쐬
*	@details �^�C�g�����S�̃A�j���[�V�����V�[�P���X���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleLogo::CreateAnimationSequence()
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �A�j���[�V�����V�[�P���X�̏�����
	m_animSequence =
	{
		// �t�F�[�Y0: �g��
		{
			0.5f,// �g��ɂ�����b��
			[this](float t)
			{
			// �i�s�x���v�Z
			float easing = Easing::EaseOutBack(t);
			// �����Œ�A�T�C�Y�����C�[�W���O���
			m_logoRect.position = Vector2(0.5f, 0.5f);
			// �T�C�Y���C�[�W���O���
			m_logoRect.size = Vector2::Lerp(Vector2(0.0f, 0.0f), Vector2(0.55f, 0.35f), easing);
			}
		},
		// �t�F�[�Y1: �ҋ@
		{
			2.0f,// �ҋ@����
			[this](float t)
			{
			// 0�œ��������ꏊ�ƃT�C�Y�ŌŒ�
			m_logoRect.position = Vector2(0.5f, 0.5f);
			m_logoRect.size = Vector2(0.55f, 0.35f);
			}
		},
		// �t�F�[�Y2: �ړ��k��
		{
			0.5f, // �ړ��E�k���ɂ�����b��
			[this](float t)
			{
			// �i�s�x���v�Z
			float easing = Easing::EaseInOutCubic(t);
			// ����Ɉړ����k��
			m_logoRect.position = Vector2::Lerp(Vector2(0.5f, 0.5f), POSITION, easing);
			// �T�C�Y���C�[�W���O���
			m_logoRect.size = Vector2::Lerp(Vector2(0.55f, 0.35f), SIZE, easing);
			}
		}
	};
	// �ŏI�Î~�t�F�[�Y�p�̔ԕ�
	m_animSequence.push_back({ 0.0f, // ����
		[this](float)
		{
			// �ŏI�Î~�ʒu�ƃT�C�Y�ɐݒ�
			m_logoRect.position = POSITION;
			m_logoRect.size = SIZE;
		} });
	// �A�j���[�V�����̏�����
	m_currentStep = 0;
	// �A�j���[�V�������Ԃ����Z�b�g
	m_animStepTime = 0.0f;
}
