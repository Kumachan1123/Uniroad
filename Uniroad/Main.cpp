/*
*	@file Main.cpp
*	@brief �Q�[���̉�ʂ�\�����郁�C���֐�
*/
// �w�b�_�[�t�@�C��
#include <pch.h>
#include "Game.h"
#include <Libraries/MyLib/MemoryLeakDetector.h>

using namespace DirectX;

#ifdef __clang__// clang�R���p�C�����g�p���Ă���ꍇ
#pragma clang diagnostic ignored "-Wcovered-switch-default"// �f�t�H���g�̃P�[�X���J�o�[����Ă��Ȃ��Ƃ����x���𖳎�
#pragma clang diagnostic ignored "-Wswitch-enum"// �񋓌^��switch���Ɋւ���x���𖳎�
#endif
#pragma warning(disable : 4061)// enum�l��switch���ŃJ�o�[����Ă��Ȃ��Ƃ����x���𖳎�
// �E�B���h�E�X�^�C���̒�`�i�^�C�g���o�[�A�V�X�e�����j���[�A�ŏ����{�^���A�ő剻�{�^�����܂ށj
#define WS_MY_WINDOW    (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

// �O���[�o���ϐ��̐錾
namespace
{
	std::unique_ptr<Game> g_game;// �Q�[���I�u�W�F�N�g�̃O���[�o���|�C���^�[
}
// �A�v���P�[�V������
LPCWSTR g_szAppName = L"�Ђ��̃��j���[�h";
// Windows�v���V�[�W���̐錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// �Q�[���I�������̐錾
void ExitGame() noexcept;

