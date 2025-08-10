/*
*	@file	SpeedUpButton.h
*	@brief	�X�s�[�h�A�b�v�{�^���N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/MouseClick/MouseClick.h"
#include "Game/Interface/IButton.h"
#include "KumachiLib/Button/Button.h"

// �O���錾
class CommonResources;

// �X�s�[�h�A�b�v�{�^���N���X
class SpeedUpButton : public IButton
{
public:
	// �A�N�Z�T
	// �X�s�[�h�A�b�v�{�^���������ꂽ���ǂ������擾
	bool IsPressed() const override { return m_isPressed; }
	// �X�s�[�h�A�b�v�{�^���������ꂽ���ǂ�����ݒ�
	void SetPressed(bool isPressed) override { m_isPressed = isPressed; }
	// ���W���擾
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ���W��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// �T�C�Y���擾
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// �T�C�Y��ݒ�
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public�֐�
	// �R���X�g���N�^
	SpeedUpButton();
	// �f�X�g���N�^
	~SpeedUpButton();
	// ������ 
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V����
	void Update(float elapsedTime)override;
	// �`�悷��
	void Render()override;
private:
	// private�萔
	// �\���ʒu�i����j
	static const DirectX::SimpleMath::Vector2 POSITION;
	// �\���T�C�Y
	static const DirectX::SimpleMath::Vector2 SIZE;
private:
	// private�ϐ�
	// ���ʃ��\�[�X 
	CommonResources* m_pCommonResources;
	// �X�s�[�h�A�b�v�{�^��
	std::unique_ptr<Button> m_pButton;
	// �{�^���̋�`
	Rect m_buttonRect;
	// ���W
	DirectX::SimpleMath::Vector2 m_position;
	// �T�C�Y
	DirectX::SimpleMath::Vector2 m_size;
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// �����蔻��
	bool m_isHit;
	// �{�^���������ꂽ�t���O
	bool m_isPressed;
};
