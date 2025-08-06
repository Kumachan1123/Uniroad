/*
*	@file Mouse.h
*	@brief ����ȃr���[�|�[�g�ȉ�ʂŎg���}�E�X�N���X
*/
#pragma once
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"

// �O���錾
class CommonResources;

// �}�E�X�N���X
class MyMouse
{
public:
	// �A�N�Z�T
	// �}�E�X�̈ʒu���擾
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// �}�E�X�̈ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	// �}�E�X������p�l���̍��W���擾
	DirectX::SimpleMath::Vector2 GetPanelPosition() const { return m_panelPosition; }
	// �}�E�X������p�l���̍��W��ݒ�
	void SetPanelPosition(const DirectX::SimpleMath::Vector2& position) { m_panelPosition = position; }
	// �V�����^�C���̌��̍��W���擾
	DirectX::SimpleMath::Vector2 GetNewTilePosition() const { return m_newTilePosition; }
	// �V�����^�C���̌��̍��W��ݒ�
	void SetNewTilePosition(const DirectX::SimpleMath::Vector2& position) { m_newTilePosition = position; }
	// �_���𑜓x��̃r���[�|�[�g����X���擾
	float GetVpLeftUI() const { return m_vp_left_UI; }
	// �_���𑜓x��̃r���[�|�[�g����Y���擾
	float GetVpTopUI() const { return m_vp_top_UI; }
	// �_���𑜓x��̃r���[�|�[�g�����擾
	float GetVpWidthUI() const { return m_vp_width_UI; }
	// �_���𑜓x��̃r���[�|�[�g�������擾
	float GetVpHeightUI() const { return m_vp_height_UI; }
	// �r���[�|�[�g��ݒ�
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }
	// �}�E�X�h���b�O�t���O���擾
	bool IsMouseDrag() const { return m_isMouseDrag; }
	// �}�E�X�h���b�O�t���O��ݒ�
	void SetMouseDrag(bool isMouseDrag) { m_isMouseDrag = isMouseDrag; }
	// �������Ă���p�l���̔ԍ����擾
	int GetHitPanelIndex() const { return m_hitPanelIndex; }
	// �������Ă���p�l���̔ԍ���ݒ�
	void SetHitPanelIndex(int index) { m_hitPanelIndex = index; }
	// �������Ă���p�l���̍s�ԍ����擾
	int GetHitPanelRowIndex() const { return m_hitPanelRowIndex; }
	// �������Ă���p�l���̍s�ԍ���ݒ�
	void SetHitPanelRowIndex(int rowIndex) { m_hitPanelRowIndex = rowIndex; }
	// �������Ă���p�l���̗�ԍ����擾
	int GetHitPanelColIndex() const { return m_hitPanelColIndex; }
	// �������Ă���p�l���̗�ԍ���ݒ�
	void SetHitPanelColIndex(int colIndex) { m_hitPanelColIndex = colIndex; }
	// �������Ă���V�����o�Ă����^�C���̔ԍ����擾
	int GetHitNewTileIndex() const { return m_hitNewTileIndex; }
	// �������Ă���V�����o�Ă����^�C���̔ԍ���ݒ�
	void SetHitNewTileIndex(int index) { m_hitNewTileIndex = index; }
	// �q�b�g�t���O���擾
	bool IsHit() const { return m_hit; }
	// �q�b�g�t���O��ݒ�
	void SetHit(bool hit) { m_hit = hit; }
	// �q�b�g�t���O(�V�����o�Ă����^�C��)���擾
	bool IsHitNewTile() const { return m_hitNewTile; }
	// �q�b�g�t���O(�V�����o�Ă����^�C��)��ݒ�
	void SetHitNewTile(bool hitNewTile) { m_hitNewTile = hitNewTile; }
	// ���������ꂽ���ǂ������擾
	bool IsLeftReleased() const { return m_leftReleased; }
	// ���������ꂽ���ǂ�����ݒ�
	void SetLeftReleased(bool leftReleased) { m_leftReleased = leftReleased; }

public:
	// public�֐�
	// �R���X�g���N�^
	MyMouse();
	// �f�X�g���N�^
	~MyMouse();
	// ������
	void Initialize(CommonResources* resources);
	// �X�V
	void Update(const float elapsedTime);
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �}�E�X�̈ʒu
	DirectX::SimpleMath::Vector2 m_position;
	// �}�E�X������p�l���̍��W
	DirectX::SimpleMath::Vector2 m_panelPosition;
	// �V�����^�C���̌��̍��W�i���쒼�O�̈ʒu�j
	DirectX::SimpleMath::Vector2 m_newTilePosition;
	// ����p�r���[�|�[�g
	D3D11_VIEWPORT m_viewPortControll;
	// �E�B���h�E�̕�
	float m_renderWidth;
	// �E�B���h�E�̍���
	float m_renderHeight;
	// �_���𑜓x��̃r���[�|�[�g����X
	float m_vp_left_UI;
	// �_���𑜓x��̃r���[�|�[�g����Y
	float m_vp_top_UI;
	// �_���𑜓x��̃r���[�|�[�g��
	float m_vp_width_UI;
	// �_���𑜓x��̃r���[�|�[�g����
	float m_vp_height_UI;
	// �}�E�X�h���b�O�t���O
	bool m_isMouseDrag;
	// �������Ă���p�l���̔ԍ�
	int m_hitPanelIndex;
	// �������Ă���p�l���̍s�ԍ�
	int m_hitPanelRowIndex;
	// �������Ă���p�l���̗�ԍ�
	int m_hitPanelColIndex;
	// �������Ă���V�����o�Ă����^�C���̔ԍ�
	int m_hitNewTileIndex;
	// �q�b�g�t���O(�p�l��)
	bool m_hit;
	// �q�b�g�t���O(�V�����o�Ă����^�C��)
	bool m_hitNewTile;
	// �O��̍��{�^���̏��
	bool m_prevLeftButton;
	// ���{�^���������ꂽ���ǂ���
	bool m_leftReleased;
};