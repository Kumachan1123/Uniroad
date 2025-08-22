/*
*	@file ItemBase.h
*	@brief �A�C�e���̃x�[�X�N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/MainScreen/Items/ItemDatas/ItemDatas.h"

// �O���錾
class MiniCharacter;
class CommonResources;
class ICamera;
class ShadowMapLight;

// �A�C�e���x�[�X�N���X
class ItemBase
{
	// �A�N�Z�T
public:
	// �A�C�e���̈ʒu���擾����
	virtual const DirectX::SimpleMath::Vector3& GetPosition() const = 0;
	// �A�C�e���̈ʒu��ݒ肷��
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
	// �A�C�e���̃��f�����擾����
	virtual DirectX::Model* GetModel() const = 0;
	// �A�C�e���̃��f����ݒ肷��
	virtual void SetModel(DirectX::Model* model) = 0;
	// �A�C�e���̃��[���h�s����擾����
	virtual const DirectX::SimpleMath::Matrix& GetWorldMatrix() const = 0;
	// �A�C�e���̃��[���h�s���ݒ肷��
	virtual void SetWorldMatrix(const DirectX::SimpleMath::Matrix& world) = 0;
	// �A�C�e���̂���s�ԍ����擾����
	virtual int GetRow() const = 0;
	// �A�C�e���̂���s�ԍ���ݒ肷��
	virtual void SetRow(int row) = 0;
	// �A�C�e���̂����ԍ����擾����
	virtual int GetCol() const = 0;
	// �A�C�e���̂����ԍ���ݒ肷��
	virtual void SetCol(int col) = 0;
	// �J�������擾����
	virtual ICamera* GetCamera() const = 0;
	// �J������ݒ肷��
	virtual void SetCamera(ICamera* camera) = 0;
	// �V���h�E�}�b�v���C�g���擾����
	virtual ShadowMapLight* GetShadowMapLight() const = 0;
	// �V���h�E�}�b�v���C�g��ݒ肷��
	virtual void SetShadowMapLight(ShadowMapLight* shadowMapLight) = 0;

public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~ItemBase() = default;
	// ������
	virtual void Initialize(CommonResources* resources, const ItemInfo& info) = 0;
	// �X�V
	virtual void Update(float elapsedTime) = 0;
	// �`��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// �A�C�e�����擾�����Ƃ��̏���
	virtual void OnGet(MiniCharacter* miniCharacter) = 0;
	// �A�C�e�����g�p�����Ƃ��̏���
	virtual void OnUse(MiniCharacter* miniCharacter) = 0;
	// �A�C�e����j�������Ƃ��̏���
	virtual void OnDiscard(MiniCharacter* miniCharacter) = 0;

};