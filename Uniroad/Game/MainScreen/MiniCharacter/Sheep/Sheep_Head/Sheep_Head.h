/*
*	@file Sheep_Head.h
*	@brief �Ђ��̓��N���X�̃w�b�_�[�t�@�C��
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include <DeviceResources.h>
// �O�����C�u����
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "Game/Scenes/PlayScene/PlayScene.h"
#include "Game/MainScreen/MiniCharacter/Interface/ILeaf.h"
#include "KumachiLib/Easing/Easing.h"

// �O���錾
class CommonResources;

// �Ђ��̓��N���X
class SheepHead : public ILeaf
{
public:
	// �A�N�Z�T
	// �m�[�h�ԍ����擾����
	int GetNodeNumber() const { return m_nodeNumber; }
	// ���iID���擾����
	int GetPartID() const { return m_partID; }
	// ���i�ԍ����擾����
	int GetPartNumber() const { return m_partNumber; }
	// �e���擾����
	IComponent* GetParent() const { return m_pParent; }
	// ���݂̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }
	// ���݂̈ʒu��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& currretPosition) { m_currentPosition = currretPosition; }
	// ���݂̉�]�p���擾����
	DirectX::SimpleMath::Quaternion GetAngle() const { return m_currentAngle; }
	// ���݂̉�]�p��ݒ肷��
	void SetAngle(const DirectX::SimpleMath::Quaternion& currentAngle) { m_currentAngle = currentAngle; }
	// ���݂̑��x���擾����
	DirectX::SimpleMath::Vector3 GetVelocity() const { return 	DirectX::SimpleMath::Vector3::Zero; }
	// �ړ��t���O���擾
	bool IsMoving() const { return false; }
	// ���ʂ��擾����
	float GetMass() const { return m_mass; }
	// ���ʂ�ݒ肷��
	void SetMass(const float& mass) { m_mass = mass; }
	// ���f�����擾����
	DirectX::Model* GetModel() { return m_pModel; }
	// CSV�}�b�v���擾����
	CSVMap* GetCSVMap() { return nullptr; }
	// CSV�A�C�e�����擾����
	CSVItem* GetCSVItem() { return nullptr; }
	// ���Ɍ����^�C���̃N���X���擾����
	NextTiles* GetNextTiles() { return nullptr; }
	// �V���h�E�}�b�v���C�g���擾����
	ShadowMapLight* GetShadowMapLight() { return nullptr; }
	// ���[���h�s����擾����
	DirectX::SimpleMath::Matrix& GetWorldMatrix() { return m_worldMatrix; }
public:
	// public�֐�
	// �R���X�g���N�^
	SheepHead(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// �f�X�g���N�^
	~SheepHead();
	// ����������
	void Initialize(CommonResources* commonResources);
	// �X�V����
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle);
	// �`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �㏈�����s��
	void Finalize();


private:
	// private�֐�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �e
	IComponent* m_pParent;
	// ����
	float m_time;
	// ���f��
	DirectX::Model* m_pModel;
	// �m�[�h�ԍ�
	int m_nodeNumber;
	// ���i�ԍ�
	int m_partNumber;
	// ���iID
	int m_partID;
	// �����ʒu
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// ������]�p
	DirectX::SimpleMath::Quaternion m_initialAngle;
	// ���݂̈ʒu
	DirectX::SimpleMath::Vector3 m_currentPosition;
	// ���݂̉�]�p
	DirectX::SimpleMath::Quaternion m_currentAngle;
	// �v���C���[��]�p
	DirectX::SimpleMath::Quaternion m_rotationMiniCharacterAngle;
	// �v���C���[���x
	DirectX::SimpleMath::Vector3 m_MiniCharacterVelocity;
	// ����
	float m_mass;
	// �m�[�h�z��
	std::vector<std::unique_ptr<IComponent>> m_pNodes;
	//	���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;

};

