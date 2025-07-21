/*
*	@file CSVItem.h
*	@brief CSV�`���̃A�C�e�����z��ǂݍ��݁A�`�悷��N���X
*/
#pragma once
// �W�����C�u����
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cassert>
// DirectX�֘A
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/MainScreen/Items/ItemDatas/ItemDatas.h"
#include "Game/MainScreen/Items/ItemFactory/ItemFactory.h"
// �O���錾
class CommonResources;

// CSV�`���̃}�b�v��ǂݍ���
class CSVItem
{
public:
	// �A�N�Z�T
	// �w��ʒu�̃^�C���̃A�C�e�������擾����(�s, ��)
	const MapItemData& GetItemData(int row, int col) const;
	// �w����W�̃^�C���̃A�C�e�������擾����(���[���h���W)
	const MapItemData& GetItemData(const DirectX::SimpleMath::Vector3& pos) const;
	// �w����W�̃^�C���̃A�C�e����������(�l����ɌĂ�)(�s, ��)
	void  RemoveItem(int row, int col);


	// �}�b�v�̍ő�񐔂ƍs�����擾����
	const int GetMaxCol() const { return MAXCOL; }// ��
	const int GetMaxRow() const { return MAXRAW; }// �s��
	// �l�����ꂽ���������Z����
	void CountMedals() { m_collectedMedals++; }
	// �l�����ꂽ���_���̖������擾����
	int GetCollectedMedals() const { return m_collectedMedals; }
public:
	// public�֐�
	// �R���X�g���N�^
	CSVItem(CommonResources* resources);
	// �f�X�g���N�^
	~CSVItem();
	// CSV�`���̃A�C�e���z�u��ǂݍ���
	void LoadItem(const std::string& filePath);
	// �X�V����
	void Update(float elapsedTime);
	// �����蔻��`��
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
private:
	// private�֐�
	// ����������������
	void InitializeTileDictionary();

private:
	CommonResources* m_pCommonResources;
	//�}�b�v
	// �ő�s
	const int MAXRAW = 5;
	// �ő��
	const int MAXCOL = 5;
	// �^�C���̎���
	std::unordered_map<std::string, ItemInfo> m_tileDictionary;
	// �^�C���̃����_�����O�f�[�^
	std::vector<ItemRenderData> m_tiles;
	// �}�b�v�f�[�^
	std::vector<std::vector<MapItemData>> m_mapItemData;
	// �A�C�e���z��
	std::vector<std::unique_ptr<ItemBase>> m_itemArray;
	// ����
	float m_time;
	// �l�����ꂽ���_��
	int m_collectedMedals;
};