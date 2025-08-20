/*
*	@file GoalLock.h
*	@brief �S�[�����b�N�̃A�C�e�����`����N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// �O���錾
class CommonResources;

// �S�[�����b�N�N���X
class GoalLock : public ItemBase
{
public:
	// �A�N�Z�T
	// �A�C�e���̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetPosition() const override { return m_position; }
	// �A�C�e���̈ʒu��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// �A�C�e���̃��f�����擾����
	DirectX::Model* GetModel() const override { return m_pModel; }
	// �A�C�e���̃��f����ݒ肷��
	void SetModel(DirectX::Model* model) override { m_pModel = model; }
	// �A�C�e���̃��[���h�s����擾����
	const DirectX::SimpleMath::Matrix& GetWorldMatrix() const override { return m_worldMatrix; }
	// �A�C�e���̃��[���h�s���ݒ肷��
	void SetWorldMatrix(const DirectX::SimpleMath::Matrix& world) override { m_worldMatrix = world; }
	// �A�C�e���̂���s�ԍ����擾����
	int GetRow() const override { return m_row; }
	// �A�C�e���̂���s�ԍ���ݒ肷��
	void SetRow(int row) override { m_row = row; }
	// �A�C�e���̂����ԍ����擾����
	int GetCol() const override { return m_col; }
	// �A�C�e���̂����ԍ���ݒ肷��
	void SetCol(int col) override { m_col = col; }
	// �J�������擾����
	ICamera* GetCamera() const override { return m_pCamera; }
	// �J������ݒ肷��
	void SetCamera(ICamera* camera) override { m_pCamera = camera; }
public:
	// public�֐�
	// �R���X�g���N�^
	GoalLock();
	// �f�X�g���N�^
	~GoalLock() override;
	// ������
	void Initialize(CommonResources* resources, const ItemInfo& info) override;
	// �X�V
	void Update(float elapsedTime) override;
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// �A�C�e�����擾�����Ƃ��̏���
	void OnGet(MiniCharacter* miniCharacter) override;
	// �A�C�e�����g�p�����Ƃ��̏���
	void OnUse(MiniCharacter* miniCharacter) override;
	// �A�C�e����j�������Ƃ��̏���
	void OnDiscard(MiniCharacter* miniCharacter) override;
private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �S�[�����b�N�̃��f��
	DirectX::Model* m_pModel;
	// �J�����̃|�C���^�[
	ICamera* m_pCamera;
	// �S�[�����b�N�̏��
	ItemInfo m_itemInfo;
	// �S�[�����b�N�̈ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// �S�[�����b�N�̃X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// �S�[�����b�N�̉�]
	DirectX::SimpleMath::Quaternion m_rotation;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ����
	float m_time;
	// ��]���x
	float m_rotationSpeed;
	// �S�[�����b�N���l������ď�����܂ł̎���
	float m_deleteTime;
	// �S�[�����b�N���l�����ꂽ���ǂ���
	bool m_isCollected;
	// �~�j�L�����̃|�C���^�[
	MiniCharacter* m_pMiniCharacter;
	// ���̃A�C�e��������s�ԍ�
	int m_row;
	// ���̃A�C�e���������ԍ�
	int m_col;

};