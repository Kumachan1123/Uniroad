/*
*	@file MedalCounter.h
*	@brief ���_���J�E���^�[�̏������`����N���X
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

// �O���錾
class CommonResources;

// ���_���J�E���^�[�N���X
class MedalCounter
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
	// �J�E���^�[�̒P��
	struct Counter
	{
		int unit1 = 0;// 1�̈�
		int unit10 = 0;// 10�̈�
	};
public:
	// �A�N�Z�T
	// �W�߂����_���������擾����
	int GetCollectedMedalCount() const { return m_collectedMedalCountSave; }
	// �W�߂����_��������ݒ肷��
	void SetCollectedMedalCount(int count) { m_collectedMedalCountSave = count; }
public:
	// public�֐�
	// �R���X�g���N�^
	MedalCounter();
	// ������ 
	void Initialize(CommonResources* resources, int width, int height);
	// �X�V����
	void Update(float elapsedTime);
	// �`�悷��
	void Render();

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
public:
	// public�萔
	// ���_��
	static const int VERTEX_COUNT = 4;
	// ���_���摜�̈ʒuX
	const float MEDAL_POS_X = -1.0f;
	// ���_���摜�̈ʒuY
	const float MEDAL_POS_Y = 1.0f;
	// ���_���摜�̃T�C�YX
	const float MEDAL_SIZE_X = 0.11f;
	// ���_���摜�̃T�C�YY
	const float MEDAL_SIZE_Y = 0.11f / 0.58f;
	// �u�~�v�摜�̈ʒuX
	const float X_POS_X = -0.9f;
	// �u�~�v�摜�̈ʒuY
	const float X_POS_Y = 0.99f;
	// �u�~�v�摜�̃T�C�YX
	const float X_SIZE_X = 0.078f;
	// �u�~�v�摜�̃T�C�YY
	const float X_SIZE_Y = 0.1f / 0.58f;
	// 10�̈ʂ̈ʒuX
	const float NUMBER10_POS_X = -0.82f;
	// 10�̈ʂ̈ʒuY
	const float NUMBER10_POS_Y = 0.99f;
	// 10�̈ʂ̃T�C�YX
	const float NUMBER10_SIZE_X = 0.078f;
	// 10�̈ʂ̃T�C�YY
	const float NUMBER10_SIZE_Y = 0.1f / 0.58f;
	// 1�̈ʂ̈ʒuX
	const float NUMBER1_POS_X = -0.745f;
	// 1�̈ʂ̈ʒuY
	const float NUMBER1_POS_Y = 0.99f;
	// 1�̈ʂ̃T�C�YX
	const float NUMBER1_SIZE_X = 0.078f;
	// 1�̈ʂ̃T�C�YY
	const float NUMBER1_SIZE_Y = 0.1f / 0.58f;
private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �`��\�͈͂̕�
	int m_viewportWidth;
	// �`��\�͈͂̍���
	int m_viewportHeight;
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	// �W�߂����_������
	Counter m_collectedMedalCount;
	// �W�߂����_�������i�ۑ��p�j
	int m_collectedMedalCountSave;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���_���̃e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pMedalTextures;
	// ���_���̒��_���
	DirectX::DX11::VertexPositionTexture m_verticesMedal[VERTEX_COUNT];
	// �u�~�v�̉摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pXTextures;
	// �u�~�v�̒��_���
	DirectX::DX11::VertexPositionTexture m_verticesX[VERTEX_COUNT];
	// �����摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pNumberTextures;
	// �����̒��_���(10�̈�)
	DirectX::DX11::VertexPositionTexture m_verticesNumber10[VERTEX_COUNT];
	// �����̒��_���(1�̈�)
	DirectX::DX11::VertexPositionTexture m_verticesNumber1[VERTEX_COUNT];
};