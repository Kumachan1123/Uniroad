/*
*	@file Medal.h
*	@brief ���_���N���X
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
#include "KumachiLib/Easing/Easing.h"
#include "Game/Particle/Particle.h"
#include "Game/Particle/Utility.h"
#include "Game/MainScreen/Shadow/Shadow.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"


// �O���錾
class CommonResources;

// ���_���N���X
class Medal : public ItemBase
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
	// �V���h�E�}�b�v���C�g���擾����
	ShadowMapLight* GetShadowMapLight() const override { return m_pShadowMapLight; }
	// �V���h�E�}�b�v���C�g��ݒ肷��
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) override { m_pShadowMapLight = shadowMapLight; }

public:
	// public�֐�
	// �R���X�g���N�^
	Medal();
	// �f�X�g���N�^
	~Medal() override;
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
	// private�֐�
	// �p�[�e�B�N���̃p�����[�^�[��ݒ肷��
	Utility::ParticleParams SetParticleParams() const;
private:
	// �萔
	// �f�t�H���g�̉�]���x
	static constexpr float DEFAULT_ROTATION_SPEED = 1.0f;
	// �l������ď�����܂ł̉�]���x
	static constexpr float COLLECTED_ROTATION_SPEED = 100.0f;
private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// ���_���̃��f��
	DirectX::Model* m_pModel;
	// �J�����̃|�C���^�[
	ICamera* m_pCamera;
	// �V���h�E�}�b�v���C�g
	ShadowMapLight* m_pShadowMapLight;
	// �p�[�e�B�N��
	std::unique_ptr<Particle> m_pParticle;
	// �e
	std::unique_ptr<Shadow> m_pShadow;
	// ���_���̏��
	ItemInfo m_itemInfo;
	// ���_���̈ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���_���̃X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// ���_���̉�]
	DirectX::SimpleMath::Quaternion m_rotation;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ����
	float m_time;
	// ��]���x
	float m_rotationSpeed;
	// ���_�����l������ď�����܂ł̎���
	float m_deleteTime;
	// ���_�����l�����ꂽ���ǂ���
	bool m_isCollected;
	// �~�j�L�����̃|�C���^�[
	MiniCharacter* m_pMiniCharacter;
	// ���̃A�C�e��������s�ԍ�
	int m_row;
	// ���̃A�C�e���������ԍ�
	int m_col;
};