// NVIDIA��AMD�̃O���t�B�b�N�J�[�h�̍œK���ݒ�
extern "C"// C���ꃊ���P�[�W���g�p���ăG�N�X�|�[�g���邽�߂̐錾
{
	// NVIDIA Optimus��L���ɂ��邽�߂̃t���O
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// AMD�̍����\���[�h��v�����邽�߂̃t���O
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

/*
*	@brief ���C���֐�
*	@details �Q�[���̃��C���G���g���|�C���g
*	@param hInstance �A�v���P�[�V�����̃C���X�^���X�n���h��
*	@param hPrevInstance �O�̃C���X�^���X�n���h���i�g�p���Ȃ��j
*	@param lpCmdLine �R�}���h���C�������i�g�p���Ȃ��j
*	@param nCmdShow �E�B���h�E�̕\�����
*	@return �A�v���P�[�V�����̏I���R�[�h
*/
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(_DEBUG)// �f�o�b�O�r���h�̏ꍇ
	// ���������[�N���o���������
	mylib::MemoryLeakDetector();
#endif
	// hPrevInstance�̖��g�p�x�����\���ɂ���
	UNREFERENCED_PARAMETER(hPrevInstance);
	// lpCmdLine�̖��g�p�x�����\���ɂ���
	UNREFERENCED_PARAMETER(lpCmdLine);
	// CPU��DirectX Math���C�u�������T�|�[�g���Ă��Ȃ��ꍇ�͏I��
	if (!XMVerifyCPUSupport())return 1;
	// COM���C�u������������(�}���`�X���b�h�x�[�X�̏��������g�p)
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	// COM�̏������Ɏ��s�����ꍇ�͏I��
	if (FAILED(hr))	return 1;
	// �Q�[���I�u�W�F�N�g���쐬
	g_game = std::make_unique<Game>();
	// �t���X�N���[�����[�h�̏����l��ݒ�(true: �t���X�N���[���Afalse: �E�B���h�E���[�h)
	static bool s_fullscreen = true;
	// ��ʃ��[�h�I��
	if (MessageBox(NULL, L"�t���X�N���[���ɂ��܂����H", L"��ʃ��[�h�ݒ�", MB_YESNO) == IDYES)// ���[�U�[���u�͂��v��I�������ꍇ
	{
		// �t���X�N���[�����[�h�ɐݒ�
		s_fullscreen = true;
	}
	else	// ���[�U�[���u�������v��I�������ꍇ
	{
		// �E�B���h�E���[�h�ɐݒ�
		s_fullscreen = false;
	}

	// �E�B���h�E�N���X��o�^���A�E�B���h�E���쐬����
	{
		// Windows�̃E�B���h�E�N���X���`����\����
		WNDCLASSEXW wcex = {};
		// �\���̂̃T�C�Y��ݒ�
		wcex.cbSize = sizeof(WNDCLASSEXW);
		// �E�B���h�E�̃X�^�C����ݒ�i��������ѐ����̃��T�C�Y���ɍĕ`��j
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		// �E�B���h�E�v���V�[�W���̊֐��|�C���^��ݒ�
		wcex.lpfnWndProc = WndProc;
		// �A�v���P�[�V�����̃C���X�^���X�n���h����ݒ�
		wcex.hInstance = hInstance;
		// �A�v���P�[�V�����̃A�C�R�������[�h
		wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
		// �f�t�H���g�̃J�[�\����ݒ�
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		// �E�B���h�E�̔w�i�F��ݒ�
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		// �E�B���h�E�N���X����ݒ�
		wcex.lpszClassName = L"_3DPGWindowClass";
		// �������A�C�R�������[�h
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
		// �E�B���h�E�N���X�̓o�^�Ɏ��s�����ꍇ�͏I��
		if (!RegisterClassExW(&wcex)) 	return 1;
		// �E�B���h�E�̏�����
		int w, h;
		// �t���X�N���[���̏ꍇ
		if (s_fullscreen == true)
		{
			// ����1920�ɐݒ�
			w = 1920;
			// ������1080�ɐݒ�
			h = 1080;
		}
		// �E�B���h�E���[�h�̏ꍇ�̓Q�[���̃f�t�H���g�T�C�Y���擾
		else g_game->GetDefaultSize(w, h);
		// �E�B���h�E�̃N���C�A���g�̈�̋�`���`
		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
		// �E�B���h�E�̃N���C�A���g�̈�̋�`�𒲐�
		AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);
		// �E�B���h�E���쐬
		HWND hwnd = CreateWindowExW(
			0,						// �g���X�^�C���Ȃ�
			L"_3DPGWindowClass",	// �E�B���h�E�N���X��
			g_szAppName,			// �A�v���P�[�V������
			WS_MY_WINDOW,			// �E�B���h�E�X�^�C���i�^�C�g���o�[�A�V�X�e�����j���[�A�ŏ����{�^���A�ő剻�{�^�����܂ށj
			CW_USEDEFAULT,			// �E�B���h�E�ʒu�i�f�t�H���g�ʒu�j
			CW_USEDEFAULT,			// �E�B���h�E�ʒu�i�f�t�H���g�ʒu�j
			rc.right - rc.left,		// �E�B���h�E�̕��i������̋�`�̕��j
			rc.bottom - rc.top,		// �E�B���h�E�̍����i������̋�`�̍����j
			nullptr,				// �e�E�B���h�E�Ȃ�
			nullptr,				// ���j���[�n���h���Ȃ�
			hInstance,				// �A�v���P�[�V�����̃C���X�^���X�n���h��
			nullptr					// �A�v���P�[�V�����̃p�����[�^�Ȃ�
		);
		// �E�B���h�E�̍쐬�Ɏ��s�����ꍇ�͏I��
		if (!hwnd)	return 1;
		// �E�B���h�E��\������
		ShowWindow(hwnd, nCmdShow);
		// �E�B���h�E�̃��[�U�[�f�[�^�ɃQ�[���I�u�W�F�N�g�̃|�C���^��ݒ�
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));
		// �E�B���h�E�̃N���C�A���g�̈�̋�`���擾
		GetClientRect(hwnd, &rc);
		// �Q�[���̏��������s��
		g_game->Initialize(hwnd, 1920, 1080);
		// �t���X�N���[�����[�h�̏ꍇ
		if (s_fullscreen)
		{
			// �Q�[���̃t���X�N���[����Ԃ�ݒ�
			g_game->SetFullscreenState(TRUE);
		}
	}
	// ���b�Z�[�W�\���̂�������
	MSG msg = {};
	// ���b�Z�[�W��WM_QUIT�łȂ����胋�[�v�𑱂���
	while (WM_QUIT != msg.message)
	{
		// ���b�Z�[�W���L���[�ɂ��邩���m�F
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))// ���b�Z�[�W���L���[�ɂ���Ύ��o���ď���
		{
			// ���b�Z�[�W��ϊ�
			TranslateMessage(&msg);
			// ���b�Z�[�W���f�B�X�p�b�`
			DispatchMessage(&msg);
		}
		else// ���b�Z�[�W���Ȃ��ꍇ
		{
			// �Q�[����Tick�֐����Ăяo���čX�V�������s��
			g_game->Tick();
		}
	}
	// �t���X�N���[�����[�h�̏ꍇ
	if (s_fullscreen)
	{
		// �Q�[���̃t���X�N���[����Ԃ�����
		g_game->SetFullscreenState(FALSE);
	}
	// �Q�[���I�u�W�F�N�g�����Z�b�g���ă����������
	g_game.reset();
	// COM���C�u�����̏I������
	CoUninitialize();
	// ���b�Z�[�W��wParam��Ԃ��i�A�v���P�[�V�����̏I���R�[�h�j
	return static_cast<int>(msg.wParam);
}
/*
*	@brief �E�B���h�E�v���V�[�W��
*	@details �E�B���h�E�ɑ����郁�b�Z�[�W����������֐�
*	@param hWnd �E�B���h�E�n���h��
*	@param message ���b�Z�[�W�̎��ʎq
*	@param wParam ���b�Z�[�W�Ɋ֘A����ǉ����
*	@param lParam ���b�Z�[�W�Ɋ֘A����ǉ����
*	@return ���b�Z�[�W�̏�������
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��Ԃ�ێ����邽�߂̐ÓI�ϐ�
	// �E�B���h�E�̃T�C�Y�ύX�����ǂ����������t���O
	static bool s_in_sizemove = false;
	// �A�v���P�[�V�������T�X�y���h��Ԃ��ǂ����������t���O
	static bool s_in_suspend = false;
	// �A�v���P�[�V�������ŏ�������Ă��邩�ǂ����������t���O
	static bool s_minimized = false;
	// �A�v���P�[�V�������t���X�N���[�����[�h���ǂ����������t���O
	static bool s_fullscreen = false;

	// �E�B���h�E�̃��[�U�[�f�[�^����Q�[���I�u�W�F�N�g�̃|�C���^���擾
	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)// ���b�Z�[�W�̎�ނɉ����ď����𕪊�
	{
	case WM_PAINT:// �E�B���h�E�̍ĕ`��v��
		// �E�B���h�E�T�C�Y���ύX���ŃQ�[�������݂���ꍇ
		if (s_in_sizemove && game)
		{
			// �T�C�Y�ύX���͎��O�ŕ`��
			game->Tick();
		}
		else // �T�C�Y�ύX���łȂ��ꍇ�� 
		{
			// �`��̂��߂̍\���̂�������
			PAINTSTRUCT ps;
			// �E�B���h�E�̕`����J�n
			std::ignore = BeginPaint(hWnd, &ps);
			// �`����I��
			EndPaint(hWnd, &ps);
		}
		// switch���̏I��
		break;
	case WM_DISPLAYCHANGE:// �f�B�X�v���C�̐ݒ肪�ύX���ꂽ�Ƃ�
		// �Q�[���I�u�W�F�N�g�Ƀf�B�X�v���C�ύX��ʒm
		if (game)game->OnDisplayChange();
		// switch���̏I��
		break;
	case WM_MOVE:// �E�B���h�E���ړ����ꂽ�Ƃ�
		// �Q�[���I�u�W�F�N�g�ɃE�B���h�E�ړ���ʒm
		if (game)game->OnWindowMoved();
		// switch���̏I��
		break;
	case WM_ENTERSIZEMOVE:// �E�B���h�E�̃T�C�Y�ύX���J�n���ꂽ�Ƃ�
		// �T�C�Y�ύX���t���O��ݒ�
		s_in_sizemove = true;
		// switch���̏I��
		break;
	case WM_EXITSIZEMOVE:// �E�B���h�E�̃T�C�Y�ύX���I�������Ƃ�
		// �T�C�Y�ύX���t���O������
		s_in_sizemove = false;
		// �Q�[���I�u�W�F�N�g�����݂���ꍇ
		if (game)
		{
			// �E�B���h�E�̃N���C�A���g�̈�̋�`���`
			RECT rc;
			// �E�B���h�E�̃N���C�A���g�̈�̋�`���擾
			GetClientRect(hWnd, &rc);
			// �Q�[���I�u�W�F�N�g�ɃE�B���h�E�T�C�Y�ύX��ʒm
			game->OnWindowSizeChanged(1920, 1080);
		}
		// switch���̏I��
		break;
	case WM_GETMINMAXINFO:// �E�B���h�E�̍ŏ�����эő�T�C�Y���擾���邽�߂̃��b�Z�[�W
		// lParam��nullptr�łȂ��ꍇ
		if (lParam)
		{
			// lParam��MINMAXINFO�\���̂ɃL���X�g
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			// �ŏ��g���b�N�T�C�Y�̕���320�ɐݒ�
			info->ptMinTrackSize.x = 320;
			// �ŏ��g���b�N�T�C�Y�̍�����180�ɐݒ�
			info->ptMinTrackSize.y = 180;
		}
		// switch���̏I��
		break;
	case WM_ACTIVATEAPP:
		if (game)// �Q�[���I�u�W�F�N�g�����݂���ꍇ
		{
			// lParam��nullptr�łȂ��ꍇ
			if (wParam)// �A�v���P�[�V�������A�N�e�B�u�ɂȂ����ꍇ
			{
				// OnActivated���Ăяo��
				game->OnActivated();
			}
			else // �A�v���P�[�V��������A�N�e�B�u�ɂȂ����ꍇ
			{
				// OnDeactivated���Ăяo��
				game->OnDeactivated();
			}
		}
		// �L�[�{�[�h�̃��b�Z�[�W������
		Keyboard::ProcessMessage(message, wParam, lParam);
		// �}�E�X�̃��b�Z�[�W������
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		// switch���̏I��
		break;
	case WM_POWERBROADCAST: // �V�X�e���̓d����Ԃ��ύX���ꂽ�Ƃ��ɑ��M����郁�b�Z�[�W
		// wParam�̒l�ɉ����ď����𕪊�
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:// �V�X�e�����T�X�y���h��Ԃɓ���O�ɑ��M����郁�b�Z�[�W
			// �Q�[���I�u�W�F�N�g�ɃT�X�y���h��ʒm
			if (!s_in_suspend && game)	game->OnSuspending();
			// �T�X�y���h��ԃt���O��ݒ�
			s_in_suspend = true;
			// ���b�Z�[�W�̏����������������Ƃ�����TRUE��Ԃ�
			return TRUE;
		case PBT_APMRESUMESUSPEND: // �V�X�e�����T�X�y���h��Ԃ��畜�A�����Ƃ��ɑ��M����郁�b�Z�[�W
			// �E�B���h�E���ŏ�������Ă��Ȃ��ꍇ
			if (!s_minimized)
			{
				// �Q�[���I�u�W�F�N�g�ɕ��A��ʒm
				if (s_in_suspend && game)game->OnResuming();
				// �T�X�y���h��ԃt���O������
				s_in_suspend = false;
			}
			// ���b�Z�[�W�̏����������������Ƃ�����TRUE��Ԃ�
			return TRUE;
		}
		// switch���̏I��
		break;
	case WM_DESTROY:// �E�B���h�E���j�������Ƃ��ɑ��M����郁�b�Z�[�W
		// ���b�Z�[�W�L���[��WM_QUIT���b�Z�[�W���|�X�g���āA�A�v���P�[�V�������I������悤�Ɏw��
		PostQuitMessage(0);
		// switch���̏I��
		break;
	case WM_ACTIVATE:		// �E�B���h�E���A�N�e�B�u����A�N�e�B�u�ɂȂ����Ƃ�
	case WM_INPUT:			// DirectInput��RawInput���g�p���Ă���
	case WM_MOUSEMOVE:		// �}�E�X���ړ������Ƃ�
	case WM_LBUTTONDOWN:	// ���{�^���������ꂽ�Ƃ�
	case WM_LBUTTONUP:		// ���{�^���������ꂽ�Ƃ�
	case WM_RBUTTONDOWN:	// �E�{�^���������ꂽ�Ƃ�
	case WM_RBUTTONUP:		// �E�{�^���������ꂽ�Ƃ�
	case WM_MBUTTONDOWN:	// ���{�^���������ꂽ�Ƃ�
	case WM_MBUTTONUP:		// ���{�^���������ꂽ�Ƃ�
	case WM_MOUSEWHEEL:		// �}�E�X�z�C�[������]�����Ƃ�
	case WM_XBUTTONDOWN:	// �T�C�h�{�^���������ꂽ�Ƃ�
	case WM_XBUTTONUP:		// �T�C�h�{�^���������ꂽ�Ƃ�
	case WM_MOUSEHOVER:		// �}�E�X���z�o�[��ԂɂȂ����Ƃ�
		// �}�E�X�̃��b�Z�[�W������
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		// switch���̏I��
		break;
	case WM_KEYDOWN:		// �L�[�������ꂽ�Ƃ�
	case WM_KEYUP:			// �L�[�������ꂽ�Ƃ�
	case WM_SYSKEYUP:		// �V�X�e���L�[�������ꂽ�Ƃ�
		// �L�[�{�[�h�̃��b�Z�[�W������
		Keyboard::ProcessMessage(message, wParam, lParam);
		// switch���̏I��
		break;
	case WM_SYSKEYDOWN:		// �V�X�e���L�[�������ꂽ�Ƃ�
		// Alt�L�[�������ꂽ��Ԃ�Enter�L�[�������ꂽ�ꍇ�A�t���X�N���[�����[�h��؂�ւ���
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// �t���X�N���[�����[�h�̏ꍇ
			if (s_fullscreen)
			{
				// �E�B���h�E�X�^�C����ʏ�̃E�B���h�E�X�^�C���ɐݒ�
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_MY_WINDOW);
				// �g���X�^�C����ʏ�̃E�B���h�E�X�^�C���ɐݒ�
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);
				// ����1920�ɐݒ�
				int width = 1920;
				// ������1080�ɐݒ�
				int height = 1080;
				// �Q�[���I�u�W�F�N�g�����݂���ꍇ�A�f�t�H���g�̃E�B���h�E�T�C�Y���擾
				if (game)game->GetDefaultSize(width, height);
				// �E�B���h�E��ʏ�̏�Ԃŕ\��
				ShowWindow(hWnd, SW_SHOWNORMAL);
				// �E�B���h�E�����̃T�C�Y�ɖ߂��Ƃ��ɃT�C�Y��␳����
				// �E�B���h�E�̃N���C�A���g�̈�̋�`��������
				RECT rc{};
				// �E�B���h�E�̃N���C�A���g�̈�̋�`�𒲐�
				AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);
				// �E�B���h�E�̈ʒu�ƃT�C�Y��ݒ�
				SetWindowPos(
					hWnd, HWND_TOP,											// �E�B���h�E���őO�ʂɕ\��
					0, 0,													// �E�B���h�E�̈ʒu��(0, 0)�ɐݒ�
					width + rc.right - rc.left, height + rc.bottom - rc.top,// �E�B���h�E�̃T�C�Y�𒲐�
					SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED			// �E�B���h�E�̈ʒu��ύX�����AZ�I�[�_�[��ύX�����A�t���[�����ĕ`�悷��
				);
			}
			else// �E�B���h�E���[�h�̏ꍇ
			{
				// �E�B���h�E�X�^�C�����|�b�v�A�b�v�X�^�C���ɐݒ�
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
				// �g���X�^�C�����őO�ʂɐݒ�
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
				// �E�B���h�E�̈ʒu�ƃT�C�Y��ύX�����AZ�I�[�_�[��ύX�����A�t���[�����ĕ`�悷��
				SetWindowPos(
					hWnd,															// �E�B���h�E�n���h��
					HWND_TOP,														// �E�B���h�E���őO�ʂɕ\��
					0, 0, 0, 0,														// �E�B���h�E�̈ʒu��(0, 0)�ɐݒ�
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);		// �E�B���h�E�̈ʒu��ύX�����A�T�C�Y��ύX�����AZ�I�[�_�[��ύX�����A�t���[�����ĕ`�悷��
				// �E�B���h�E���ő剻���ĕ\��
				ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			}
			s_fullscreen = !s_fullscreen; // �t���X�N���[����Ԃ�؂�ւ���
		}
		// switch���̏I��
		break;

	case WM_MENUCHAR:// ���j���[�̃L�����N�^�[���͂����������Ƃ�
		// ���j���[���A�N�e�B�u�ŁA���[�U�[���j�[���j�b�N��A�N�Z�����[�^�L�[�ɑΉ����Ȃ��L�[���������ꍇ�͖���
		return MAKELRESULT(0, MNC_CLOSE);
	}
	// �f�t�H���g�̃E�B���h�E�v���V�[�W�����Ăяo���āA���b�Z�[�W����������
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
*	@brief �Q�[�����I������֐�
*	@details ���̊֐��̓Q�[�����I�����邽�߂Ɏg�p�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ExitGame() noexcept
{
	// ���b�Z�[�W�L���[��WM_QUIT���b�Z�[�W���|�X�g���āA�A�v���P�[�V�������I������悤�Ɏw��
	PostQuitMessage(0);
}