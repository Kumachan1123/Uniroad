/*
*	@file	AudioManager.h
*	@brief	音声管理クラス
*/
#pragma once
#ifndef AUDIOMANAGER_DEFINED
#define AUDIOMANAGER_DEFINED
// 標準ライブラリ
#include <string>
#include <fstream>
#include <unordered_map>
#include <thread> 
#include <memory>
// FMOD(外部ライブラリ)
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

// 前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

// 音声管理クラス
class AudioManager
{
public:
	// public関数
	// コンストラクタ
	AudioManager();
	// デストラクタ
	~AudioManager();
	// 初期化
	void Initialize();
	// 音声データのロード
	bool LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay);
	// 音声データの取得
	FMOD::Sound* GetSound(const std::string& key);
	// 音を再生する
	void PlaySound(const std::string& soundKey, float volume);
	// 音を停止する
	void StopSound(const std::string& soundKey);
	// 更新（FMODシステムの更新が必要）
	void Update();
	// 解放
	void Shutdown();
private:
	// private変数
	// FMODシステム
	FMOD::System* m_pFMODSystem;
	// チャンネル
	std::unordered_map<std::string, FMOD::Channel*> m_pChannels;
	// サウンド
	std::unordered_map<std::string, FMOD::Sound*> m_pSounds;
	// キーごとの二重再生可否
	std::unordered_map<std::string, bool> m_pAllowMultiplePlayMap;
	// ボリューム
	float m_volume;
};
#endif // AUDIOMANAGER_DEFINED