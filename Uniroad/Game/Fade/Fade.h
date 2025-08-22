/*
*	@file Fade.h
*	@brief �t�F�[�h�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"
#include "Game/Interface/IImage.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
#include "KumachiLib/Animaiton/Animation.h"

// �O���錾
class CommonResources;

// �t�F�[�h�N���X
class Fade : public IImage
{
public:
	// public�񋓌^
	// �t�F�[�h���
	enum class FadeState
	{
		None,// �Ȃ�
		// �ȉ��A�t�F�[�h�C���E�A�E�g�̊J�n���
		FadeIn,// �t�F�[�h�C��
		FadeOut,// �t�F�[�h�A�E�g
		// �ȉ��A�t�F�[�h�C���E�A�E�g�̓r�����
		FadeInEnd,// �t�F�[�h�C���I��
		FadeOutEnd,// �t�F�[�h�A�E�g�I��
	};
public:
	// �A�N�Z�T
	// �t�F�[�h��Ԃ̎擾
	void SetState(FadeState state) { m_fadeState = state; }
	// �t�F�[�h��Ԃ̐ݒ�
	FadeState GetState() const { return m_fadeState; }
	// ���S�̈ʒu���擾
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ���S�̈ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// ���S�̃T�C�Y���擾
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// ���S�̃T�C�Y��ݒ�
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public�֐�
	// �R���X�g���N�^
	Fade();
	// �f�X�g���N�^
	~Fade();
	// ������
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V
	void Update(float elapsedTime)override;
	// �摜��\��
	void Render()override;
private:
	// private�֐�
	// �萔�o�b�t�@���X�V
	void UpdateConstantBuffer()override;
	// �t�F�[�h�A�E�g
	void FadeOut(float elapsedTime);
	// �t�F�[�h�C��
	void FadeIn(float elapsedTime);
public:
	// public�萔
	// �t�F�[�h�ŏ��l
	static const float FADE_MIN;
	// �t�F�[�h�ő�l
	static const float FADE_MAX;
	// �t�F�[�h���x
	static const float FADE_SPEED;
	// �t�F�[�h�����l
	static const float FADE_INIT;
	// �t�F�[�h�̊��炩��
	static const float FADE_SMOOTHNESS;
private:
	// private�萔
	// �\���ʒu�i����j
	static const DirectX::SimpleMath::Vector2 POSITION;
	// �\���T�C�Y
	static const DirectX::SimpleMath::Vector2 SIZE;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �t�F�[�h���
	FadeState m_fadeState;
	// �摜
	std::unique_ptr<Image> m_pImage;
	// ���S�̋�`
	Rect m_rect;
	// �t�F�[�h����
	float m_fadeTime;
	// �萔�o�b�t�@
	FadeBuffer m_fadeBuffer;
	// ���W
	DirectX::SimpleMath::Vector2 m_position;
	// �T�C�Y
	DirectX::SimpleMath::Vector2 m_size;
};