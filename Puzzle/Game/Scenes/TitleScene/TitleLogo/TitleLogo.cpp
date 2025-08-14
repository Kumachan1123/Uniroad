/*
*	@file TitleLogo.cpp
*	@brief �^�C�g�����S�N���X
*/
#include "pch.h"
#include "TitleLogo.h"

// ���W���`
const DirectX::SimpleMath::Vector2 TitleLogo::POSITION(0.125f, 0.15f);
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
	// �A�j���[�V�����N���X���쐬
	m_pAnimation = std::make_unique<Animation>();
	// �摜���쐬
	m_pImage = std::make_unique<Image>();
	// �V�F�[�_�[�p�X��n��
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// �摜��ݒ�
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Title"));
	// �V�F�[�_�[�o�b�t�@�T�C�Y��ݒ�
	m_pImage->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
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
	// �A�j���[�V�������X�V
	m_pAnimation->Update(elapsedTime);
	// �萔�o�b�t�@���X�V
	UpdateConstantBuffer();
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
	m_pImage->DrawQuadWithBuffer(m_logoRect, m_spriteSheetBuffer);
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

	// �t�F�[�Y0: �g��
	m_pAnimation->CreateAnimationSequence({
			0.5f,// �g��ɂ�����b��
			[this](float t) {
			// �i�s�x���v�Z
			float easing = Easing::EaseOutBack(t);
			// �����Œ�A�T�C�Y�����C�[�W���O���
			m_logoRect.position = Vector2(0.5f, 0.5f);
			// �T�C�Y���C�[�W���O���
			m_logoRect.size = Vector2::Lerp(Vector2(0.0f, 0.0f), Vector2(0.55f, 0.35f), easing);
	} });
	// �t�F�[�Y1: �ҋ@
	m_pAnimation->CreateAnimationSequence({
			2.0f,// �ҋ@����
			[this](float) {
			// 0�œ��������ꏊ�ƃT�C�Y�ŌŒ�
			m_logoRect.position = Vector2(0.5f, 0.5f);
			m_logoRect.size = Vector2(0.55f, 0.35f);
	} });
	// �t�F�[�Y2: �ړ��k��
	m_pAnimation->CreateAnimationSequence({
			0.5f, // �ړ��E�k���ɂ�����b��
			[this](float t) {
			// �i�s�x���v�Z
			float easing = Easing::EaseInOutCubic(t);
			// ����Ɉړ����k��
			m_logoRect.position = Vector2::Lerp(Vector2(0.5f, 0.5f), POSITION, easing);
			// �T�C�Y���C�[�W���O���
			m_logoRect.size = Vector2::Lerp(Vector2(0.55f, 0.35f), SIZE, easing);
	} });
	// �ŏI�Î~�t�F�[�Y�p�̔ԕ�
	m_pAnimation->CreateAnimationSequence({
			0.0f, // ����
			[this](float) {
			// �ŏI�Î~�ʒu�ƃT�C�Y�ɐݒ�
			m_logoRect.position = POSITION;
			m_logoRect.size = SIZE;
	} });

}
/*
*	@brief �萔�o�b�t�@���X�V
*	@details �^�C�g�����S�̒萔�o�b�t�@���X�V����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleLogo::UpdateConstantBuffer()
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
