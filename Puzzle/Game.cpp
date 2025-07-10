/*
*	@file Game.cpp
*	@brief ゲームのメインクラス
*/
#include <pch.h>
#include "Game.h"
/*
*	@brief ゲームを終了する
*	@detail ゲームを終了する関数(Main.cppから呼び出される)
*	@param なし
*	@return なし
*/
extern void ExitGame() noexcept;



using Microsoft::WRL::ComPtr;
/*
*	@brief コンストラクタ
*	@detail ゲームのメインクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Game::Game() noexcept(false)
	: m_deviceResources{}// デバイスリソース
	, m_timer{}// タイマー
	, m_commonStates{}// コモンステート
	, m_pCommonResources{}// 共通リソース
	, m_debugString{}// デバッグ文字列
	, m_inputManager{}// 入力マネージャ
	, m_sceneManager{}// シーンマネージャ
	, m_audioManager{}// オーディオマネージャ
	, m_modelManager{}// モデルマネージャ
	, m_textureManager{}// テクスチャマネージャ
	, m_fullscreen{ FALSE }// フルスクリーン状態
{
	// デバイスリソースを作成する
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	// デバイス通知を登録する
	m_deviceResources->RegisterDeviceNotify(this);
}


/*
*	@brief ゲームの初期化
*	@detail ゲームの実行に必要なDirect3Dリソースを初期化
*	@param window ウィンドウハンドル
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Game::Initialize(HWND window, int width, int height)
{
	using namespace DirectX;
	// デバイスリソース関連を設定する
	// ウィンドウを設定する
	m_deviceResources->SetWindow(window, width, height);
	// デバイスリソースを作成する
	m_deviceResources->CreateDeviceResources();
	// デバイス依存リソースを作成する
	CreateDeviceDependentResources();
	// ウィンドウサイズ依存リソースを作成する
	m_deviceResources->CreateWindowSizeDependentResources();
	// ウィンドウサイズ依存リソースを作成する
	CreateWindowSizeDependentResources();
	// デバイスを取得する
	auto device = m_deviceResources->GetD3DDevice();
	// デバイスコンテキストを取得する
	auto context = m_deviceResources->GetD3DDeviceContext();
	// 入力マネージャを作成する
	m_inputManager = std::make_unique<mylib::InputManager>(window);
	// コモンステートを作成する
	m_commonStates = std::make_unique<CommonStates>(device);
	// デバッグ文字列を作成する
	m_debugString = std::make_unique<mylib::DebugString>(
		device,		// デバイス
		context,	// デバイスコンテキスト
		L"Resources/Fonts/SegoeUI_18.spritefont"// フォントファイルのパス
	);
	// オーディオマネージャーを作成する
	m_audioManager = std::make_unique<AudioManager>();
	// 共通リソースを作成する
	m_pCommonResources = std::make_unique<CommonResources>();
	// モデルマネージャを作成する
	m_modelManager = std::make_unique<ModelManager>();
	// テクスチャマネージャを作成する
	m_textureManager = std::make_unique<TextureManager>();
	//// モデルマネージャの初期化を別スレッドで行う
	//std::thread modelInitializeThread([this, device]()
	//	{
	//		// モデルマネージャを初期化する
	//		m_modelManager->Initialize(m_deviceResources->GetD3DDevice());
	//	});
	//// テクスチャマネージャの初期化を別スレッドで行う
	//std::thread textureInitializeThread([this, device]()
	//	{
	//		// テクスチャマネージャを初期化する
	//		m_textureManager->Initialize(m_deviceResources->GetD3DDevice());
	//	});
	//// モデルマネージャの初期化が完了するまで待機する
	//modelInitializeThread.join();
	//// テクスチャマネージャの初期化が完了するまで待機する
	//textureInitializeThread.join();
	m_modelManager->Initialize(m_deviceResources->GetD3DDevice());// モデルマネージャを初期化する
	m_textureManager->Initialize(m_deviceResources->GetD3DDevice());// テクスチャマネージャを初期化する
	// シーンへ渡す共通リソースを設定する
	m_pCommonResources->Initialize(
		&m_timer,				// タイマー
		m_deviceResources.get(),// デバイスリソース
		m_commonStates.get(),	// コモンステート
		m_debugString.get(),	// デバッグ文字列
		m_inputManager.get(),	// 入力マネージャ
		m_audioManager.get(),	// オーディオマネージャ
		m_modelManager.get(),	// モデルマネージャ
		m_textureManager.get()	// テクスチャマネージャ 
	);
	// シーンマネージャを作成する
	m_sceneManager = std::make_unique<SceneManager>();
	// シーンマネージャを初期化する
	m_sceneManager->Initialize(m_pCommonResources.get());
	//カーソルを見えるようにする
	ShowCursor(TRUE);
}

#pragma region Frame Update
/*
*	@brief ゲームループを実行する
*	@detail ゲームループを実行する関数
*	@param なし
*	@return なし
*/
void Game::Tick()
{
	// タイマーを更新し、Update関数を呼び出す
	m_timer.Tick([&]() { Update(m_timer); });
	// 描画処理を呼び出す
	Render();
}
/*
*	@brief ゲームの更新処理
*	@detail ゲームの更新処理を行う関数
*	@param timer タイマー
*	@return なし
*/
void Game::Update(DX::StepTimer const& timer)
{
	// 経過時間を取得する
	float elapsedTime = float(timer.GetElapsedSeconds());
	// 入力マネージャを更新する
	m_inputManager->Update();
	// ESCキーでゲームを終了する
	if (m_inputManager->GetKeyboardState().Escape)
	{
		ExitGame();// ゲームを終了する
		return;
	}
	// シーンマネージャを更新する
	m_sceneManager->Update(elapsedTime);
}
#pragma endregion

