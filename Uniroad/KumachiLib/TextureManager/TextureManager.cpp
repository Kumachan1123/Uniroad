/*
*	@file TextureManager.cpp
*	@brief �e�N�X�`���}�l�[�W���[�N���X�̎���
*/
#include <pch.h>
#include "TextureManager.h"
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief �R���X�g���N�^
*	@details �e�N�X�`���}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TextureManager::TextureManager()
	: m_pCommonResources(nullptr)// ���ʃ��\�[�X
	, m_pDevice(nullptr)// �f�o�C�X
	, m_pTextureMap()// �e�N�X�`���}�b�v
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e�N�X�`���}�l�[�W���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TextureManager::~TextureManager()
{
	// �e�N�X�`���}�b�v�̉��
	for (auto& pair : m_pTextureMap)pair.second.Reset();
	// �}�b�v�̃N���A
	m_pTextureMap.clear();
}
/*
*	@brief �e�N�X�`���̏�����
*	@details �e�N�X�`���}�l�[�W���[�N���X�̏��������s��
*	@param pDevice �f�o�C�X
*	@return �Ȃ�
*/
void TextureManager::Initialize(ID3D11Device1* pDevice)
{
	// nlohmann::json�̃G�C���A�X���`
	using json = nlohmann::json;
	// �W�����O��Ԃ��g�p
	using namespace std;
	// �f�o�C�X�̐ݒ�
	m_pDevice = pDevice;
	//�ǂݍ��ރt�@�C���̖��O���쐬
	string filename = "Resources/Jsons/textures.json";
	//�t�@�C�����J��
	ifstream ifs(filename.c_str());
	// �t�@�C��������ɊJ���Ȃ������狭���I��
	if (!ifs.good())return;
	//json�I�u�W�F�N�g
	json j;
	//�t�@�C������ǂݍ���
	ifs >> j;
	//�t�@�C�������
	ifs.close();
	// JSON�̊e�A�C�e���ɑ΂��ă��[�v
	for (const auto& item : j.items())
	{
		// �L�[
		std::string key = item.key();
		// �l�i�t�@�C���p�X�j
		std::string path = item.value();
		// ������ϊ�
		std::wstring wpath(path.begin(), path.end());
		// �ǂݍ��݊֐�
		LoadTexture(key, wpath.c_str());
	}
}
/*
*	@brief �e�N�X�`���̓ǂݍ���
*	@details �w�肳�ꂽ�L�[�ƃp�X�Ńe�N�X�`����ǂݍ��݁A�}�b�v�ɒǉ�����
*	@param key �e�N�X�`���̃L�[
*	@param path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void TextureManager::LoadTexture(const std::string& key, const wchar_t* path)
{
	// �e�N�X�`���i�[�p
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(m_pDevice, path, nullptr, texture.ReleaseAndGetAddressOf());
	// �}�b�v�ɒǉ�
	m_pTextureMap[key] = texture;

}
/*
*	@brief �e�N�X�`���̎擾
*	@details �w�肳�ꂽ�L�[�̃e�N�X�`�����擾����
*	@param key �e�N�X�`���̃L�[
*	@return �w�肳�ꂽ�L�[�̃e�N�X�`���ւ̃|�C���^
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& key)
{
	// �w�肳�ꂽ�L�[�̃e�N�X�`�����擾
	return m_pTextureMap[key].Get();
}