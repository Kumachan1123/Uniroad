/*
*	@file Panel.h
*	@brief �p�l���N���X
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
#include "Game/Interface/IGameUI.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/CSVItem/CSVItem.h"
#include "Game/MainScreen/Tiles/TileDatas/TileDatas.h"
#include "Game/MainScreen/Items/ItemDatas/ItemDatas.h"
#include "Game/Mouse/Mouse.h"

// �O���錾
class CommonResources;
class CSVItem;

// �p�l���N���X
class Panel : public IGameUI
{
public:
	// �\����
	// �A�C�e���̏��
	struct ItemInfo
	{
		int row;		// �s�ԍ�
		int col;		// ��ԍ�
		bool isCollected;	// ���W�ς݂��ǂ���
	};
public:
	// �A�N�Z�T
	// UI�Ƀq�b�g�������ǂ����擾
	bool GetIsHit()const { return m_hit; }
	// UI�Ƀq�b�g�������ǂ����ݒ�
	void SetIsHit(bool hit) { m_hit = hit; }
	// CSV�}�b�v��ݒ�
	void SetCSVMap(CSVMap* pCSVMap) { m_pCSVMap = pCSVMap; }
	// CSV�A�C�e����ݒ�
	void SetCSVItem(CSVItem* pCSVItem) { m_pCSVItem = pCSVItem; }
	// �}�E�X�|�C���^�[��ݒ�
	void SetMouse(MyMouse* pMouse) { m_pMouse = pMouse; }
	// �r���[�|�[�g��ݒ�
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }
	// �v���C���[�̍��W��ݒ�
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position) { m_playerPosition = position; }

public:
	// public�֐�
	// �R���X�g���N�^
	Panel(int mapSizeX, int mapSizeY);
	// �f�X�g���N�^
	~Panel();
	// ������
	void Initialize(CommonResources* resources, int width, int height) override;
	// �X�V
	void Update(const float elapsedTime) override;
	// �v���C���[�̃A�C�R���̍X�V
	void UpdatePlayerIcons(const float elapsedTime);
	// �`��
	void Render() override;
	// UI�ǉ�
	void Add(
		const std::string& key,						// �e�N�X�`���̃L�[
		const DirectX::SimpleMath::Vector2& position,	// �ʒu
		const DirectX::SimpleMath::Vector2& scale,	// �X�P�[��
		KumachiLib::ANCHOR anchor,					// �A���J�[
		UIType type) override;						// UI�̎��

	// �^�C�����̕`��
	void DrawTiles();
	// �A�C�e�����̕`��
	void DrawItems();

private:
	// private�֐�
	// �A�C�e���̔z�u
	void PlaceItems(const MapItemData& itemData, int row, int col, const DirectX::SimpleMath::Vector2& pos);
	// �v���C���[�̔z�u
	void PlacePlayer(const MapTileData& tileData, int row, int col, const DirectX::SimpleMath::Vector2& pos);
private:
	// private�萔
	// 1�^�C���̕�
	static const float TILE_SIZE;
	// �^�C���̐��i0�`5�j
	static const int TILE_COUNT;
	// �^�C���̍��W�̕␳�l
	static const float TILE_POSITION_CORRECTION;
	// �^�C���̐��i�␳�l�l���j
	static const float TILE_COUNT_CORRECTED;
	// 3D��ԏ�̃v���C���[�̍��W�̕␳�l
	static const float PLAYER_POSITION_CORRECTION;
	// 3D��ԏ�̃^�C���̕��i�␳�l�l���j
	static const float TILE_SIZE_3D;
	// �`��I�t�Z�b�gX
	static const float DRAW_OFFSET_X;
	// �`��I�t�Z�b�gY
	static const float DRAW_OFFSET_Y;

private:
	// �����o�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �^�C��
	std::vector<std::unique_ptr<Canvas>> m_pTiles;
	// �A�C�e��
	std::vector<std::pair<std::unique_ptr<Canvas>, ItemInfo>> m_pItems;
	// �v���C���[�̃A�C�R��
	std::vector<std::unique_ptr<Canvas>> m_pPlayerIcons;
	// �}�E�X�̃|�C���^�[
	MyMouse* m_pMouse;
	// �v���C���[�̍��W
	DirectX::SimpleMath::Vector3 m_playerPosition;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// ����
	float m_time;
	// �q�b�g�t���O
	bool m_hit;
	// �q�b�g�t���O��������UI�̃C���f�b�N�X
	int m_menuIndex;
	// �}�b�v�T�C�Y
	int m_mapSizeX, m_mapSizeY;
	// CSV�}�b�v
	CSVMap* m_pCSVMap;
	// CSV�A�C�e��
	CSVItem* m_pCSVItem;
	// ����p�r���[�|�[�g
	D3D11_VIEWPORT m_viewPortControll;
	// �s�i�ۑ��p�j
	int m_row;
	// ��i�ۑ��p�j
	int m_col;
};