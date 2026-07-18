/*
*	@file	SettingManager.cpp
*	@brief	設定管理クラスの実装ファイル
*/
#include "pch.h"
#include "SettingManager.h"
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"
// 標準ライブラリ
#include <fstream>
#include <unordered_map>
#include <thread> 
#include <memory>
#include <string>
// ファイル名
const std::string SettingManager::FILE_NAME = "Resources/Jsons/Setting.json";
/*
*	@brief コンストラクタ
*	@details 設定管理クラスのコンストラクタ
*	@param なし
*	@return なし
*/
SettingManager::SettingManager()
	: m_bgmVolume(1.0f) // BGM音量初期値
	, m_seVolume(1.0f) // SE音量初期値
	, m_speedMode(1.0f) // 倍速モード初期値
{}
/*
*	@brief デストラクタ
*	@details 設定管理クラスのデストラクタ
*	@param なし
*	@return なし
*/
SettingManager::~SettingManager()
{
	// 何もしない
}
/*
*	@brief 初期化
*	@details ゲーム起動時に設定ファイルを読み込み、設定を各変数に保存する
*	@param なし
*	@return なし
**/
void SettingManager::Initialize()
{
	// nlohmann::jsonのエイリアスを定義
	using json = nlohmann::json;
	// 標準名前空間を使用
	using namespace std;
	//ファイルを開く
	ifstream ifs(FILE_NAME.c_str());
	// ファイルが正常に開けなかったら強制終了
	if (!ifs.good())return;
	//jsonオブジェクト
	json j;
	//ファイルから読み込む
	ifs >> j;
	//ファイルを閉じる
	ifs.close();
	// BGM音量
	m_bgmVolume = j["BGM"];
	// SE音量
	m_seVolume = j["SE"];
	// 倍速モードの倍率
	m_speedMode = j["SpeedMode"];
}
/*
*	@brief 設定書き込み
*	@details 設定をJSON形式でファイルに保存する
*	@param なし
*	@return なし
*/
void SettingManager::SaveSettings()
{
	// nlohmann::jsonのエイリアスを定義
	using json = nlohmann::json;
	// 標準名前空間を使用
	using namespace std;
	// ファイルに書き込む
	ofstream ofs(FILE_NAME.c_str());
	// ファイルが正常に開けたら
	if (ofs.good())
	{
		// jsonオブジェクトを作成
		json j;
		// BGM音量
		j["BGM"] = m_bgmVolume;
		// SE音量
		j["SE"] = m_seVolume;
		// 倍速モードの倍率
		j["SpeedMode"] = m_speedMode;
		//ファイルに書き込む
		ofs << j;
		//ファイルを閉じる
		ofs.close();
	}
}
