/*
*	@file AudioManager.cpp
*	@brief �����Ǘ��N���X
*/
#include <pch.h>
#include "AudioManager.h"
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief �R���X�g���N�^
*	@details FMOD�V�X�e���̏��������s���A�Q�[�����Ŏg�p����S�Ă̌��ʉ���BGM��ǂݍ���
*	@param �Ȃ�
*	@retrun �Ȃ�
*/
AudioManager::AudioManager()
	: m_pFMODSystem(nullptr)// FMOD�V�X�e�� 
	, m_pSounds()// �T�E���h 
	, m_pChannels()// �`�����l�� 
{
	Initialize();// ������
}
/*
*	@brief �f�X�g���N�^
*	@details �����֘A���\�[�X�̉���������s��
*	@param �Ȃ�
*	@retrun �Ȃ�
*/
AudioManager::~AudioManager() { Shutdown(); }// FMOD�V���b�g�_�E��

/*
*	@brief ������
*	@details �V�X�e���쐬����я��������s���A
*			 �Q�[�����Ŏg�p����S�Ă̌��ʉ���BGM��ǂݍ���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void AudioManager::Initialize()
{
	// nlohmann::json�̃G�C���A�X���`
	using json = nlohmann::json;
	// �W�����O��Ԃ��g�p
	using namespace std;
	// FMOD�V�X�e���̍쐬
	FMOD_RESULT result = FMOD::System_Create(&m_pFMODSystem);
	// �G���[����
	if (result != FMOD_OK || !m_pFMODSystem)
	{
		// �V�X�e���̏������Ɏ��s
		m_pFMODSystem = nullptr;
		// �I��
		return;
	}
	// FMOD�V�X�e���̏�����
	result = m_pFMODSystem->init(512, FMOD_INIT_NORMAL, nullptr);
	// �G���[����
	if (result != FMOD_OK)
	{
		// �V�X�e���̏������Ɏ��s
		m_pFMODSystem = nullptr;
		// �I��
		return;
	}
	//�ǂݍ��ރt�@�C���̖��O���쐬
	string filename = "Resources/Jsons/Audios.json";
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
	//// "sounds"�z����̊e�T�E���h����������
	//for (const auto& sound : j["sounds"])
	//{
	//	// �t�@�C���p�X
	//	std::string filePath = sound["file"];
	//	// �L�[
	//	std::string key = sound["key"];
	//	// ���[�v�t���O
	//	bool isLoop = sound["loop"];
	//	// �e����ʉ��EBGM�̓ǂݍ���
	//	LoadSound(filePath, key, isLoop);
	//}
}
/*
*	@brief �����Đ�����
*	@details �����f�[�^���Đ�����(BGM�͓�d�Đ������Ȃ��j
*	@param soundKey �Đ����鉹���f�[�^�̃L�[
*	@param volume ����
*	@return �Ȃ�
*/
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{

	// �w�肳�ꂽ�����f�[�^�̃L�[������
	auto soundIt = m_pSounds.find(soundKey);
	// �����f�[�^�����������ꍇ
	if (soundIt != m_pSounds.end())
	{
		// �����f�[�^�̎擾
		FMOD::Sound* sound = soundIt->second;
		// �����f�[�^�����������ꍇ
		// ��d�Đ��ۂ̃t���O���擾
		auto allowIt = m_pAllowMultiplePlayMap.find(soundKey);
		// ������Ȃ���΃f�t�H���g�� false
		bool allowMultiple = (allowIt != m_pAllowMultiplePlayMap.end()) ? allowIt->second : false;

		// ��d�Đ��������Ă��Ȃ��ꍇ
		if (!allowMultiple)
		{
			// �`�����l��������
			auto channelIt = m_pChannels.find(soundKey);
			// �`�����l�������������ꍇ
			if (channelIt != m_pChannels.end())
			{
				// �`�����l���̎擾
				FMOD::Channel* existingChannel = channelIt->second;
				// �Đ������ǂ�����ێ�����ϐ�
				bool isPlaying = false;
				// �Đ������m�F
				existingChannel->isPlaying(&isPlaying);
				// �����Đ����Ȃ�
				if (isPlaying)
				{
					// ���ʂ����X�V
					existingChannel->setVolume(volume);
					// �Đ����Ȃ��i�d���Đ��h�~�j
					return;
				}
			}
		}
		// �����Đ�����
		// �`�����l����錾
		FMOD::Channel* channel = nullptr;
		// �����f�[�^���Đ�
		m_pFMODSystem->playSound(sound, nullptr, false, &channel);
		// �`�����l����ۑ�����
		m_pChannels[soundKey] = channel;
		// �`�����l�������݂���ꍇ�A���ʂ�ݒ肷��
		if (channel)channel->setVolume(volume);
	}
}
/*
*	@brief �����V�X�e���̍X�V
*	@details FMOD�V�X�e���̍X�V���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void AudioManager::Update() { m_pFMODSystem->update(); }

/*
*	@brief �����֘A���\�[�X�̉������
*	@details FMOD�V�X�e���̉�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void AudioManager::Shutdown()
{
	// m_pFMODSystem �� null �Ȃ������Ȃ�
	if (!m_pFMODSystem) return;
	// �`�����l���̒�~�Ɖ��
	for (auto& pair : m_pChannels)
	{
		// �`�����l�������݂���ꍇ
		if (pair.second)
		{
			// �`�����l�����~
			pair.second->stop();
			// �`�����l�������
			pair.second = nullptr;
		}
	}
	// �`�����l�����N���A
	m_pChannels.clear();
	// �����n�b�V�������
	m_pChannels.rehash(0);
	// ���ׂẴT�E���h�����
	for (auto& pair : m_pSounds)
	{
		// �T�E���h�����݂���ꍇ
		if (pair.second)
		{
			// �T�E���h���~
			pair.second->release();
			// �T�E���h�����
			pair.second = nullptr;
		}
	}
	// �T�E���h���N���A
	m_pSounds.clear();

	// FMOD�V�X�e�������݂���ꍇ
	if (m_pFMODSystem)
	{
		// �ŏI�X�V
		m_pFMODSystem->update();
		// FMOD�V�X�e���̉��
		m_pFMODSystem->release();
		// �V�X�e���� null �ɐݒ�
		m_pFMODSystem = nullptr;
		// ���S�̂��߁A�����ҋ@
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
/*
*	@brief �����f�[�^�̃��[�h
*	@details �����f�[�^�����[�h����(���[�h�ς݂Ȃ牽�����Ȃ�)
*	@param filePath ���[�h���鉹���t�@�C���̃p�X
*	@param key �����f�[�^�̃L�[
*	@param allowMultiplePlay ��d�Đ��������邩�ǂ���
*	@return ���������ꍇ�� true�A���s�����ꍇ�� false
*/
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay)
{
	// ���Ƀ��[�h�ς݂Ȃ�I��
	if (m_pSounds.find(key) != m_pSounds.end()) return false;
	// �����f�[�^��錾
	FMOD::Sound* sound = nullptr;
	// �����f�[�^�̍쐬
	FMOD_RESULT result = m_pFMODSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	// �G���[����
	if (result != FMOD_OK || !sound) return false;
	// �����f�[�^��ۑ�
	m_pSounds[key] = sound;
	// ��d�Đ��̉ۂ�ۑ�
	m_pAllowMultiplePlayMap[key] = allowMultiplePlay;
	// �����܂ł��ꂽ�琬��
	return true;
}
/*
*	@brief �����f�[�^�̎擾
*	@details �����f�[�^���擾����
*	@param key �����f�[�^�̃L�[
*	@return �����f�[�^�̃|�C���^(nullptr�̏ꍇ�͎擾���s)
*/
FMOD::Sound* AudioManager::GetSound(const std::string& key)
{
	// �w�肳�ꂽ�L�[������
	auto it = m_pSounds.find(key);
	// �����f�[�^��������Ȃ��ꍇ�� nullptr ��Ԃ�
	return (it != m_pSounds.end()) ? it->second : nullptr;
}

/*
*	@brief �����~����
*	@details �w�肳�ꂽ�����f�[�^���~����
*	@param soundKey ��~���鉹���f�[�^�̃L�[
*	@return �Ȃ�
*/
void AudioManager::StopSound(const std::string& soundKey)
{
	// �w�肳�ꂽ�����f�[�^�̃L�[������
	auto channelIt = m_pChannels.find(soundKey);
	// �����f�[�^�����������ꍇ
	if (channelIt != m_pChannels.end())
	{
		// �`�����l�����擾
		FMOD::Channel* channel = channelIt->second;
		// �`�����l�����~
		channel->stop();
	}
}