#pragma region Frame Render
/*
*	@brief ゲームの描画処理
*	@detail ゲームの描画処理を行う関数
*	@param なし
*	@return なし
*/
void Game::Render()
{
	// 最初のUpdate前は何も描画しない
	if (m_timer.GetFrameCount() == 0)
		return;
	Clear();// 画面をクリアする
	// PIXイベントを開始する
	m_deviceResources->PIXBeginEvent(L"Render");
	// デバイスコンテキストを取得する
	auto context = m_deviceResources->GetD3DDeviceContext();
	// デバイスコンテキストの未使用警告を抑制する
	UNREFERENCED_PARAMETER(context);
	// シーンマネージャを描画する
	m_sceneManager->Render();
#ifdef _DEBUG
	// デバッグ文字列を描画する
	m_debugString->Render(m_commonStates.get());
	// デバッグ文字列を作成する：FPS
	m_debugString->AddString("fps : %d", m_timer.GetFramesPerSecond());
#endif
	// PIXイベントを終了する
	m_deviceResources->PIXEndEvent();
	// 新しいフレームを表示する
	m_deviceResources->Present();
}

/*
*	@brief 画面をクリアする
*	@detail 画面をクリアする関数
*	@param なし
*	@return なし
*/
void Game::Clear()
{
	using namespace DirectX;
	// PIXイベントを開始する
	m_deviceResources->PIXBeginEvent(L"Clear");
	// デバイスコンテキストを取得する
	auto context = m_deviceResources->GetD3DDeviceContext();
	// レンダーターゲットビューを取得する
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	// 深度ステンシルビューを取得する
	auto depthStencil = m_deviceResources->GetDepthStencilView();
	// レンダリングターゲットビューをクリアする
	context->ClearRenderTargetView(renderTarget, Colors::Gray);
	// 深度ステンシルビューをクリアする
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// レンダーターゲットと深度ステンシルビューを設定する
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	// スクリーンビューポートを取得する
	auto const viewport = m_deviceResources->GetScreenViewport();
	// ビューポートを設定する
	context->RSSetViewports(1, &viewport);
	// PIXイベントを終了する
	m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
/*
*	@brief ゲームがアクティブ化されたときの処理
*	@detail ゲームがアクティブウィンドウになったときの処理
*	@param なし
*	@return なし
*/
void Game::OnActivated() {/*do nothing*/ }
/*
*	@brief ゲームが非アクティブ化されたときの処理
*	@detail ゲームが非アクティブウィンドウになったときの処理
*	@param なし
*	@return なし
*/
void Game::OnDeactivated() {/*do nothing*/ }
/*
*	@brief ゲームが一時停止されたときの処理
*	@detail ゲームが一時停止されたときの処理
*	@param なし
*	@return なし
*/
void Game::OnSuspending() {/*do nothing*/ }
/*
*	@brief ゲームが再開されたときの処理
*	@detail ゲームが再開されたときの処理
*	@param なし
*	@return なし
*/
void Game::OnResuming()
{
	// タイマーをリセットする
	m_timer.ResetElapsedTime();
}
/*
*	@brief ウィンドウが移動されたときの処理
*	@detail ウィンドウが移動されたときの処理
*	@param なし
*	@return なし
*/
void Game::OnWindowMoved()
{
	// ウィンドウのサイズを取得する
	auto const r = m_deviceResources->GetOutputSize();
	// ウィンドウサイズが変更されたことを通知する
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
	// フルスクリーンか調べる
	BOOL fullscreen = FALSE;
	m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);
	// フルスクリーン状態を取得する
	if (m_fullscreen != fullscreen)// フルスクリーンが解除されてしまった時の処理
	{
		// フルスクリーン状態を更新する
		m_fullscreen = fullscreen;
		// ウィンドウサイズ依存リソースを作成する
		m_deviceResources->CreateWindowSizeDependentResources();
	}
}
/*
*	@brief ディスプレイの変更があったときの処理
*	@detail ディスプレイの変更があったときの処理
*	@param なし
*	@return なし
*/
void Game::OnDisplayChange()
{
	// カラースペースを更新する
	m_deviceResources->UpdateColorSpace();
}
/*
*	@brief ウィンドウサイズが変更されたときの処理
*	@detail ウィンドウサイズが変更されたときの処理
*	@param width 新しいウィンドウの幅
*	@param height 新しいウィンドウの高さ
*	@return なし
*/
void Game::OnWindowSizeChanged(int width, int height)
{
	// ウィンドウサイズが変更されていない場合は何もしない
	if (!m_deviceResources->WindowSizeChanged(width, height))return;
	// ウィンドウサイズ依存リソースを作成する
	CreateWindowSizeDependentResources();
}

