/*
*	@file Game.h
*	@brief ゲームのメインクラス
*/
#pragma once
#ifndef GAME_DEFINED
#define GAME_DEFINED
// 標準ライブラリ
#include <thread>
// DirectX
#include <DeviceResources.h>
#include "StepTimer.h"
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/nlohmann/json.hpp>
// ゲーム関連
#include "Game/CommonResources/CommonResources.h"
#include "Game/SceneManager/IScene.h"
#include "Game/SceneManager/SceneManager.h"
#include "KumachiLib/AudioManager/AudioManager.h"
#include "KumachiLib/ModelManager/ModelManager.h"
#include "KumachiLib/TextureManager/TextureManager.h"
#include "Game/Screen/Screen.h"

// ゲームクラス
class Game final : public DX::IDeviceNotify
{
public:
	// public関数
	// コンストラクタ
	Game() noexcept(false);
	// デストラクタ
	~Game() = default;

	// ムーブコンストラクタ
	Game(Game&&) = default;
	// ムーブ代入演算子
	Game& operator= (Game&&) = default;

	// コピーコンストラクタ
	Game(Game const&) = delete;
	// コピー代入演算子
	Game& operator= (Game const&) = delete;

	// 初期化
	void Initialize(HWND window, int width, int height);
	// ゲームループを実行
	void Tick();
	// デバイスが失われたときの処理
	void OnDeviceLost() override;
	// デバイスが復元されたときの処理
	void OnDeviceRestored() override;
	// メッセージ
	// アクティブ化されたときの処理
	void OnActivated();
	// 非アクティブ化されたときの処理
	void OnDeactivated();
	// 一時停止されたときの処理
	void OnSuspending();
	// 再開されたときの処理
	void OnResuming();
	// ウィンドウが移動されたときの処理
	void OnWindowMoved();
	// ディスプレイの変更があったときの処理
	void OnDisplayChange();
	// ウィンドウサイズが変更されたときの処理
	void OnWindowSizeChanged(int width, int height);
	// デフォルトのウィンドウサイズを取得
	void GetDefaultSize(int& width, int& height) const noexcept;
	// フルスクリーン状態を設定
	void SetFullscreenState(BOOL value);
private:
	// private関数
	// 更新処理
	void Update(DX::StepTimer const& timer);
	// 描画処理
	void Render();
	// 画面をクリア
	void Clear();
	// デバイス依存リソースの作成
	void CreateDeviceDependentResources();
	// ウィンドウサイズ依存リソースの作成
	void CreateWindowSizeDependentResources();
private:
	// private変数
	// デバイスリソース
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;
	// タイマー
	DX::StepTimer                           m_timer;
	// フルスクリーン状態
	BOOL m_fullscreen;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>  m_commonStates;
	// コモンリソース
	std::unique_ptr<CommonResources>        m_pCommonResources;
	// デバッグ文字列
	std::unique_ptr<mylib::DebugString>     m_debugString;
	// 入力マネージャ
	std::unique_ptr<mylib::InputManager>    m_inputManager;
	// シーンマネージャ
	std::unique_ptr<SceneManager>           m_sceneManager;
	// オーディオマネージャ
	std::unique_ptr<AudioManager>           m_audioManager;
	// モデルマネージャ
	std::unique_ptr<ModelManager>           m_modelManager;
	// テクスチャマネージャ
	std::unique_ptr<TextureManager>         m_textureManager;
};
#endif // GAME_DEFINED
