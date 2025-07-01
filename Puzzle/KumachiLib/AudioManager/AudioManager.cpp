/*
*	@file AudioManager.cpp
*	@brief 音声管理クラス
*/
#include <pch.h>
#include "AudioManager.h"
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief コンストラクタ
*	@details FMODシステムの初期化を行い、ゲーム内で使用する全ての効果音とBGMを読み込む
*	@param なし
*	@retrun なし
*/
AudioManager::AudioManager()
	: m_pFMODSystem(nullptr)// FMODシステム 
	, m_pSounds()// サウンド 
	, m_pChannels()// チャンネル 
{
	Initialize();// 初期化
}
/*
*	@brief デストラクタ
*	@details 音声関連リソースの解放処理を行う
*	@param なし
*	@retrun なし
*/
AudioManager::~AudioManager() { Shutdown(); }// FMODシャットダウン

/*
*	@brief 初期化
*	@details システム作成および初期化を行い、
*			 ゲーム内で使用する全ての効果音とBGMを読み込む
*	@param なし
*	@return なし
*/
void AudioManager::Initialize()
{
	// nlohmann::jsonのエイリアスを定義
	using json = nlohmann::json;
	// 標準名前空間を使用
	using namespace std;
	// FMODシステムの作成
	FMOD_RESULT result = FMOD::System_Create(&m_pFMODSystem);
	// エラー処理
	if (result != FMOD_OK || !m_pFMODSystem)
	{
		// システムの初期化に失敗
		m_pFMODSystem = nullptr;
		// 終了
		return;
	}
	// FMODシステムの初期化
	result = m_pFMODSystem->init(512, FMOD_INIT_NORMAL, nullptr);
	// エラー処理
	if (result != FMOD_OK)
	{
		// システムの初期化に失敗
		m_pFMODSystem = nullptr;
		// 終了
		return;
	}
	//読み込むファイルの名前を作成
	string filename = "Resources/Jsons/Audios.json";
	//ファイルを開く
	ifstream ifs(filename.c_str());
	// ファイルが正常に開けなかったら強制終了
	if (!ifs.good())return;
	//jsonオブジェクト
	json j;
	//ファイルから読み込む
	ifs >> j;
	//ファイルを閉じる
	ifs.close();
	//// "sounds"配列内の各サウンドを処理する
	//for (const auto& sound : j["sounds"])
	//{
	//	// ファイルパス
	//	std::string filePath = sound["file"];
	//	// キー
	//	std::string key = sound["key"];
	//	// ループフラグ
	//	bool isLoop = sound["loop"];
	//	// 各種効果音・BGMの読み込み
	//	LoadSound(filePath, key, isLoop);
	//}
}
/*
*	@brief 音を再生する
*	@details 音声データを再生する(BGMは二重再生させない）
*	@param soundKey 再生する音声データのキー
*	@param volume 音量
*	@return なし
*/
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{

	// 指定された音声データのキーを検索
	auto soundIt = m_pSounds.find(soundKey);
	// 音声データが見つかった場合
	if (soundIt != m_pSounds.end())
	{
		// 音声データの取得
		FMOD::Sound* sound = soundIt->second;
		// 音声データが見つかった場合
		// 二重再生可否のフラグを取得
		auto allowIt = m_pAllowMultiplePlayMap.find(soundKey);
		// 見つからなければデフォルトで false
		bool allowMultiple = (allowIt != m_pAllowMultiplePlayMap.end()) ? allowIt->second : false;

		// 二重再生を許可していない場合
		if (!allowMultiple)
		{
			// チャンネルを検索
			auto channelIt = m_pChannels.find(soundKey);
			// チャンネルが見つかった場合
			if (channelIt != m_pChannels.end())
			{
				// チャンネルの取得
				FMOD::Channel* existingChannel = channelIt->second;
				// 再生中かどうかを保持する変数
				bool isPlaying = false;
				// 再生中か確認
				existingChannel->isPlaying(&isPlaying);
				// もし再生中なら
				if (isPlaying)
				{
					// 音量だけ更新
					existingChannel->setVolume(volume);
					// 再生しない（重複再生防止）
					return;
				}
			}
		}
		// 音を再生する
		// チャンネルを宣言
		FMOD::Channel* channel = nullptr;
		// 音声データを再生
		m_pFMODSystem->playSound(sound, nullptr, false, &channel);
		// チャンネルを保存する
		m_pChannels[soundKey] = channel;
		// チャンネルが存在する場合、音量を設定する
		if (channel)channel->setVolume(volume);
	}
}
/*
*	@brief 音声システムの更新
*	@details FMODシステムの更新を行う
*	@param なし
*	@return なし
*/
void AudioManager::Update() { m_pFMODSystem->update(); }

