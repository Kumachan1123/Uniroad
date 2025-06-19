/*
*	@file Main.cpp
*	@brief ゲームの画面を表示するメイン関数
*/
// ヘッダーファイル
#include <pch.h>
#include "Game.h"
#include <Libraries/MyLib/MemoryLeakDetector.h>

using namespace DirectX;

#ifdef __clang__// clangコンパイラを使用している場合
#pragma clang diagnostic ignored "-Wcovered-switch-default"// デフォルトのケースがカバーされていないという警告を無視
#pragma clang diagnostic ignored "-Wswitch-enum"// 列挙型のswitch文に関する警告を無視
#endif
#pragma warning(disable : 4061)// enum値がswitch文でカバーされていないという警告を無視
// ウィンドウスタイルの定義（タイトルバー、システムメニュー、最小化ボタン、最大化ボタンを含む）
#define WS_MY_WINDOW    (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

// グローバル変数の宣言
namespace
{
	std::unique_ptr<Game> g_game;// ゲームオブジェクトのグローバルポインター
}
// アプリケーション名
LPCWSTR g_szAppName = L"シグナルレイダーズ";
// Windowsプロシージャの宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// ゲーム終了処理の宣言
void ExitGame() noexcept;

// NVIDIAとAMDのグラフィックカードの最適化設定
extern "C"// C言語リンケージを使用してエクスポートするための宣言
{
	// NVIDIA Optimusを有効にするためのフラグ
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// AMDの高性能モードを要求するためのフラグ
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

/*
*	@brief メイン関数
*	@details ゲームのメインエントリポイント
*	@param hInstance アプリケーションのインスタンスハンドル
*	@param hPrevInstance 前のインスタンスハンドル（使用しない）
*	@param lpCmdLine コマンドライン引数（使用しない）
*	@param nCmdShow ウィンドウの表示状態
*	@return アプリケーションの終了コード
*/
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(_DEBUG)// デバッグビルドの場合
	// メモリリーク検出器を初期化
	mylib::MemoryLeakDetector();
#endif
	// hPrevInstanceの未使用警告を非表示にする
	UNREFERENCED_PARAMETER(hPrevInstance);
	// lpCmdLineの未使用警告を非表示にする
	UNREFERENCED_PARAMETER(lpCmdLine);
	// CPUがDirectX Mathライブラリをサポートしていない場合は終了
	if (!XMVerifyCPUSupport())return 1;
	// COMライブラリを初期化(マルチスレッドベースの初期化を使用)
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	// COMの初期化に失敗した場合は終了
	if (FAILED(hr))	return 1;
	// ゲームオブジェクトを作成
	g_game = std::make_unique<Game>();
	// フルスクリーンモードの初期値を設定(true: フルスクリーン、false: ウィンドウモード)
	static bool s_fullscreen = true;
#ifdef _DEBUG// デバッグビルドの場合
	// 画面モード選択
	if (MessageBox(NULL, L"フルスクリーンにしますか？", L"画面モード設定", MB_YESNO) == IDYES)// ユーザーが「はい」を選択した場合
	{
		// フルスクリーンモードに設定
		s_fullscreen = true;
	}
	else	// ユーザーが「いいえ」を選択した場合
	{
		// ウィンドウモードに設定
		s_fullscreen = false;
	}
#endif
	// ウィンドウクラスを登録し、ウィンドウを作成する
	{
		// Windowsのウィンドウクラスを定義する構造体
		WNDCLASSEXW wcex = {};
		// 構造体のサイズを設定
		wcex.cbSize = sizeof(WNDCLASSEXW);
		// ウィンドウのスタイルを設定（水平および垂直のリサイズ時に再描画）
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		// ウィンドウプロシージャの関数ポインタを設定
		wcex.lpfnWndProc = WndProc;
		// アプリケーションのインスタンスハンドルを設定
		wcex.hInstance = hInstance;
		// アプリケーションのアイコンをロード
		wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
		// デフォルトのカーソルを設定
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		// ウィンドウの背景色を設定
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		// ウィンドウクラス名を設定
		wcex.lpszClassName = L"_3DPGWindowClass";
		// 小さいアイコンをロード
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
		// ウィンドウクラスの登録に失敗した場合は終了
		if (!RegisterClassExW(&wcex)) 	return 1;
		// ウィンドウの初期化
		int w, h;
		// フルスクリーンの場合
		if (s_fullscreen == true)
		{
			// 幅を1920に設定
			w = 1920;
			// 高さを1080に設定
			h = 1080;
		}
		// ウィンドウモードの場合はゲームのデフォルトサイズを取得
		else g_game->GetDefaultSize(w, h);
		// ウィンドウのクライアント領域の矩形を定義
		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
		// ウィンドウのクライアント領域の矩形を調整
		AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);
		// ウィンドウを作成
		HWND hwnd = CreateWindowExW(
			0,						// 拡張スタイルなし
			L"_3DPGWindowClass",	// ウィンドウクラス名
			g_szAppName,			// アプリケーション名
			WS_MY_WINDOW,			// ウィンドウスタイル（タイトルバー、システムメニュー、最小化ボタン、最大化ボタンを含む）
			CW_USEDEFAULT,			// ウィンドウ位置（デフォルト位置）
			CW_USEDEFAULT,			// ウィンドウ位置（デフォルト位置）
			rc.right - rc.left,		// ウィンドウの幅（調整後の矩形の幅）
			rc.bottom - rc.top,		// ウィンドウの高さ（調整後の矩形の高さ）
			nullptr,				// 親ウィンドウなし
			nullptr,				// メニューハンドルなし
			hInstance,				// アプリケーションのインスタンスハンドル
			nullptr					// アプリケーションのパラメータなし
		);
		// ウィンドウの作成に失敗した場合は終了
		if (!hwnd)	return 1;
		// ウィンドウを表示する
		ShowWindow(hwnd, nCmdShow);
		// ウィンドウのユーザーデータにゲームオブジェクトのポインタを設定
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));
		// ウィンドウのクライアント領域の矩形を取得
		GetClientRect(hwnd, &rc);
		// ゲームの初期化を行う
		g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
		// フルスクリーンモードの場合
		if (s_fullscreen)
		{
			// ゲームのフルスクリーン状態を設定
			g_game->SetFullscreenState(TRUE);
		}
	}
	// メッセージ構造体を初期化
	MSG msg = {};
	// メッセージがWM_QUITでない限りループを続ける
	while (WM_QUIT != msg.message)
	{
		// メッセージがキューにあるかを確認
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))// メッセージがキューにあれば取り出して処理
		{
			// メッセージを変換
			TranslateMessage(&msg);
			// メッセージをディスパッチ
			DispatchMessage(&msg);
		}
		else// メッセージがない場合
		{
			// ゲームのTick関数を呼び出して更新処理を行う
			g_game->Tick();
		}
	}
	// フルスクリーンモードの場合
	if (s_fullscreen)
	{
		// ゲームのフルスクリーン状態を解除
		g_game->SetFullscreenState(FALSE);
	}
	// ゲームオブジェクトをリセットしてメモリを解放
	g_game.reset();
	// COMライブラリの終了処理
	CoUninitialize();
	// メッセージのwParamを返す（アプリケーションの終了コード）
	return static_cast<int>(msg.wParam);
}
/*
*	@brief ウィンドウプロシージャ
*	@details ウィンドウに送られるメッセージを処理する関数
*	@param hWnd ウィンドウハンドル
*	@param message メッセージの識別子
*	@param wParam メッセージに関連する追加情報
*	@param lParam メッセージに関連する追加情報
*	@return メッセージの処理結果
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 状態を保持するための静的変数
	// ウィンドウのサイズ変更中かどうかを示すフラグ
	static bool s_in_sizemove = false;
	// アプリケーションがサスペンド状態かどうかを示すフラグ
	static bool s_in_suspend = false;
	// アプリケーションが最小化されているかどうかを示すフラグ
	static bool s_minimized = false;
	// アプリケーションがフルスクリーンモードかどうかを示すフラグ
	static bool s_fullscreen = false;

	// ウィンドウのユーザーデータからゲームオブジェクトのポインタを取得
	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)// メッセージの種類に応じて処理を分岐
	{
	case WM_PAINT:// ウィンドウの再描画要求
		// ウィンドウサイズが変更中でゲームが存在する場合
		if (s_in_sizemove && game)
		{
			// サイズ変更中は自前で描画
			game->Tick();
		}
		else // サイズ変更中でない場合は 
		{
			// 描画のための構造体を初期化
			PAINTSTRUCT ps;
			// ウィンドウの描画を開始
			std::ignore = BeginPaint(hWnd, &ps);
			// 描画を終了
			EndPaint(hWnd, &ps);
		}
		// switch文の終了
		break;
	case WM_DISPLAYCHANGE:// ディスプレイの設定が変更されたとき
		// ゲームオブジェクトにディスプレイ変更を通知
		if (game)game->OnDisplayChange();
		// switch文の終了
		break;
	case WM_MOVE:// ウィンドウが移動されたとき
		// ゲームオブジェクトにウィンドウ移動を通知
		if (game)game->OnWindowMoved();
		// switch文の終了
		break;
	case WM_ENTERSIZEMOVE:// ウィンドウのサイズ変更が開始されたとき
		// サイズ変更中フラグを設定
		s_in_sizemove = true;
		// switch文の終了
		break;
	case WM_EXITSIZEMOVE:// ウィンドウのサイズ変更が終了したとき
		// サイズ変更中フラグを解除
		s_in_sizemove = false;
		// ゲームオブジェクトが存在する場合
		if (game)
		{
			// ウィンドウのクライアント領域の矩形を定義
			RECT rc;
			// ウィンドウのクライアント領域の矩形を取得
			GetClientRect(hWnd, &rc);
			// ゲームオブジェクトにウィンドウサイズ変更を通知
			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		// switch文の終了
		break;
	case WM_GETMINMAXINFO:// ウィンドウの最小および最大サイズを取得するためのメッセージ
		// lParamがnullptrでない場合
		if (lParam)
		{
			// lParamをMINMAXINFO構造体にキャスト
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			// 最小トラックサイズの幅を320に設定
			info->ptMinTrackSize.x = 320;
			// 最小トラックサイズの高さを180に設定
			info->ptMinTrackSize.y = 180;
		}
		// switch文の終了
		break;
	case WM_ACTIVATEAPP:
		if (game)// ゲームオブジェクトが存在する場合
		{
			// lParamがnullptrでない場合
			if (wParam)// アプリケーションがアクティブになった場合
			{
				// OnActivatedを呼び出す
				game->OnActivated();
			}
			else // アプリケーションが非アクティブになった場合
			{
				// OnDeactivatedを呼び出す
				game->OnDeactivated();
			}
		}
		// キーボードのメッセージを処理
		Keyboard::ProcessMessage(message, wParam, lParam);
		// マウスのメッセージを処理
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		// switch文の終了
		break;
	case WM_POWERBROADCAST: // システムの電源状態が変更されたときに送信されるメッセージ
		// wParamの値に応じて処理を分岐
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:// システムがサスペンド状態に入る前に送信されるメッセージ
			// ゲームオブジェクトにサスペンドを通知
			if (!s_in_suspend && game)	game->OnSuspending();
			// サスペンド状態フラグを設定
			s_in_suspend = true;
			// メッセージの処理が成功したことを示すTRUEを返す
			return TRUE;
		case PBT_APMRESUMESUSPEND: // システムがサスペンド状態から復帰したときに送信されるメッセージ
			// ウィンドウが最小化されていない場合
			if (!s_minimized)
			{
				// ゲームオブジェクトに復帰を通知
				if (s_in_suspend && game)game->OnResuming();
				// サスペンド状態フラグを解除
				s_in_suspend = false;
			}
			// メッセージの処理が成功したことを示すTRUEを返す
			return TRUE;
		}
		// switch文の終了
		break;
	case WM_DESTROY:// ウィンドウが破棄されるときに送信されるメッセージ
		// メッセージキューにWM_QUITメッセージをポストして、アプリケーションを終了するように指示
		PostQuitMessage(0);
		// switch文の終了
		break;
	case WM_ACTIVATE:		// ウィンドウがアクティブか非アクティブになったとき
	case WM_INPUT:			// DirectInputやRawInputを使用している
	case WM_MOUSEMOVE:		// マウスが移動したとき
	case WM_LBUTTONDOWN:	// 左ボタンが押されたとき
	case WM_LBUTTONUP:		// 左ボタンが離されたとき
	case WM_RBUTTONDOWN:	// 右ボタンが押されたとき
	case WM_RBUTTONUP:		// 右ボタンが離されたとき
	case WM_MBUTTONDOWN:	// 中ボタンが押されたとき
	case WM_MBUTTONUP:		// 中ボタンが離されたとき
	case WM_MOUSEWHEEL:		// マウスホイールが回転したとき
	case WM_XBUTTONDOWN:	// サイドボタンが押されたとき
	case WM_XBUTTONUP:		// サイドボタンが離されたとき
	case WM_MOUSEHOVER:		// マウスがホバー状態になったとき
		// マウスのメッセージを処理
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		// switch文の終了
		break;
	case WM_KEYDOWN:		// キーが押されたとき
	case WM_KEYUP:			// キーが離されたとき
	case WM_SYSKEYUP:		// システムキーが離されたとき
		// キーボードのメッセージを処理
		Keyboard::ProcessMessage(message, wParam, lParam);
		// switch文の終了
		break;
	case WM_SYSKEYDOWN:		// システムキーが押されたとき
		// Altキーが押された状態でEnterキーが押された場合、フルスクリーンモードを切り替える
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// フルスクリーンモードの場合
			if (s_fullscreen)
			{
				// ウィンドウスタイルを通常のウィンドウスタイルに設定
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_MY_WINDOW);
				// 拡張スタイルを通常のウィンドウスタイルに設定
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);
				// 幅を1920に設定
				int width = 1920;
				// 高さを1080に設定
				int height = 1080;
				// ゲームオブジェクトが存在する場合、デフォルトのウィンドウサイズを取得
				if (game)game->GetDefaultSize(width, height);
				// ウィンドウを通常の状態で表示
				ShowWindow(hWnd, SW_SHOWNORMAL);
				// ウィンドウを元のサイズに戻すときにサイズを補正する
				// ウィンドウのクライアント領域の矩形を初期化
				RECT rc{};
				// ウィンドウのクライアント領域の矩形を調整
				AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);
				// ウィンドウの位置とサイズを設定
				SetWindowPos(
					hWnd, HWND_TOP,											// ウィンドウを最前面に表示
					0, 0,													// ウィンドウの位置を(0, 0)に設定
					width + rc.right - rc.left, height + rc.bottom - rc.top,// ウィンドウのサイズを調整
					SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED			// ウィンドウの位置を変更せず、Zオーダーを変更せず、フレームを再描画する
				);
			}
			else// ウィンドウモードの場合
			{
				// ウィンドウスタイルをポップアップスタイルに設定
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
				// 拡張スタイルを最前面に設定
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
				// ウィンドウの位置とサイズを変更せず、Zオーダーを変更せず、フレームを再描画する
				SetWindowPos(
					hWnd,															// ウィンドウハンドル
					HWND_TOP,														// ウィンドウを最前面に表示
					0, 0, 0, 0,														// ウィンドウの位置を(0, 0)に設定
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);		// ウィンドウの位置を変更せず、サイズを変更せず、Zオーダーを変更せず、フレームを再描画する
				// ウィンドウを最大化して表示
				ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			}
			s_fullscreen = !s_fullscreen; // フルスクリーン状態を切り替える
		}
		// switch文の終了
		break;

	case WM_MENUCHAR:// メニューのキャラクター入力が発生したとき
		// メニューがアクティブで、ユーザーがニーモニックやアクセラレータキーに対応しないキーを押した場合は無視
		return MAKELRESULT(0, MNC_CLOSE);
	}
	// デフォルトのウィンドウプロシージャを呼び出して、メッセージを処理する
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
*	@brief ゲームを終了する関数
*	@details この関数はゲームを終了するために使用される
*	@param なし
*	@return なし
*/
void ExitGame() noexcept
{
	// メッセージキューにWM_QUITメッセージをポストして、アプリケーションを終了するように指示
	PostQuitMessage(0);
}