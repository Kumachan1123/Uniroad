/*
*	@file ModelManager.h
*	@brief ���f���}�l�[�W���[�N���X
*/
#pragma once
#ifndef MODEL_MANAGER_DEFINED
#define MODEL_MANAGER_DEFINED
// �W�����C�u����
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>

// ���f���}�l�[�W���[�N���X
class ModelManager
{
public:
	// �A�N�Z�T
	// ���f���̎擾
	DirectX::Model* GetModel(const std::string& key);
	// �w�肳�ꂽ�X�e�[�W�̋�̃��f�����쐬���ĕԂ�
	DirectX::Model* GetSkyModel(const std::string& stageID);
public:
	// public�֐�
	// �R���X�g���N�^
	ModelManager();
	// �f�X�g���N�^
	~ModelManager();
	// ���f���̏�����
	void Initialize(ID3D11Device1* pDevice);
private:
	// private�֐�
	// �e���f���ɃG�t�F�N�g�ݒ�
	void SetupBulletModelEffects();
	// �X�e�[�W���f���ɃG�t�F�N�g�ݒ�
	void SetupStageModelEffects();
	// JSON�t�@�C���̓ǂݍ���
	void LoadJsonFile();
	//�V�����f���̃p�X��JSON�t�@�C������ǂݍ���
	void LoadSkyModelsJson();
private:
	// private�ϐ�
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���f���̃}�b�v
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// ���f���p�X�̃}�b�v
	std::unordered_map<std::string, std::wstring> m_skyModelPathMap;
	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
#endif // MODEL_MANAGER_DEFINED
