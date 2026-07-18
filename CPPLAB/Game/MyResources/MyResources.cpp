#include "pch.h"
#include "MyResources.h"

void MyResources::Initialize(DX::StepTimer* timer,
							 DX::DeviceResources* dr,
							 DirectX::CommonStates* commonStates,
							 mylib::DebugString* debugString,
							 mylib::InputManager* inputManager,
							 AudioManager* audioManager,
							 ModelManager* modelManager,
							 TextureManager* textureManager,
							 SettingManager* settingManager)

{
	assert(timer);			// タイマーがnullptrでないことを確認
	assert(dr);				// デバイスリソースがnullptrでないことを確認
	assert(commonStates);	// コモンステートがnullptrでないことを確認
	assert(debugString);	// デバッグ文字列がnullptrでないことを確認
	assert(inputManager);	// 入力マネージャがnullptrでないことを確認
	assert(audioManager);	// オーディオマネージャがnullptrでないことを確認
	assert(modelManager);	// モデルマネージャがnullptrでないことを確認
	assert(textureManager);	// テクスチャマネージャがnullptrでないことを確認
	assert(settingManager);// 設定マネージャがnullptrでないことを確認

	m_stepTimer = timer;			// タイマーを設定する
	m_deviceResources = dr;			// デバイスリソースを設定する
	m_commonStates = commonStates;	// コモンステートを設定する
	m_debugString = debugString;	// デバッグ文字列を設定する
	m_inputManager = inputManager;	// 入力マネージャを設定する
	m_audioManager = audioManager;	// オーディオマネージャを設定する
	m_modelManager = modelManager;	// モデルマネージャを設定する
	m_textureManager = textureManager; // テクスチャマネージャを設定する
	m_settingManager = settingManager; // 設定マネージャを設定する
}
