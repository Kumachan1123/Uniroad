/*
*	@file ShaderBuffer.h
*	@brief �V�F�[�_�[�o�b�t�@�̍\���̂��܂Ƃ߂��w�b�_�[�t�@�C��
*/
#pragma once

// �\����
// �X�v���C�g�V�[�g�p�萔�o�b�t�@
struct SpriteSheetBuffer
{
	DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
	DirectX::SimpleMath::Matrix matView;    // �r���[�s��
	DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
	DirectX::SimpleMath::Vector4 count;     // �J�E���g
	DirectX::SimpleMath::Vector4 height;    // ����
	DirectX::SimpleMath::Vector4 width;     // ��
};
// �t�F�[�h�p�萔�o�b�t�@
struct FadeBuffer
{
	float smoothness = 0;                       // �t�F�[�h�̊��炩��
	float fadeAmount = 0;                       // �t�F�[�h�̐i�s�x�i0.0�`1.0�j
	DirectX::SimpleMath::Vector2 padding;		// �p�f�B���O
};