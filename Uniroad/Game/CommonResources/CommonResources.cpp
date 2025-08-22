/*
*	@file	CommonResources.cpp
*	@brief	�V�[���֓n���A�Q�[�����Ŏg�p���鋤�ʃ��\�[�X
*/
#include <pch.h>
#include "CommonResources.h"
/*
*	@brief	�R���X�g���N�^
*	@details ����������nullptr��ݒ肷��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
CommonResources::CommonResources()
	: m_stepTimer{ nullptr }		// �^�C�}�[
	, m_deviceResources{ nullptr }	// �f�o�C�X���\�[�X
	, m_commonStates{ nullptr }		// �R�����X�e�[�g
	, m_debugString{ nullptr }		// �f�o�b�O������
	, m_inputManager{ nullptr }		// ���̓}�l�[�W��
	, m_audioManager{ nullptr }		// �I�[�f�B�I�}�l�[�W��
	, m_modelManager{ nullptr }		// ���f���}�l�[�W��
	, m_textureManager{ nullptr }	// �e�N�X�`���}�l�[�W��
{
}
/*
*	@brief	����������
*	@details main.cpp�ō쐬�������\�[�X���󂯎��
*	@param	timer			�󂯓n������^�C�}�[
*	@param	dr				�󂯓n������f�o�C�X���\�[�X
*	@param	commonStates	�󂯓n������R�����X�e�[�g
*	@param	debugString		�󂯓n������f�o�b�O������
*	@param	inputManager	�󂯓n��������̓}�l�[�W��
*	@param	audioManager	�󂯓n������I�[�f�B�I�}�l�[�W��
*	@param	modelManager	�󂯓n�����郂�f���}�l�[�W��
*	@param	textureManager	�󂯓n������e�N�X�`���}�l�[�W��
*	@return	�Ȃ�
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
	assert(timer);			// �^�C�}�[��nullptr�łȂ����Ƃ��m�F
	assert(dr);				// �f�o�C�X���\�[�X��nullptr�łȂ����Ƃ��m�F
	assert(commonStates);	// �R�����X�e�[�g��nullptr�łȂ����Ƃ��m�F
	assert(debugString);	// �f�o�b�O������nullptr�łȂ����Ƃ��m�F
	assert(inputManager);	// ���̓}�l�[�W����nullptr�łȂ����Ƃ��m�F
	assert(audioManager);	// �I�[�f�B�I�}�l�[�W����nullptr�łȂ����Ƃ��m�F
	assert(modelManager);	// ���f���}�l�[�W����nullptr�łȂ����Ƃ��m�F
	assert(textureManager);	// �e�N�X�`���}�l�[�W����nullptr�łȂ����Ƃ��m�F

	m_stepTimer = timer;			// �^�C�}�[��ݒ肷��
	m_deviceResources = dr;			// �f�o�C�X���\�[�X��ݒ肷��
	m_commonStates = commonStates;	// �R�����X�e�[�g��ݒ肷��
	m_debugString = debugString;	// �f�o�b�O�������ݒ肷��
	m_inputManager = inputManager;	// ���̓}�l�[�W����ݒ肷��
	m_audioManager = audioManager;	// �I�[�f�B�I�}�l�[�W����ݒ肷��
	m_modelManager = modelManager;	// ���f���}�l�[�W����ݒ肷��
	m_textureManager = textureManager; // �e�N�X�`���}�l�[�W����ݒ肷��
}
