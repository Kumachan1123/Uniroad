/*
	@file	CommonResources.h
	@brief	�V�[���֓n���A�Q�[�����Ŏg�p���鋤�ʃ��\�[�X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/AudioManager/AudioManager.h"
#include "KumachiLib/ModelManager/ModelManager.h"
#include "KumachiLib/TextureManager/TextureManager.h"

// �O���錾
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

// ���ʃ��\�[�X
class CommonResources
{
public:
	// �A�N�Z�T
	// �^�C�}�[���擾����
	DX::StepTimer* GetStepTimer() const { return m_stepTimer; }
	// �f�o�C�X���\�[�X���擾����
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }
	// �R�����X�e�[�g���擾����
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }
	// �f�o�b�O��������擾����
	mylib::DebugString* GetDebugString() const { return m_debugString; }
	// ���̓}�l�[�W�����擾����
	mylib::InputManager* GetInputManager() const { return m_inputManager; }
	// �I�[�f�B�I�}�l�[�W�����擾����
	AudioManager* GetAudioManager() const { return m_audioManager; }
	// ���f���}�l�[�W�����擾����
	ModelManager* GetModelManager() const { return m_modelManager; }
	// �e�N�X�`���}�l�[�W�����擾����
	TextureManager* GetTextureManager() const { return m_textureManager; }
public:
	// public���\�b�h
	// �R���X�g���N�^
	CommonResources();
	// �f�X�g���N�^ default�Œ�`
	~CommonResources() = default;
	// ����������
	void Initialize(
		DX::StepTimer* timer,// �󂯓n������^�C�}�[
		DX::DeviceResources* dr,// �󂯓n������f�o�C�X���\�[�X
		DirectX::CommonStates* commonStates,// �󂯓n������R�����X�e�[�g
		mylib::DebugString* debugString,// �󂯓n������f�o�b�O������
		mylib::InputManager* inputManager,// �󂯓n��������̓}�l�[�W��
		AudioManager* audioManager,// �󂯓n������I�[�f�B�I�}�l�[�W��
		ModelManager* modelManager,// �󂯓n�����郂�f���}�l�[�W��
		TextureManager* textureManager  // �󂯓n������e�N�X�`���}�l�[�W��
	);
private:
	// private�����o�ϐ�
	// �󂯓n�����郊�\�[�X�ꗗ
	// �^�C�}�[
	DX::StepTimer* m_stepTimer;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	// �R�����X�e�[�g
	DirectX::CommonStates* m_commonStates;
	// �f�o�b�O������
	mylib::DebugString* m_debugString;
	// ���̓}�l�[�W��
	mylib::InputManager* m_inputManager;
	// �I�[�f�B�I�}�l�[�W��
	AudioManager* m_audioManager;
	// ���f���}�l�[�W��
	ModelManager* m_modelManager;
	// �e�N�X�`���}�l�[�W��
	TextureManager* m_textureManager;
};
