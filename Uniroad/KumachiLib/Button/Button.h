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
#include "KumachiLib/Image/Image.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/MouseClick/MouseClick.h"

// �O���錾
class CommonResources;

// �{�^���̑匳�ƂȂ�N���X
class Button
{
public:
	// �A�N�Z�T
	// ���_�V�F�[�_�[�̃t�@�C���p�X��ݒ�
	void SetVertexShaderFilePath(std::string filePath) { m_pImage->SetVertexShaderFilePath(filePath); }
	// �s�N�Z���V�F�[�_�[�̃t�@�C���p�X��ݒ�
	void SetPixelShaderFilePath(std::string filePath) { m_pImage->SetPixelShaderFilePath(filePath); }
	// �e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pImage->SetTexture(pTexture); }
	// �V�F�[�_�[�o�b�t�@�T�C�Y��ݒ�
	void SetShaderBufferSize(int size) { m_pImage->SetShaderBufferSize(size); }
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
	template<typename T>
	void DrawQuadWithBuffer(const Rect& rect, const T& buffer);
	// �����蔻��
	bool Hit(const DirectX::SimpleMath::Vector2& mousePosition, const Rect& buttonRect);

private:
	// private�萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �摜�N���X
	std::unique_ptr<Image> m_pImage;
};

/*
*	@brief �`��
*	@details �{�^���̕`����s��
*	@param rect �{�^���̋�`
*	@param buffer �`��Ɏg�p����o�b�t�@
*	@return �Ȃ�
*/
template<typename T>
inline void Button::DrawQuadWithBuffer(const Rect& rect, const T& buffer)
{
	// �摜��`��
	m_pImage->DrawQuadWithBuffer(rect, buffer);
}
