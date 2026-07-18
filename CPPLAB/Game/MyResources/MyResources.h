#pragma once
#include <KumachiLib\Singleton\Singleton.h>
// 標準ライブラリ
#include <cassert>
// 自作ヘッダーファイル
#include "KumachiLib/AudioManager/AudioManager.h"
#include "KumachiLib/ModelManager/ModelManager.h"
#include "KumachiLib/TextureManager/TextureManager.h"
#include "KumachiLib/SettingManager/SettingManager.h"

// 前方宣言
namespace DX
{
	class StepTimer;
	class DeviceResources;
}
namespace mylib
{
	class DebugString;
	class InputManager;
}
class AudioManager;
class ModelManager;
class TextureManager;
class SettingManager;
class MyResources : public Singleton<MyResources>
{
	friend class Singleton<MyResources>;
public:
	// アクセサ
	// タイマーを取得する
	DX::StepTimer* GetStepTimer() const { return m_stepTimer; }
	// デバイスリソースを取得する
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }
	// コモンステートを取得する
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }
	// デバッグ文字列を取得する
	mylib::DebugString* GetDebugString() const { return m_debugString; }
	// 入力マネージャを取得する
	mylib::InputManager* GetInputManager() const { return m_inputManager; }
	// オーディオマネージャを取得する
	AudioManager* GetAudioManager() const { return m_audioManager; }
	// モデルマネージャを取得する
	ModelManager* GetModelManager() const { return m_modelManager; }
	// テクスチャマネージャを取得する
	TextureManager* GetTextureManager() const { return m_textureManager; }
	// 設定マネージャを取得する
	SettingManager* GetSettingManager() const { return m_settingManager; }

	// 初期化する
	void Initialize(
		DX::StepTimer* timer,// 受け渡しするタイマー
		DX::DeviceResources* dr,// 受け渡しするデバイスリソース
		DirectX::CommonStates* commonStates,// 受け渡しするコモンステート
		mylib::DebugString* debugString,// 受け渡しするデバッグ文字列
		mylib::InputManager* inputManager,// 受け渡しする入力マネージャ
		AudioManager* audioManager,// 受け渡しするオーディオマネージャ
		ModelManager* modelManager,// 受け渡しするモデルマネージャ
		TextureManager* textureManager,  // 受け渡しするテクスチャマネージャ
		SettingManager* settingManager  // 受け渡しする設定マネージャ
	);

private:
	// publicメソッド
	// コンストラクタ
	MyResources() = default;
	// デストラクタ defaultで定義
	~MyResources() = default;
private:
	// privateメンバ変数
	// 受け渡しするリソース一覧
	// タイマー
	DX::StepTimer* m_stepTimer;
	// デバイスリソース
	DX::DeviceResources* m_deviceResources;
	// コモンステート
	DirectX::CommonStates* m_commonStates;
	// デバッグ文字列
	mylib::DebugString* m_debugString;
	// 入力マネージャ
	mylib::InputManager* m_inputManager;
	// オーディオマネージャ
	AudioManager* m_audioManager;
	// モデルマネージャ
	ModelManager* m_modelManager;
	// テクスチャマネージャ
	TextureManager* m_textureManager;
	// 設定マネージャ
	SettingManager* m_settingManager;
};