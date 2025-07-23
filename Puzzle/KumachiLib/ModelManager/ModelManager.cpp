/*
*	@file ModelManager.cpp
*	@brief ���f���}�l�[�W���[�N���X
*/
#include <pch.h>
#include "ModelManager.h"
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"

/*
*	@brief �R���X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ModelManager::ModelManager()
	: m_pDevice(nullptr) // �f�o�C�X
	, m_pModelMap() // ���f���̃}�b�v
	, m_skyModelPathMap() // �V�����f���p�X�̃}�b�v
	, m_pEffectFactory(nullptr) // �G�t�F�N�g�t�@�N�g���[
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ModelManager::~ModelManager()
{
	// ���f���̃}�b�v���N���A
	m_pModelMap.clear();
	// �G�t�F�N�g�t�@�N�g���[�����Z�b�g
	m_pEffectFactory.reset();
	// �f�o�C�X��nullptr�ɐݒ�
	m_pDevice = nullptr;
}
/*
*	@brief ���f���̏�����
*	@details �e�탂�f���̃��[�h�ƃG�t�F�N�g�̐ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::Initialize(ID3D11Device1* pDevice)
{
	// �f�o�C�X��ݒ�
	m_pDevice = pDevice;
	// �G�t�F�N�g�t�@�N�g���[�̍쐬
	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pDevice);
	// �G�t�F�N�g�̋��L�𖳌��ɂ���
	m_pEffectFactory->SetSharing(false);
	// JSON�t�@�C���̓ǂݍ���
	LoadJsonFile();
	//// �V�����f���̃p�X��JSON�t�@�C������ǂݍ���
	//LoadSkyModelsJson();
	//// �e���f���̍쐬
	//SetupBulletModelEffects();
	//// �X�e�[�W���f���̍쐬
	//SetupStageModelEffects();
}
/*
*	@brief JSON�t�@�C���̓ǂݍ���
*	@details ���f���̃p�X��JSON�t�@�C������ǂݍ���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::LoadJsonFile()
{
	// nlohmann::json�̃G�C���A�X���`
	using json = nlohmann::json;
	// �W�����O��Ԃ��g�p
	using namespace std;
	//�ǂݍ��ރt�@�C���̖��O���쐬
	string filename = "Resources/Jsons/Models.json";
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
		// ���f���̃f�B���N�g�����w��
		m_pEffectFactory->SetDirectory(L"Resources/Models");
		// ���f����ǂݍ���
		m_pModelMap[key] = DirectX::Model::CreateFromCMO(m_pDevice, wpath.c_str(), *m_pEffectFactory);
		// �G�e���f���̃G�t�F�N�g��ݒ肷��
		m_pModelMap[key]->UpdateEffects([&](DirectX::IEffect* effect)
			{
				// �G�t�F�N�g��BasicEffect�ɃL���X�g
				auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
				// �e���C�g�𖳌��ɂ���
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, true);
			});
	}

}
void ModelManager::LoadSkyModelsJson()
{
	// nlohmann::json�̃G�C���A�X���`
	using json = nlohmann::json;
	// �W�����O��Ԃ��g�p
	using namespace std;
	//�ǂݍ��ރt�@�C���̖��O���쐬
	string filename = "Resources/Jsons/SkyModels.json";
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
		// ���f���p�X���}�b�v�ɒǉ�
		m_skyModelPathMap[key] = wpath;
	}
}
/*
*	@brief �e���f���̃G�t�F�N�g�ݒ�
*	@details ���e�ƓG�e�̃��f���ɃG�t�F�N�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::SetupBulletModelEffects()
{
	// ���e���f���̃G�t�F�N�g��ݒ肷��
	m_pModelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// �G�t�F�N�g��BasicEffect�ɃL���X�g
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			// �g�U�F��ݒ�
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);
			// �������F��ݒ�
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});
	// �G�e���f���̃G�t�F�N�g��ݒ肷��
	m_pModelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// �G�t�F�N�g��BasicEffect�ɃL���X�g
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			// �g�U�F��ݒ�
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, 0.2f, 0, 1));
			// �����̐F��ݒ�
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red);
			// �������F��ݒ�
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato);
		});
}

/*
*	@brief �X�e�[�W���f���̃G�t�F�N�g�ݒ�
*	@details �X�e�[�W���f���ɃG�t�F�N�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::SetupStageModelEffects()
{
	// �X�e�[�W���f���̃G�t�F�N�g��ݒ肷��
	m_pModelMap["Stage"]->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �G�t�F�N�g��BasicEffect�ɃL���X�g
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			// �G�t�F�N�g��nullptr�̏ꍇ�͏������I����
			if (!basicEffect)return;
			// �e���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(0, false);
			basicEffect->SetLightEnabled(1, false);
			basicEffect->SetLightEnabled(2, false);
			// ���f����������������
			basicEffect->SetEmissiveColor(DirectX::Colors::White);
		}
	);
}
/*
*	@brief ���f�����擾����
*	@details �w�肳�ꂽ�L�[�ɑΉ����郂�f�����擾����
*	@param key ���f���̃L�[
*	@return �w�肳�ꂽ�L�[�ɑΉ����郂�f���̃|�C���^�B������Ȃ��ꍇ��nullptr��Ԃ�
*/
DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	// �L�[�ɑΉ����郂�f��������
	auto it = m_pModelMap.find(key);
	// ���������ꍇ�̓��f����Ԃ�
	if (it != m_pModelMap.end())	return it->second.get();
	// ������Ȃ������ꍇ��nullptr��Ԃ�
	return nullptr;
}
/*
*	@brief �w�肳�ꂽ�X�e�[�W�̋�̃��f�����쐬���ĕԂ�
*	@details �X�e�[�WID�ɉ�������̃��f�����擾����
*	@param stageID �X�e�[�WID
*	@return �w�肳�ꂽ�X�e�[�W�̋�̃��f���̃|�C���^�B������Ȃ��ꍇ��nullptr��Ԃ�
*/
DirectX::Model* ModelManager::GetSkyModel(const std::string& stageID)
{
	// �X�e�[�WID�ɑΉ������̃��f���̃p�X������
	auto it = m_skyModelPathMap.find(stageID);
	// ������Ȃ������ꍇ��nullptr��Ԃ�
	if (it == m_skyModelPathMap.end())	return nullptr;
	// ���f����ێ�
	m_pModelMap["Sky"] = DirectX::Model::CreateFromCMO(m_pDevice, it->second.c_str(), *m_pEffectFactory);
	// ���f����Ԃ�
	return m_pModelMap["Sky"].get();
}
