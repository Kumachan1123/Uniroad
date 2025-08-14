/*
*	@file Game.cpp
*	@brief �Q�[���̃��C���N���X
*/
#include <pch.h>
#include "Game.h"
/*
*	@brief �Q�[�����I������
*	@detail �Q�[�����I������֐�(Main.cpp����Ăяo�����)
*	@param �Ȃ�
*	@return �Ȃ�
*/
extern void ExitGame() noexcept;



using Microsoft::WRL::ComPtr;
/*
*	@brief �R���X�g���N�^
*	@detail �Q�[���̃��C���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Game::Game() noexcept(false)
	: m_deviceResources{}// �f�o�C�X���\�[�X
	, m_timer{}// �^�C�}�[
	, m_commonStates{}// �R�����X�e�[�g
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_debugString{}// �f�o�b�O������
	, m_inputManager{}// ���̓}�l�[�W��
	, m_sceneManager{}// �V�[���}�l�[�W��
	, m_audioManager{}// �I�[�f�B�I�}�l�[�W��
	, m_modelManager{}// ���f���}�l�[�W��
	, m_textureManager{}// �e�N�X�`���}�l�[�W��
	, m_fullscreen{ FALSE }// �t���X�N���[�����
{
	// �f�o�C�X���\�[�X���쐬����(�X�e���V���o�b�t�@���g�p���邽�߁ADXGI_FORMAT_D24_UNORM_S8_UINT���w��)
	m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
	// �f�o�C�X�ʒm��o�^����
	m_deviceResources->RegisterDeviceNotify(this);
}


/*
*	@brief �Q�[���̏�����
*	@detail �Q�[���̎��s�ɕK�v��Direct3D���\�[�X��������
*	@param window �E�B���h�E�n���h��
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Game::Initialize(HWND window, int width, int height)
{
	using namespace DirectX;
	// �f�o�C�X���\�[�X�֘A��ݒ肷��
	// �E�B���h�E��ݒ肷��
	m_deviceResources->SetWindow(window, width, height);
	// �f�o�C�X���\�[�X���쐬����
	m_deviceResources->CreateDeviceResources();
	// �f�o�C�X�ˑ����\�[�X���쐬����
	CreateDeviceDependentResources();
	// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
	m_deviceResources->CreateWindowSizeDependentResources();
	// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
	CreateWindowSizeDependentResources();
	// �f�o�C�X���擾����
	auto device = m_deviceResources->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾����
	auto context = m_deviceResources->GetD3DDeviceContext();
	// ���̓}�l�[�W�����쐬����
	m_inputManager = std::make_unique<mylib::InputManager>(window);
	// �R�����X�e�[�g���쐬����
	m_commonStates = std::make_unique<CommonStates>(device);
	// �f�o�b�O��������쐬����
	m_debugString = std::make_unique<mylib::DebugString>(
		device,		// �f�o�C�X
		context,	// �f�o�C�X�R���e�L�X�g
		L"Resources/Fonts/SegoeUI_18.spritefont"// �t�H���g�t�@�C���̃p�X
	);
	// �I�[�f�B�I�}�l�[�W���[���쐬����
	m_audioManager = std::make_unique<AudioManager>();
	// ���ʃ��\�[�X���쐬����
	m_pCommonResources = std::make_unique<CommonResources>();
	// ���f���}�l�[�W�����쐬����
	m_modelManager = std::make_unique<ModelManager>();
	// �e�N�X�`���}�l�[�W�����쐬����
	m_textureManager = std::make_unique<TextureManager>();
	//// ���f���}�l�[�W���̏�������ʃX���b�h�ōs��
	//std::thread modelInitializeThread([this, device]()
	//	{
	//		// ���f���}�l�[�W��������������
	//		m_modelManager->Initialize(m_deviceResources->GetD3DDevice());
	//	});
	//// �e�N�X�`���}�l�[�W���̏�������ʃX���b�h�ōs��
	//std::thread textureInitializeThread([this, device]()
	//	{
	//		// �e�N�X�`���}�l�[�W��������������
	//		m_textureManager->Initialize(m_deviceResources->GetD3DDevice());
	//	});
	//// ���f���}�l�[�W���̏���������������܂őҋ@����
	//modelInitializeThread.join();
	//// �e�N�X�`���}�l�[�W���̏���������������܂őҋ@����
	//textureInitializeThread.join();
	m_modelManager->Initialize(m_deviceResources->GetD3DDevice());// ���f���}�l�[�W��������������
	m_textureManager->Initialize(m_deviceResources->GetD3DDevice());// �e�N�X�`���}�l�[�W��������������
	// �V�[���֓n�����ʃ��\�[�X��ݒ肷��
	m_pCommonResources->Initialize(
		&m_timer,				// �^�C�}�[
		m_deviceResources.get(),// �f�o�C�X���\�[�X
		m_commonStates.get(),	// �R�����X�e�[�g
		m_debugString.get(),	// �f�o�b�O������
		m_inputManager.get(),	// ���̓}�l�[�W��
		m_audioManager.get(),	// �I�[�f�B�I�}�l�[�W��
		m_modelManager.get(),	// ���f���}�l�[�W��
		m_textureManager.get()	// �e�N�X�`���}�l�[�W�� 
	);
	// �V�[���}�l�[�W�����쐬����
	m_sceneManager = std::make_unique<SceneManager>();
	// �V�[���}�l�[�W��������������
	m_sceneManager->Initialize(m_pCommonResources.get());
	//�J�[�\����������悤�ɂ���
	ShowCursor(TRUE);
}

#pragma region Frame Update
/*
*	@brief �Q�[�����[�v�����s����
*	@detail �Q�[�����[�v�����s����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::Tick()
{
	// �^�C�}�[���X�V���AUpdate�֐����Ăяo��
	m_timer.Tick([&]() { Update(m_timer); });
	// �`�揈�����Ăяo��
	Render();
}
/*
*	@brief �Q�[���̍X�V����
*	@detail �Q�[���̍X�V�������s���֐�
*	@param timer �^�C�}�[
*	@return �Ȃ�
*/
void Game::Update(DX::StepTimer const& timer)
{
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(timer.GetElapsedSeconds());
	// ���̓}�l�[�W�����X�V����
	m_inputManager->Update();
	// ESC�L�[�ŃQ�[�����I������
	if (m_inputManager->GetKeyboardState().Escape)
	{
		ExitGame();// �Q�[�����I������
		return;
	}
	// �V�[���}�l�[�W�����X�V����
	m_sceneManager->Update(elapsedTime);
}
#pragma endregion