/*
*	@brief デフォルトのウィンドウサイズを取得する
*	@detail デフォルトのウィンドウサイズを取得する関数
*	@param width デフォルトのウィンドウの幅
*	@param height デフォルトのウィンドウの高さ
*	@return なし
*/
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	// デフォルトのウィンドウの幅を設定
	width = Screen::WIDTH;
	// デフォルトのウィンドウの高さを設定
	height = Screen::HEIGHT;
}
#pragma endregion

#pragma region Direct3D Resources
/*
*	@brief デバイス依存リソースを作成する
*	@detail デバイス依存リソースを作成する関数
*	@param なし
*	@return なし
*/
void Game::CreateDeviceDependentResources()
{
	// デバイスを取得する
	auto device = m_deviceResources->GetD3DDevice();
	// デバイスの未使用警告を抑制する
	UNREFERENCED_PARAMETER(device);
}
/*
*	@brief ウィンドウサイズ依存リソースを作成する
*	@detail ウィンドウサイズ依存リソースを作成する関数
*	@param なし
*	@return なし
*/
void Game::CreateWindowSizeDependentResources() {/*do nothing*/ }
/*
*	@brief デバイスが失われたときの処理
*	@detail デバイスが失われたときの処理を行う関数
*	@param なし
*	@return なし
*/
void Game::OnDeviceLost() {/*do nothing*/ }
/*
*	@brief デバイスが復元されたときの処理
*	@detail デバイス・ウィンドウサイズ依存リソースを再作成する関数
*	@param なし
*	@return なし
*/
void Game::OnDeviceRestored()
{
	// デバイス依存リソースを作成
	CreateDeviceDependentResources();
	// ウィンドウサイズ依存リソースを作成
	CreateWindowSizeDependentResources();
}
/*
*	@brief フルスクリーン状態を設定する
*	@detail フルスクリーン状態を設定する関数
*	@param value フルスクリーン状態（TRUE: フルスクリーン、FALSE: ウィンドウモード）
*	@return なし
*/
void Game::SetFullscreenState(BOOL value)
{
	// フルスクリーン状態を更新
	m_fullscreen = value;
	// フルスクリーン状態を設定
	m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
	// フルスクリーン状態に応じてウィンドウサイズ依存リソースを作成
	if (value) m_deviceResources->CreateWindowSizeDependentResources();
}
#pragma endregion
