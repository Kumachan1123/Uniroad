#pragma once
// // �W�����C�u����
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
//// ����w�b�_�[�t�@�C��
//#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
class TileBase;
// �^�C���̏��\����
struct TileInfo
{
	std::string modelName;                   // �g�p���郂�f����
	bool hasCollision = false;              // �����蔻�������
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f }; // �X�P�[�����O
};
// �^�C���̃����_�����O�f�[�^�\����
struct TileRenderData
{
	DirectX::Model* model;
	DirectX::SimpleMath::Matrix world;
};
// �}�b�v��1�}�X���̏��
struct MapTileData
{
	TileInfo tileInfo;               // ���̏ꏊ�̃^�C����ށiBlock�AStart�AGoal�Ȃǁj
	DirectX::SimpleMath::Vector3 pos; // ���̃}�X�̃��[���h���W
	bool hasCollision = false;               // �����蔻�肠�邩
	std::unique_ptr<TileBase> tileBasePtr = nullptr; // �^�C���̃x�[�X�N���X�ւ̃|�C���^

};