#pragma region Frame Render
/*
*	@brief �Q�[���̕`�揈��
*	@detail �Q�[���̕`�揈�����s���֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::Render()
{
	// �ŏ���Update�O�͉����`�悵�Ȃ�
	if (m_timer.GetFrameCount() == 0)
		return;
	Clear();// ��ʂ��N���A����
	// PIX�C�x���g���J�n����
	m_deviceResources->PIXBeginEvent(L"Render");
	// �f�o�C�X�R���e�L�X�g���擾����
	auto context = m_deviceResources->GetD3DDeviceContext();
	// �f�o�C�X�R���e�L�X�g�̖��g�p�x����}������
	UNREFERENCED_PARAMETER(context);
	// �V�[���}�l�[�W����`�悷��
	m_sceneManager->Render();
#ifdef _DEBUG
	// �f�o�b�O�������`�悷��
	m_debugString->Render(m_commonStates.get());
	// �f�o�b�O��������쐬����FFPS
	m_debugString->AddString("fps : %d", m_timer.GetFramesPerSecond());
#endif
	// PIX�C�x���g���I������
	m_deviceResources->PIXEndEvent();
	// �V�����t���[����\������
	m_deviceResources->Present();
}

/*
*	@brief ��ʂ��N���A����
*	@detail ��ʂ��N���A����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::Clear()
{
	using namespace DirectX;
	// PIX�C�x���g���J�n����
	m_deviceResources->PIXBeginEvent(L"Clear");
	// �f�o�C�X�R���e�L�X�g���擾����
	auto context = m_deviceResources->GetD3DDeviceContext();
	// �����_�[�^�[�Q�b�g�r���[���擾����
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	// �[�x�X�e���V���r���[���擾����
	auto depthStencil = m_deviceResources->GetDepthStencilView();
	// �����_�����O�^�[�Q�b�g�r���[���N���A����
	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	// �[�x�X�e���V���r���[���N���A����
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// �����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���r���[��ݒ肷��
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	// �X�N���[���r���[�|�[�g���擾����
	auto const viewport = m_deviceResources->GetScreenViewport();
	// �r���[�|�[�g��ݒ肷��
	context->RSSetViewports(1, &viewport);
	// PIX�C�x���g���I������
	m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
/*
*	@brief �Q�[�����A�N�e�B�u�����ꂽ�Ƃ��̏���
*	@detail �Q�[�����A�N�e�B�u�E�B���h�E�ɂȂ����Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnActivated() {/*do nothing*/ }
/*
*	@brief �Q�[������A�N�e�B�u�����ꂽ�Ƃ��̏���
*	@detail �Q�[������A�N�e�B�u�E�B���h�E�ɂȂ����Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDeactivated() {/*do nothing*/ }
/*
*	@brief �Q�[�����ꎞ��~���ꂽ�Ƃ��̏���
*	@detail �Q�[�����ꎞ��~���ꂽ�Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnSuspending() {/*do nothing*/ }
/*
*	@brief �Q�[�����ĊJ���ꂽ�Ƃ��̏���
*	@detail �Q�[�����ĊJ���ꂽ�Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnResuming()
{
	// �^�C�}�[�����Z�b�g����
	m_timer.ResetElapsedTime();
}
/*
*	@brief �E�B���h�E���ړ����ꂽ�Ƃ��̏���
*	@detail �E�B���h�E���ړ����ꂽ�Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnWindowMoved()
{
	// �E�B���h�E�̃T�C�Y���擾����
	auto const r = m_deviceResources->GetOutputSize();
	// �E�B���h�E�T�C�Y���ύX���ꂽ���Ƃ�ʒm����
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
	// �t���X�N���[�������ׂ�
	BOOL fullscreen = FALSE;
	m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);
	// �t���X�N���[����Ԃ��擾����
	if (m_fullscreen != fullscreen)// �t���X�N���[������������Ă��܂������̏���
	{
		// �t���X�N���[����Ԃ��X�V����
		m_fullscreen = fullscreen;
		// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
		m_deviceResources->CreateWindowSizeDependentResources();
	}
}
/*
*	@brief �f�B�X�v���C�̕ύX���������Ƃ��̏���
*	@detail �f�B�X�v���C�̕ύX���������Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDisplayChange()
{
	// �J���[�X�y�[�X���X�V����
	m_deviceResources->UpdateColorSpace();
}
/*
*	@brief �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̏���
*	@detail �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̏���
*	@param width �V�����E�B���h�E�̕�
*	@param height �V�����E�B���h�E�̍���
*	@return �Ȃ�
*/
void Game::OnWindowSizeChanged(int width, int height)
{
	// �E�B���h�E�T�C�Y���ύX����Ă��Ȃ��ꍇ�͉������Ȃ�
	if (!m_deviceResources->WindowSizeChanged(width, height))return;
	// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
	CreateWindowSizeDependentResources();
}

