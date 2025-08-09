/*
*	@file Canvas.h
*	@brief �L�����o�X�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/Anchor/Anchor.h"

// �O���錾
class CommonResources;

class Canvas
{
public:
	// �񋓌^
	// �V�F�[�_�[�̎��
	enum class ShaderType
	{
		HP = 0,// HP�Q�[�W�̂悤�ȓ���������
		CIRCLE,// �~�`�Q�[�W�̂悤�ȓ���������
		ANIM,// UV�A�j���[�V����������
		SELECT,// �I����Ԃ̂悤�ȓ���������
		OTHER,// ���̑�
	};
	// �\����
	// �V�F�[�_�[�ɑ���萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 windowSize;// �E�B���h�E�T�C�Y
		float time;// ����
		DirectX::SimpleMath::Vector3 color;// �F
	};
public:
	// �A�N�Z�T
		// �E�B���h�E�̃T�C�Y��ݒ�
	void SetWindowSize(const int& width, const int& height);
	// �V�F�[�_�[�^�C�v��ݒ�
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; }
	// �X�P�[�����擾
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	// �X�P�[����ݒ�
	void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }
	// �I����Ԃ̃X�P�[�����擾
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	// �ʒu���擾
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// �ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; };
	// �A���J�[���擾
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	// �A���J�[��ݒ�
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }
	// ���Ԃ��擾
	float GetTime() const { return m_time; }
	// ���Ԃ�ݒ�
	void SetTime(float time) { m_time = time; }
	// �w��ʒu�Ƀq�b�g�������ǂ������擾
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const;
public:
	// �R���X�g���N�^
	Canvas(CommonResources* pCommonResources);
	// �f�X�g���N�^
	~Canvas();
	// �e�N�X�`���̓ǂݍ���
	void LoadTexture(std::string key);
	// UI�̍쐬
	void Create(DX::DeviceResources* pDR
		, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor);
	// �`��
	void Render();
private:
	// private�֐�
	// �V�F�[�_�[�̍쐬
	void CreateShaders();
private:
	// �萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private�ϐ�
// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ����
	float m_time;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	// �e�N�X�`���n���h���̔z��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���\�[�X�n���h��
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �R���X�^���g�o�b�t�@�i�V�F�[�_�[�ɑ���f�[�^�j
	ConstBuffer m_constBuffer;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �e�N�X�`���̕��ƍ���
	int m_textureWidth, m_textureHeight;
	// �X�P�[��
	DirectX::SimpleMath::Vector2 m_scale;
	// �x�[�X�X�P�[��
	DirectX::SimpleMath::Vector2 m_baseScale;
	// �`��ʒu
	DirectX::SimpleMath::Vector2 m_position;
	// �A���J�[
	KumachiLib::ANCHOR m_anchor;
	// �V�F�[�_�[�̎��
	ShaderType m_shaderType;
};

