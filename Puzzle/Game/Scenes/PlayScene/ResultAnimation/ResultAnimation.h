/*
*	@file ResultAnimation.h
*	@brief ���ʃA�j���[�V�����N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/Easing/Easing.h"
// �O���錾
class CommonResources;

// ���ʃA�j���[�V�����N���X
class ResultAnimation
{
private:
	// private�\����
	// �V�F�[�_�[�ɓn���萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��
	};
public:
	// �A�N�Z�T
	// �v���C�V�[�����猋�ʂ��󂯎��
	void SetResult(bool gameover, bool gameclear);
	// �A�j���[�V�����I���t���O���擾����
	bool IsAnimationEnd() const { return m_animationEnd; }
	// �A�j���[�V�����I���t���O��ݒ肷��
	void SetAnimationEnd(bool end) { m_animationEnd = end; }
	// �A�j���[�V�����L���t���O���擾����
	bool IsAnimationEnable() const { return m_animationEnable; }
	// �A�j���[�V�����L���t���O��ݒ肷��
	void SetAnimationEnable(bool enable) { m_animationEnable = enable; }

public:
	// public�֐�
	// �R���X�g���N�^
	ResultAnimation();
	// �f�X�g���N�^
	~ResultAnimation();
	// ������
	void Initialize(CommonResources* resources);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �e�N�X�`������
	void DecideTexture();
private:
	// private�֐�
	// �V�F�[�_�[�̍쐬
	void CreateShaders();
	// �摜��\��
	void DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
		DirectX::VertexPositionTexture* vertices,
		float startX, float startY, float width, float height,
		int frameIndex, int frameCols, int frameRows);
private:
	// private�萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// ���_��
	static const int VERTEX_COUNT = 4;
	// �摜�̈ʒuX(�Q�[���N���A�j
	const float  POS_X_CLEAR = -0.75f;
	// �摜�̈ʒuX(�Q�[���I�[�o�[�j
	const float POS_X_OVER = -0.65f;
	// �摜�̈ʒuY
	const float POS_Y = 0.75;
	// �摜�̃T�C�YX
	const float SIZE_X = 1.0f * 0.16f;
	// �摜�̃T�C�YY
	const float SIZE_Y = 1.0f * 0.32f;

private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �Q�[���I�[�o�[�t���O
	bool m_gameover;
	// �Q�[���N���A�t���O
	bool m_gameclear;
	// ����
	float m_time;
	// �A�j���[�V�����L���t���O
	bool m_animationEnable;
	// �A�j���[�V�����I���t���O
	bool m_animationEnd;
	// �摜����t���O
	bool m_decideTexture;
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// �摜�̈ʒuX
	float m_positionX;
	// �ʂɑ��삷�邽�߂̉摜�̈ʒuY
	std::vector<float> m_positionsY;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���_���
	DirectX::DX11::VertexPositionTexture m_vertices[VERTEX_COUNT];
};
