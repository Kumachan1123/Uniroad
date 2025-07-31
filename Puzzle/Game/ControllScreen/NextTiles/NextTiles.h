/*
*	@file	NextTiles.h
*	@brief	���̃^�C���N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <string>
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
#include "Game/ControllScreen/Canvas/Canvas.h"
#include "Game/Interface/IUI/IUI.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/Mouse/Mouse.h"
#include "Game/MainScreen/Tiles/TileDatas/TileDatas.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/ControllScreen/TileConnectionTable/TileConnectionTable.h"

// �O���錾
class CommonResources;

// ���̃^�C���N���X
class NextTiles : public IUI
{
private:
	// �^�C������ێ�����\����
	struct TileInfo
	{
		std::string textureKey; // �e�N�X�`���̃L�[
		std::unique_ptr<Canvas> canvas; // �L�����o�X�I�u�W�F�N�g
	};

public:
	// �A�N�Z�T
	// UI�Ƀq�b�g�������ǂ����擾
	bool GetIsHit()const { return m_hit; }
	// UI�Ƀq�b�g�������ǂ����ݒ�
	void SetIsHit(bool hit) { m_hit = hit; }
	// CSV�}�b�v��ݒ�
	void SetCSVMap(CSVMap* pCSVMap) { m_pCSVMap = pCSVMap; }
	// �}�E�X�|�C���^�[��ݒ�
	void SetMouse(MyMouse* pMouse) { m_pMouse = pMouse; }
	// �r���[�|�[�g��ݒ�
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }
	// �~�j�L�����̐i�s����(���x�x�N�g��)��ݒ�
	void SetMiniCharacterVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_miniCharacterVelocity = velocity; }
	// �~�j�L������������^�C���̖��O��ݒ�
	void SetMiniCharacterTileName(const std::string& tileName) { m_miniCharacterTileName = tileName; }
public:
	// public�֐�
	// �R���X�g���N�^
	NextTiles();
	// �f�X�g���N�^
	~NextTiles();
	// ������
	void Initialize(CommonResources* resources, int width, int height) override;
	// �X�V
	void Update(const float elapsedTime) override;
	// �`��
	void Render() override;
	// UI�ǉ�
	void Add(
		const std::string& key,						// �e�N�X�`���̃L�[
		const DirectX::SimpleMath::Vector2& position,	// �ʒu
		const DirectX::SimpleMath::Vector2& scale,	// �X�P�[��
		KumachiLib::ANCHOR anchor,					// �A���J�[
		UIType type) override;						// UI�̎��
	// �ݒu�ς݃^�C���̂ݕ`��
	void DrawPlacedTiles() const;
private:
	// private�֐�
	// �X�V���ɒ���I��UI��ǉ�����
	void AddNextTiles();
	// �V�����p�l����z�u
	void AddToPanel();
	// ���̈ʒu�Ƀ^�C����߂�
	void ResetTilePosition();
	// ������^�C���Ɛi�s�������玟�ɐi�߂�^�C�����擾
	std::vector<std::string> GetAvailableNextTiles(const std::string& currentTileName, const DirectX::SimpleMath::Vector3& velocity) const;
	// �^�C���̐ڑ��\�ȃ^�C�����擾
	Direction GetDirectionFromVelocity(const DirectX::SimpleMath::Vector3& velocity) const;
	// �ڑ��\�ȃ^�C���𒊑I
	std::string GetRandomConnectableTile(const std::vector<std::string>& availableTiles, const std::string& previousTileName) const;
private:
	// private�֐�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �ݒu�\�^�C��
	std::vector<TileInfo> m_pTile;
	// �w�i
	std::vector<TileInfo> m_pBack;
	// �ݒu�ς݃^�C��
	std::vector<TileInfo> m_pPlacedTile;
	// CSV�}�b�v
	CSVMap* m_pCSVMap;
	// �}�E�X�̃|�C���^�[
	MyMouse* m_pMouse;
	// �e�I���\UI�̏����ʒu���X�g
	std::vector<DirectX::SimpleMath::Vector2> m_initialPositions;
	// �h���b�O����UI�C���f�b�N�X
	int m_draggingIndex;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// ����
	float m_time;
	// �q�b�g�t���O
	bool m_hit;
	// �q�b�g�t���O��������UI�̃C���f�b�N�X
	int m_menuIndex;
	// ����p�r���[�|�[�g
	D3D11_VIEWPORT m_viewPortControll;
	// �g���^�C���̎���
	std::vector<std::string> m_tilesDictionary;
	// �~�j�L�����̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_miniCharacterVelocity;
	// �~�j�L������������^�C���̖��O
	std::string m_miniCharacterTileName;
	// �Ō�ɒu�����^�C���̖��O
	std::string m_lastPlacedTileName;
	// �O�񐶐������^�C����
	std::string m_previousTileName;
};