#include "pch.h"
#include "AudioManager.h"

// シングルトンインスタンスの初期化
std::unique_ptr<AudioManager> AudioManager::m_instance = nullptr;

// インスタンスを取得する
AudioManager* const AudioManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new AudioManager());
		m_instance->Initialize();
	}
	return m_instance.get();
}

// コンストラクタ
AudioManager::AudioManager()
	:
	m_system(nullptr),
	m_sounds(),
	m_channels()
{
}

// デストラクタ
AudioManager::~AudioManager()
{
	Shutdown();
}

// FMODシステムの初期化
void AudioManager::Initialize()
{
	FMOD::System_Create(&m_system);
	m_system->init(512, FMOD_INIT_NORMAL, nullptr);
}


// 音を再生する(音声の種類key->"BGM","SE") (音量指定:min = 0,max = 1)
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{
	auto soundIt = m_sounds.find(soundKey);
	if (soundIt != m_sounds.end())
	{
		FMOD::Sound* sound = soundIt->second;

		// サウンドキーに「BGM」が含まれている場合、二重再生を防ぐ
		if (soundKey.find("BGM") != std::string::npos)
		{
			auto channelIt = m_channels.find(soundKey);
			if (channelIt != m_channels.end())
			{
				FMOD::Channel* existingChannel = channelIt->second;
				bool isPlaying = false;
				existingChannel->isPlaying(&isPlaying);
				if (isPlaying)
				{
					existingChannel->setVolume(volume);
					return; // 既に再生中のため、何もしない
				}
			}
		}


		// 音を再生する
		FMOD::Channel* channel = nullptr;
		m_system->playSound(sound, nullptr, false, &channel);

		// チャンネルを保存する
		m_channels[soundKey] = channel;

		// 音量を設定する
		if (channel)
		{
			channel->setVolume(volume);
		}
	}
}

// FMODシステムの更新
void AudioManager::Update()
{
	m_system->update();
}

// FMODシステムの解放
void AudioManager::Shutdown()
{
	for (auto& pair : m_sounds)
	{
		if (pair.second)
		{
			pair.second->release();
		}
	}
	m_sounds.clear();

	if (m_system)
	{
		m_system->close();
		m_system->release();
		m_system = nullptr;
	}
}

// 音声ファイルを読み込む
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key) {
	if (m_sounds.find(key) != m_sounds.end()) {
		// 既にロード済み
		return false;
	}

	FMOD::Sound* sound = nullptr;
	FMOD_RESULT result = m_system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK || !sound) {
		return false;
	}

	m_sounds[key] = sound;
	return true;
}

// 音声ファイルを取得する
FMOD::Sound* AudioManager::GetSound(const std::string& key) {
	auto it = m_sounds.find(key);
	return (it != m_sounds.end()) ? it->second : nullptr;
}

// 音を停止する
void AudioManager::StopSound(const std::string& soundKey)
{
	auto channelIt = m_channels.find(soundKey);
	if (channelIt != m_channels.end())
	{
		FMOD::Channel* channel = channelIt->second;
		channel->stop();
	}
}