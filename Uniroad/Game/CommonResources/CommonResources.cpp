/*
*	@file	CommonResources.cpp
*	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#include <pch.h>
#include "CommonResources.h"
/*
*	@brief	コンストラクタ
*	@details 初期化時にnullptrを設定する
*	@param	なし
*	@return なし
*/
CommonResources::CommonResources()
	: m_stepTimer{ nullptr }		// タイマー
	, m_deviceResources{ nullptr }	// デバイスリソース
	, m_commonStates{ nullptr }		// コモンステート
	, m_debugString{ nullptr }		// デバッグ文字列
	, m_inputManager{ nullptr }		// 入力マネージャ
	, m_audioManager{ nullptr }		// オーディオマネージャ
	, m_modelManager{ nullptr }		// モデルマネージャ
	, m_textureManager{ nullptr }	// テクスチャマネージャ
{
}
/*
*	@brief	初期化する
*	@details main.cppで作成したリソースを受け取る
*	@param	timer			受け渡しするタイマー
*	@param	dr				受け渡しするデバイスリソース
*	@param	commonStates	受け渡しするコモンステート
*	@param	debugString		受け渡しするデバッグ文字列
*	@param	inputManager	受け渡しする入力マネージャ
*	@param	audioManager	受け渡しするオーディオマネージャ
*	@param	modelManager	受け渡しするモデルマネージャ
*	@param	textureManager	受け渡しするテクスチャマネージャ
*	@return	なし
*/
void CommonResources::Initialize(
	DX::StepTimer* timer,
	DX::DeviceResources* dr,
	DirectX::CommonStates* commonStates,
	mylib::DebugString* debugString,
	mylib::InputManager* inputManager,
	AudioManager* audioManager,
	ModelManager* modelManager,
	TextureManager* textureManager
)
{
	assert(timer);			// タイマーがnullptrでないことを確認
	assert(dr);				// デバイスリソースがnullptrでないことを確認
	assert(commonStates);	// コモンステートがnullptrでないことを確認
	assert(debugString);	// デバッグ文字列がnullptrでないことを確認
	assert(inputManager);	// 入力マネージャがnullptrでないことを確認
	assert(audioManager);	// オーディオマネージャがnullptrでないことを確認
	assert(modelManager);	// モデルマネージャがnullptrでないことを確認
	assert(textureManager);	// テクスチャマネージャがnullptrでないことを確認

	m_stepTimer = timer;			// タイマーを設定する
	m_deviceResources = dr;			// デバイスリソースを設定する
	m_commonStates = commonStates;	// コモンステートを設定する
	m_debugString = debugString;	// デバッグ文字列を設定する
	m_inputManager = inputManager;	// 入力マネージャを設定する
	m_audioManager = audioManager;	// オーディオマネージャを設定する
	m_modelManager = modelManager;	// モデルマネージャを設定する
	m_textureManager = textureManager; // テクスチャマネージャを設定する
}
