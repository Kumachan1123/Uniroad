/*
*	@file	AudioManager.h
*	@brief	�����Ǘ��N���X
*/
#pragma once
#ifndef AUDIOMANAGER_DEFINED
#define AUDIOMANAGER_DEFINED
// �W�����C�u����
#include <string>
#include <fstream>
#include <unordered_map>
#include <thread> 
#include <memory>
// FMOD(�O�����C�u����)
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

// �O���錾
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

// �����Ǘ��N���X
class AudioManager
{
public:
	// public�֐�
	// �R���X�g���N�^
	AudioManager();
	// �f�X�g���N�^
	~AudioManager();
	// ������
	void Initialize();
	// �����f�[�^�̃��[�h
	bool LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay);
	// �����f�[�^�̎擾
	FMOD::Sound* GetSound(const std::string& key);
	// �����Đ�����
	void PlaySound(const std::string& soundKey, float volume);
	// �����~����
	void StopSound(const std::string& soundKey);
	// �X�V�iFMOD�V�X�e���̍X�V���K�v�j
	void Update();
	// ���
	void Shutdown();
private:
	// private�ϐ�
	// FMOD�V�X�e��
	FMOD::System* m_pFMODSystem;
	// �`�����l��
	std::unordered_map<std::string, FMOD::Channel*> m_pChannels;
	// �T�E���h
	std::unordered_map<std::string, FMOD::Sound*> m_pSounds;
	// �L�[���Ƃ̓�d�Đ���
	std::unordered_map<std::string, bool> m_pAllowMultiplePlayMap;
	// �{�����[��
	float m_volume;
};
#endif // AUDIOMANAGER_DEFINED