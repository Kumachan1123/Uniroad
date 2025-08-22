/*
*	@file Image.h
*	@brief �摜�N���X
*/
#pragma once
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
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/ShaderBuffer/ShaderBuffer.h"
// �O���錾
class CommonResources;

// �摜�N���X
class Image
{
public:
	// �A�N�Z�T
	// ���_�V�F�[�_�[�̃t�@�C���p�X��ݒ�
	void SetVertexShaderFilePath(std::string filePath) { m_vertexShaderFilePath = filePath; }
	// �s�N�Z���V�F�[�_�[�̃t�@�C���p�X��ݒ�
	void SetPixelShaderFilePath(std::string filePath) { m_pixelShaderFilePath = filePath; }
	// �e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTextures.push_back(pTexture); }
	// �o�b�t�@�T�C�Y��ݒ�
	void SetShaderBufferSize(UINT size) { m_bufferSize = size; }
public:
	// public�֐�
	// �R���X�g���N�^
	Image();
	// �f�X�g���N�^
	~Image();
	// ������
	void Initialize(CommonResources* resources, int width, int height);
	// �X�V
	void Update(const float elapsedTime);

	// �摜��\��
	template<typename T>
	void DrawQuadWithBuffer(const Rect& buttonRect, const T& buffer);
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
	SpriteSheetBuffer m_constBuffer;
	// �o�b�t�@�T�C�Y
	UINT m_bufferSize;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���_���
	DirectX::DX11::VertexPositionTexture m_vertices[VERTEX_COUNT];
};

/*
*	@brief �摜��\��
*	@details �摜��\������
*	@param rect ��`
*	@param buffer �V�F�[�_�[�o�b�t�@
*	@return �Ȃ�
*/
template<typename T>
inline void Image::DrawQuadWithBuffer(const Rect& buttonRect, const T& buffer)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �A�X�y�N�g����l������Y�����T�C�Y��␳
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = buttonRect.size.y * aspect;
	// �{�^���̋�`��ݒ�
	float startX = buttonRect.position.x - buttonRect.size.x / 2.0f;
	float startY = buttonRect.position.y - correctedHeight / 2.0f;
	// NDC���W�n�ɕϊ�
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + buttonRect.size.x, startY + correctedHeight));
	// ���_���W�̐ݒ�
	VertexPositionTexture vertices[VERTEX_COUNT]{};
	vertices[0] = { VertexPositionTexture(Vector3(ndcLT.x, ndcLT.y, 0), Vector2(0, 0)) };// ����
	vertices[1] = { VertexPositionTexture(Vector3(ndcRB.x, ndcLT.y, 0), Vector2(1, 0)) };// �E��
	vertices[2] = { VertexPositionTexture(Vector3(ndcRB.x, ndcRB.y, 0), Vector2(1, 1)) };// �E��
	vertices[3] = { VertexPositionTexture(Vector3(ndcLT.x, ndcRB.y, 0), Vector2(0, 1)) };// ����
	// �󂯓n���p�o�b�t�@�̓��e�X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &buffer);
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �f�v�X�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertices);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
