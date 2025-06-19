// Main.cpp

#include "pch.h"
#include "Game.h"

// ★追記★
#include "Libraries/MyLib/MemoryLeakDetector.h"

using namespace DirectX;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

// ★追記★
#define WS_MY_WINDOW    (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

namespace
{
	std::unique_ptr<Game> g_game;
}

LPCWSTR g_szAppName = L"ShaderLab";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ExitGame() noexcept;

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// ★追記★
#if defined(_DEBUG)
	mylib::MemoryLeakDetector();
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!XMVerifyCPUSupport())
		return 1;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;

	g_game = std::make_unique<Game>();
	static bool s_fullscreen = true;;
#ifdef _DEBUG


#endif

	// 画面モード選択
	if (MessageBox(NULL, L"フルスクリーンにしますか？", L"画面モード設定", MB_YESNO) == IDYES)
	{
		s_fullscreen = true;
	}
	else
	{
		s_fullscreen = false;
	}
	// Register class and create window
	{
		// Register class
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcex.lpszClassName = L"_3DPGWindowClass";
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
		if (!RegisterClassExW(&wcex))
			return 1;

		// Create window
		int w, h;


		if (s_fullscreen == true)// フルスクリーンの場合
		{
			w = 1920;
			h = 1080;
		}
		else
		{
			g_game->GetDefaultSize(w, h);
		}

		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

		AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE); // ★変更::WS_OVERLAPPEDWINDOW->WS_MY_WINDOW

		HWND hwnd = CreateWindowExW(
			0,
			L"_3DPGWindowClass",
			g_szAppName,
			WS_MY_WINDOW,   // ★変更::WS_OVERLAPPEDWINDOW->WS_MY_WINDOW
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left,
			rc.bottom - rc.top,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);
		// TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"_3DPGWindowClass", g_szAppName, WS_POPUP,
		// to default to fullscreen.

		if (!hwnd)
			return 1;

		ShowWindow(hwnd, nCmdShow); // ★変更: nCmdShow -> SW_SHOWDEFAULT
		// TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));

		GetClientRect(hwnd, &rc);

		g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
		if (s_fullscreen) g_game->SetFullscreenState(TRUE);
	}

	// Main message loop
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_game->Tick();
		}
	}
	if (s_fullscreen) g_game->SetFullscreenState(FALSE);
	g_game.reset();

	CoUninitialize();

	return static_cast<int>(msg.wParam);
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;

	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_PAINT:
		if (s_in_sizemove && game)
		{
			game->Tick();
		}
		else
		{
			PAINTSTRUCT ps;
			std::ignore = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_DISPLAYCHANGE:
		if (game)
		{
			game->OnDisplayChange();
		}
		break;

	case WM_MOVE:
		if (game)
		{
			game->OnWindowMoved();
		}
		break;

		//case WM_SIZE:
		//	if (wParam == SIZE_MINIMIZED)
		//	{
		//		if (!s_minimized)
		//		{
		//			s_minimized = true;
		//			if (!s_in_suspend && game)
		//				game->OnSuspending();
		//			s_in_suspend = true;
		//		}
		//	}
		//	else if (s_minimized)
		//	{
		//		s_minimized = false;
		//		if (s_in_suspend && game)
		//			game->OnResuming();
		//		s_in_suspend = false;
		//	}
		//	// フルスクリーン切り替え時にコメント化すると、画面サイズがリサイズされずに最大化できる
		//	// フルスクリーン時でも、見た目の1280x720サイズを保持する
		//	//else if (!s_in_sizemove && game)
		//	//{
		//	//    game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		//	//}
		//	break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (game)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;

	case WM_GETMINMAXINFO:
		if (lParam)
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 320;
			info->ptMinTrackSize.y = 200;
		}
		break;

	case WM_ACTIVATEAPP:
		if (game)
		{
			if (wParam)
			{
				game->OnActivated();
			}
			else
			{
				game->OnDeactivated();
			}
		}

		// ★追記★
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);

		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && game)
				game->OnSuspending();
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend && game)
					game->OnResuming();
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// ★追記ココから★
	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
		// ★追記ココまで★

	//case WM_SYSKEYDOWN:
	//	if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
	//	{
	//		// Implements the classic ALT+ENTER fullscreen toggle
	//		if (s_fullscreen)
	//		{
	//			SetWindowLongPtr(hWnd, GWL_STYLE, WS_MY_WINDOW);    // ★変更::WS_OVERLAPPEDWINDOW->WS_MY_WINDOW
	//			SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

	//			int width = 800;
	//			int height = 600;
	//			if (game)
	//				game->GetDefaultSize(width, height);

	//			ShowWindow(hWnd, SW_SHOWNORMAL);

	//			// ウィンドウを元のサイズに戻すときにサイズを補正する
	//			RECT rc{};
	//			AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);
	//			SetWindowPos(
	//				hWnd, HWND_TOP,
	//				0, 0,
	//				width + rc.right - rc.left, height + rc.bottom - rc.top,
	//				SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
	//			);
	//		}
	//		else
	//		{
	//			SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
	//			SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

	//			SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	//			ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	//		}

	//		s_fullscreen = !s_fullscreen;
	//	}
	//	break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		return MAKELRESULT(0, MNC_CLOSE);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Exit helper
void ExitGame() noexcept
{
	PostQuitMessage(0);
}
