/*
*	@file Game.h
*	@brief �Q�[���̃��C���N���X
*/
#pragma once
#ifndef GAME_DEFINED
#define GAME_DEFINED
// �W�����C�u����
#include <thread>
// DirectX
#include <DeviceResources.h>
#include "StepTimer.h"
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/nlohmann/json.hpp>
// �Q�[���֘A
#include "Game/CommonResources/CommonResources.h"
#include "Game/SceneManager/IScene.h"
#include "Game/SceneManager/SceneManager.h"
#include "KumachiLib/AudioManager/AudioManager.h"
#include "KumachiLib/ModelManager/ModelManager.h"
#include "KumachiLib/TextureManager/TextureManager.h"
#include "Game/Screen/Screen.h"

// �Q�[���N���X
class Game final : public DX::IDeviceNotify
{
public:
	// public�֐�
	// �R���X�g���N�^
	Game() noexcept(false);
	// �f�X�g���N�^
	~Game() = default;

	// ���[�u�R���X�g���N�^
	Game(Game&&) = default;
	// ���[�u������Z�q
	Game& operator= (Game&&) = default;

	// �R�s�[�R���X�g���N�^
	Game(Game const&) = delete;
	// �R�s�[������Z�q
	Game& operator= (Game const&) = delete;

	// ������
	void Initialize(HWND window, int width, int height);
	// �Q�[�����[�v�����s
	void Tick();
	// �f�o�C�X������ꂽ�Ƃ��̏���
	void OnDeviceLost() override;
	// �f�o�C�X���������ꂽ�Ƃ��̏���
	void OnDeviceRestored() override;
	// ���b�Z�[�W
	// �A�N�e�B�u�����ꂽ�Ƃ��̏���
	void OnActivated();
	// ��A�N�e�B�u�����ꂽ�Ƃ��̏���
	void OnDeactivated();
	// �ꎞ��~���ꂽ�Ƃ��̏���
	void OnSuspending();
	// �ĊJ���ꂽ�Ƃ��̏���
	void OnResuming();
	// �E�B���h�E���ړ����ꂽ�Ƃ��̏���
	void OnWindowMoved();
	// �f�B�X�v���C�̕ύX���������Ƃ��̏���
	void OnDisplayChange();
	// �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̏���
	void OnWindowSizeChanged(int width, int height);
	// �f�t�H���g�̃E�B���h�E�T�C�Y���擾
	void GetDefaultSize(int& width, int& height) const noexcept;
	// �t���X�N���[����Ԃ�ݒ�
	void SetFullscreenState(BOOL value);
private:
	// private�֐�
	// �X�V����
	void Update(DX::StepTimer const& timer);
	// �`�揈��
	void Render();
	// ��ʂ��N���A
	void Clear();
	// �f�o�C�X�ˑ����\�[�X�̍쐬
	void CreateDeviceDependentResources();
	// �E�B���h�E�T�C�Y�ˑ����\�[�X�̍쐬
	void CreateWindowSizeDependentResources();
private:
	// private�ϐ�
	// �f�o�C�X���\�[�X
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;
	// �^�C�}�[
	DX::StepTimer                           m_timer;
	// �t���X�N���[�����
	BOOL m_fullscreen;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>  m_commonStates;
	// �R�������\�[�X
	std::unique_ptr<CommonResources>        m_pCommonResources;
	// �f�o�b�O������
	std::unique_ptr<mylib::DebugString>     m_debugString;
	// ���̓}�l�[�W��
	std::unique_ptr<mylib::InputManager>    m_inputManager;
	// �V�[���}�l�[�W��
	std::unique_ptr<SceneManager>           m_sceneManager;
	// �I�[�f�B�I�}�l�[�W��
	std::unique_ptr<AudioManager>           m_audioManager;
	// ���f���}�l�[�W��
	std::unique_ptr<ModelManager>           m_modelManager;
	// �e�N�X�`���}�l�[�W��
	std::unique_ptr<TextureManager>         m_textureManager;
};
#endif // GAME_DEFINED
