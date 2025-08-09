#pragma once
// // �W�����C�u����
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>

// �O���錾
class ItemBase;
// �A�C�e���̏��\����
struct  ItemInfo
{
	std::string modelName;                   // �g�p���郂�f����
	bool hasCollision = false;              // �����蔻�������
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f }; // �X�P�[�����O
};
// �A�C�e���̃����_�����O�f�[�^�\����
struct  ItemRenderData
{
	DirectX::Model* model;
	DirectX::SimpleMath::Matrix world;
};
// �}�b�v��1�}�X���̏��
struct MapItemData
{
	ItemInfo itemInfo;               // ���̏ꏊ�̃A�C�e���̎�ށiBlock�AStart�AGoal�Ȃǁj
	DirectX::SimpleMath::Vector3 pos; // ���̃}�X�̃��[���h���W
	std::unique_ptr<ItemBase> itemBasePtr = nullptr; // �A�C�e���̃x�[�X�N���X�ւ̃|�C���^
};