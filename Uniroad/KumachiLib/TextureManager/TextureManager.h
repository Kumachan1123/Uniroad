/*
*	@file TextureManager.h
*	@brief �e�N�X�`���}�l�[�W���[�N���X
*/
#pragma once
#ifndef TEXTUREMANAGER_DEFINED
#define TEXTUREMANAGER_DEFINED
// �W�����C�u����
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>

// �O���錾
class CommonResources;

// �e�N�X�`���}�l�[�W���[�N���X
class TextureManager
{
public:
	// �A�N�Z�T
	// �e�N�X�`���̎擾
	ID3D11ShaderResourceView* GetTexture(const std::string& key);
public:
	// public�֐�
	// �R���X�g���N�^
	TextureManager();
	// �f�X�g���N�^
	~TextureManager();
	// �e�N�X�`���̏�����
	void Initialize(ID3D11Device1* pDevice);
private:
	// private�֐�
	// �e�N�X�`���̓ǂݍ���
	void LoadTexture(const std::string& key, const wchar_t* path);
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// �e�N�X�`���̃}�b�v
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextureMap;
};
#endif // TEXTUREMANAGER_DEFINED