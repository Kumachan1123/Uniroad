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
#include "Game/MainScreen/MiniCharacter/Interface/IComposite.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/CSVItem/CSVItem.h"
#include "Game/ControllScreen/NextTiles/NextTiles.h"
#include "Game/Scenes/StageSelectScene/PlaneArea/PlaneArea.h"

// �O���錾
class CommonResources;

// �~�j�L�����N�^�[�̃x�[�X�N���X
class MiniCharacterBase : public IComposite
{
public:
	// �A�N�Z�T
	// �m�[�h�ԍ����擾����
	int GetNodeNumber() const { return m_nodeNumber; }
	// ���iID���擾����
	int GetPartID() const { return  m_partID; }
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
	// ���ʂ��擾����
	float GetMass() const { return m_mass; }
	// ���ʂ�ݒ肷��
	void SetMass(const float& mass) { m_mass = mass; }
	// ���f�����擾����
	DirectX::Model* GetModel() { return nullptr; }
	// CSV�}�b�v���擾����
	CSVMap* GetCSVMap() { return m_pCSVMap; }
	// CSV�}�b�v��ݒ肷��
	void SetCSVMap(CSVMap* csvMap) { m_pCSVMap = csvMap; }
	// CSV�A�C�e�����擾����
	CSVItem* GetCSVItem() { return m_pCSVItem; }
	// CSV�A�C�e����ݒ肷��
	void SetCSVItem(CSVItem* csvItem) { m_pCSVItem = csvItem; }
	// ���ʃG���A���擾����
	PlaneArea* GetPlaneArea() { return m_pPlaneArea; }
	// ���ʃG���A��ݒ肷��
	void SetPlaneArea(PlaneArea* planeArea) { m_pPlaneArea = planeArea; }
	// ���Ɍ����^�C���̃N���X���擾����
	NextTiles* GetNextTiles() { return m_pNextTiles; }
	// ���Ɍ����^�C���̃N���X��ݒ肷��
	void SetNextTiles(NextTiles* nextTiles) { m_pNextTiles = nextTiles; }
	// �ړ��t���O���擾����
	bool IsMoving() const { return m_isMoving; }
	// �ړ��t���O��ݒ肷��
	void SetMoving(bool isMoving) { m_isMoving = isMoving; }
public:
	// �R���X�g���N�^
	MiniCharacterBase(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// �f�X�g���N�^
	~MiniCharacterBase();
	// ����������
	void Initialize(CommonResources* commonResources);
	// �X�V����
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle);
	// ���i��ǉ�����
	void Attach(std::unique_ptr<IComponent> MiniCharacterPart);
	// ���i���폜����
	void Detach(std::unique_ptr<IComponent> MiniCharacterPart);
	// �`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �㏈�����s��
	void Finalize();
private:
	// �e
	IComponent* m_pParent;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// CSV�}�b�v
	CSVMap* m_pCSVMap;
	// CSV�A�C�e��
	CSVItem* m_pCSVItem;
	// ���Ɍ����^�C���̃N���X
	NextTiles* m_pNextTiles;
	// ���ʃG���A
	PlaneArea* m_pPlaneArea;
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
	// ����
	float m_mass;
	// �m�[�h�z��
	std::vector<std::unique_ptr<IComponent>> m_nodes;
	// �~�j�L�����̈ړ��t���O
	bool m_isMoving;
};