/*
*	@brief �f�t�H���g�̃E�B���h�E�T�C�Y���擾����
*	@detail �f�t�H���g�̃E�B���h�E�T�C�Y���擾����֐�
*	@param width �f�t�H���g�̃E�B���h�E�̕�
*	@param height �f�t�H���g�̃E�B���h�E�̍���
*	@return �Ȃ�
*/
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	// �f�t�H���g�̃E�B���h�E�̕���ݒ�
	width = Screen::WIDTH;
	// �f�t�H���g�̃E�B���h�E�̍�����ݒ�
	height = Screen::HEIGHT;
}
#pragma endregion

#pragma region Direct3D Resources
/*
*	@brief �f�o�C�X�ˑ����\�[�X���쐬����
*	@detail �f�o�C�X�ˑ����\�[�X���쐬����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::CreateDeviceDependentResources()
{
	// �f�o�C�X���擾����
	auto device = m_deviceResources->GetD3DDevice();
	// �f�o�C�X�̖��g�p�x����}������
	UNREFERENCED_PARAMETER(device);
}
/*
*	@brief �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
*	@detail �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::CreateWindowSizeDependentResources() {/*do nothing*/ }
/*
*	@brief �f�o�C�X������ꂽ�Ƃ��̏���
*	@detail �f�o�C�X������ꂽ�Ƃ��̏������s���֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDeviceLost() {/*do nothing*/ }
/*
*	@brief �f�o�C�X���������ꂽ�Ƃ��̏���
*	@detail �f�o�C�X�E�E�B���h�E�T�C�Y�ˑ����\�[�X���č쐬����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDeviceRestored()
{
	// �f�o�C�X�ˑ����\�[�X���쐬
	CreateDeviceDependentResources();
	// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬
	CreateWindowSizeDependentResources();
}
/*
*	@brief �t���X�N���[����Ԃ�ݒ肷��
*	@detail �t���X�N���[����Ԃ�ݒ肷��֐�
*	@param value �t���X�N���[����ԁiTRUE: �t���X�N���[���AFALSE: �E�B���h�E���[�h�j
*	@return �Ȃ�
*/
void Game::SetFullscreenState(BOOL value)
{
	// �t���X�N���[����Ԃ��X�V
	m_fullscreen = value;
	// �t���X�N���[����Ԃ�ݒ�
	m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
	// �t���X�N���[����Ԃɉ����ăE�B���h�E�T�C�Y�ˑ����\�[�X���쐬
	if (value) m_deviceResources->CreateWindowSizeDependentResources();
}
#pragma endregion
