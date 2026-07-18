/*
*	@file 	SettingManager.h
*	@brief	設定管理クラスのヘッダーファイル
*/
#pragma once



// 設定管理クラス
class SettingManager
{
public:
	// アクセサ
	// プレイヤーが調整可能なパラメーター
	// BGM音量取得
	float GetBGMVolume() const { return m_bgmVolume; }
	// BGM音量設定
	void SetBGMVolume(float volume) { m_bgmVolume = volume; }
	// SE音量取得
	float GetSEVolume() const { return m_seVolume; }
	// SE音量設定
	void SetSEVolume(float volume) { m_seVolume = volume; }
	// プレイヤーが調整できないパラメーター
	// 倍速モードの倍率取得
	float GetSpeedMode() const { return m_speedMode; }
	// 倍速モードの倍率設定
	void SetSpeedMode(float speed) { m_speedMode = speed; }
public:
	// public関数
	// コンストラクタ
	SettingManager();
	// デストラクタ
	~SettingManager();
	// 初期化
	void Initialize();
	// 設定書き込み
	void SaveSettings();
private:
	// 定数
	static const std::string  FILE_NAME;
private:
	// private変数
	// プレイヤーが調整できるパラメーター
	// BGM音量
	float m_bgmVolume;
	// SE音量
	float m_seVolume;
	// プレイヤーが調整できないパラメーター
	// 倍速モードの倍率
	float m_speedMode;
};