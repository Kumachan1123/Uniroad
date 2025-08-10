/*
*	@file Button.h
*	@brief �{�^���̑匳�ƂȂ�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <cassert>
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/MouseClick/MouseClick.h"

// �O���錾
class CommonResources;

// �{�^���̑匳�ƂȂ�N���X
class Button
{
public:
	// public�\����
	// �{�^���̋�`
	struct ButtonRect
	{
		DirectX::SimpleMath::Vector2 position; // �ʒu
		DirectX::SimpleMath::Vector2 size;     // �T�C�Y
	};
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
	// ���_�V�F�[�_�[�̃t�@�C���p�X��ݒ�
	void SetVertexShaderFilePath(std::string filePath) { m_vertexShaderFilePath = filePath; }
	// �s�N�Z���V�F�[�_�[�̃t�@�C���p�X��ݒ�
	void SetPixelShaderFilePath(std::string filePath) { m_pixelShaderFilePath = filePath; }
	// �e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTextures.push_back(pTexture); }
public:
	// public�֐�
	// �R���X�g���N�^
	Button();
	// �f�X�g���N�^
	~Button();
	// ������
	void Initialize(CommonResources* resources, int width, int height);
	// �X�V
	void Update(const float elapsedTime);
	// �摜��\��
	void DrawQuad(float centerX, float centerY, float width, float height, int frameIndex, int frameCols, int frameRows);
	// �����蔻��
	bool Hit(const DirectX::SimpleMath::Vector2& mousePosition, const ButtonRect& buttonRect);
private:
	// private�֐�
	// �V�F�[�_�[�̍쐬
	void CreateShaders();
private:
	// private�萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// ���_��
	static const int VERTEX_COUNT = 4;
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
	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ���_�V�F�[�_�[�̃t�@�C���p�X
	std::string m_vertexShaderFilePath;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �s�N�Z���V�F�[�_�[�̃t�@�C���p�X
	std::string m_pixelShaderFilePath;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���_���
	DirectX::DX11::VertexPositionTexture m_vertices[VERTEX_COUNT];

};