/*
*	@brief 音声関連リソースの解放処理
*	@details FMODシステムの解放を行う
*	@param なし
*	@return なし
*/
void AudioManager::Shutdown()
{
	// m_pFMODSystem が null なら解放しない
	if (!m_pFMODSystem) return;
	// チャンネルの停止と解放
	for (auto& pair : m_pChannels)
	{
		// チャンネルが存在する場合
		if (pair.second)
		{
			// チャンネルを停止
			pair.second->stop();
			// チャンネルを解放
			pair.second = nullptr;
		}
	}
	// チャンネルをクリア
	m_pChannels.clear();
	// 内部ハッシュを解放
	m_pChannels.rehash(0);
	// すべてのサウンドを解放
	for (auto& pair : m_pSounds)
	{
		// サウンドが存在する場合
		if (pair.second)
		{
			// サウンドを停止
			pair.second->release();
			// サウンドを解放
			pair.second = nullptr;
		}
	}
	// サウンドをクリア
	m_pSounds.clear();

	// FMODシステムが存在する場合
	if (m_pFMODSystem)
	{
		// 最終更新
		m_pFMODSystem->update();
		// FMODシステムの解放
		m_pFMODSystem->release();
		// システムを null に設定
		m_pFMODSystem = nullptr;
		// 安全のため、少し待機
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
/*
*	@brief 音声データのロード
*	@details 音声データをロードする(ロード済みなら何もしない)
*	@param filePath ロードする音声ファイルのパス
*	@param key 音声データのキー
*	@param allowMultiplePlay 二重再生を許可するかどうか
*	@return 成功した場合は true、失敗した場合は false
*/
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay)
{
	// 既にロード済みなら終了
	if (m_pSounds.find(key) != m_pSounds.end()) return false;
	// 音声データを宣言
	FMOD::Sound* sound = nullptr;
	// 音声データの作成
	FMOD_RESULT result = m_pFMODSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	// エラー処理
	if (result != FMOD_OK || !sound) return false;
	// 音声データを保存
	m_pSounds[key] = sound;
	// 二重再生の可否を保存
	m_pAllowMultiplePlayMap[key] = allowMultiplePlay;
	// ここまでこれたら成功
	return true;
}
/*
*	@brief 音声データの取得
*	@details 音声データを取得する
*	@param key 音声データのキー
*	@return 音声データのポインタ(nullptrの場合は取得失敗)
*/
FMOD::Sound* AudioManager::GetSound(const std::string& key)
{
	// 指定されたキーを検索
	auto it = m_pSounds.find(key);
	// 音声データが見つからない場合は nullptr を返す
	return (it != m_pSounds.end()) ? it->second : nullptr;
}

/*
*	@brief 音を停止する
*	@details 指定された音声データを停止する
*	@param soundKey 停止する音声データのキー
*	@return なし
*/
void AudioManager::StopSound(const std::string& soundKey)
{
	// 指定された音声データのキーを検索
	auto channelIt = m_pChannels.find(soundKey);
	// 音声データが見つかった場合
	if (channelIt != m_pChannels.end())
	{
		// チャンネルを取得
		FMOD::Channel* channel = channelIt->second;
		// チャンネルを停止
		channel->stop();
	}
}