/*
*	@file CSVMap.h
*	@brief CSV�`���̃}�b�v��ǂݍ��݁A�`�悷��N���X
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
#include "Game/MainScreen/Tiles/TileDatas/TileDatas.h"
#include "Game/MainScreen/Tiles/TileFactory/TileFactory.h"

// �O���錾
class CommonResources;

// CSV�`���̃}�b�v��ǂݍ���
class CSVMap
{
public:
	// �A�N�Z�T
	// �w��ʒu�̃^�C�������擾����(�s, ��)
	const MapTileData& GetTileData(int row, int col) const;
	// �w����W�̃^�C�������擾����(���[���h���W)
	const MapTileData& GetTileData(const DirectX::SimpleMath::Vector3& pos) const;
	// �w����W�̍s�ԍ����擾����
	int GetRowFromPosition(const DirectX::SimpleMath::Vector3& pos) const;
	// �w����W�̗�ԍ����擾����
	int GetColFromPosition(const DirectX::SimpleMath::Vector3& pos) const;
	// �}�b�v�̍ő�񐔂ƍs�����擾����
	const int GetMaxCol() const { return MAXCOL; }// ��
	const int GetMaxRow() const { return MAXRAW; }// �s��
	// �w�肵���ʒu�Ɏw�肵�����f����z�u����
	void SetTileModel(int row, int col, const std::string& modelName);
	// �X�^�[�g�n�_��Ԃ�
	const MapTileData& GetStart() const;
	// �}�b�v�f�[�^��n��
	const std::vector<std::vector<MapTileData>>& GetMapData() const { return m_mapData; }
public:
	// public�֐�
	// �R���X�g���N�^
	CSVMap(CommonResources* resources);
	// �f�X�g���N�^
	~CSVMap();
	// CSV�`���̃}�b�v��ǂݍ���
	void LoadMap(const std::string& filePath);
	// �����蔻��`��
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
private:
	// private�֐�
	// ����������������
	void InitializeTileDictionary();
	// �[�x�X�e���V���̐ݒ�
	void CreateDepthStencilBuffer(ID3D11Device* pDevice);
private:
	// private�萔
	// ��O�̃^�C���f�[�^
	const MapTileData m_outOfMapData = { {""},DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f) };
	//�}�b�v
	const int MAXCOL = 5;
	const int MAXRAW = 5;
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �^�C���̎���
	std::unordered_map<std::string, TileInfo> m_tileDictionary;
	// �^�C���̃����_�����O�f�[�^
	std::vector<TileRenderData> m_tiles;
	// �}�b�v�f�[�^
	std::vector<std::vector<MapTileData>> m_mapData;
	// �[�x�X�e���V��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
};