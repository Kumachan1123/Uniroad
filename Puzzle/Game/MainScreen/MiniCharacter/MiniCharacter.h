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
#include <Libraries\MyLib\DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "Game/Scenes/PlayScene/PlayScene.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "Game/MainScreen/MiniCharacter/Interface/IComposite.h"
#include "Game/MainScreen/MiniCharacter/Sheep/Sheep.h"
// �O���錾
class CommonResources;

// �~�j�L�����N�^�[�N���X
class MiniCharacter : public IComposite
{
public:
	// ���iID
	enum PartID : int
	{
		MINICHARACTER = -1,
		BODY = 0,
		HEAD = 1,
		ARM_LEFT = 2,
		ARM_RIGHT = 3,
		SHEEP = 4,
		UNICYCLE_BODY = 5,
		UNICYCLE_WHEEL = 6,
	};
public:
	// �A�N�Z�T
	// �m�[�h�ԍ����擾����
	int GetNodeNumber() const { return m_nodeNumber; }
	// ���iID���擾����
	int GetPartID() const { return  m_partID; }
	// ���i�ԍ����擾����
	int GetPartNumber() const { return m_partNumber; }
	// �e���擾����
	IComponent* GetParent() const { return m_parent; }
	// ���݂̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }
	// ���݂̈ʒu��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& currretPosition) { m_currentPosition = currretPosition; }
	// ���݂̑��x���擾����
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_currentVelocity; }
	// ���݂̑��x��ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& currentVelocity) { m_currentVelocity = currentVelocity; }
	// ���݂̉�]�p���擾����
	DirectX::SimpleMath::Quaternion GetAngle() const { return m_currentAngle; }
	// ���݂̉�]�p��ݒ肷��
	void SetAngle(const DirectX::SimpleMath::Quaternion& currentAngle) { m_currentAngle = currentAngle; }
	// �h����擾����
	DirectX::SimpleMath::Quaternion GetShakeQuaternion() const { return m_shakeQuaternion; }
	// �h���ݒ肷��
	void SetShakeQuaternion(const DirectX::SimpleMath::Quaternion& shakeQuaternion) { m_shakeQuaternion = shakeQuaternion; }



	// ���ʂ��擾����
	float GetMass() const { return m_mass; }
	// ���ʂ�ݒ肷��
	void SetMass(const float& mass) { m_mass = mass; }
	// ���f�����擾����
	DirectX::Model* GetModel() { return nullptr; }
	// CSV�}�b�v���擾����
	CSVMap* GetCSVMap() { return nullptr; }
	// CSV�A�C�e�����擾����
	CSVItem* GetCSVItem() { return nullptr; }
	// ���Ɍ����^�C���̃N���X���擾����
	NextTiles* GetNextTiles() { return nullptr; }

	// ���[���h�s����擾����
	DirectX::SimpleMath::Matrix& GetWorldMatrix() { return m_worldMatrix; }
	// �^�C���ɓ��������ǂ������m�F����
	bool HasEnteredTile(const TileBase* tile) const { return m_enteredTiles.count(tile) > 0; }
	// �^�C���ɓ��������Ƃ��L�^����
	void SetEnteredTile(const TileBase* tile) { m_enteredTiles.insert(tile); }
	// �������^�C���̃Z�b�g���N���A����
	void ResetEnteredTiles() { m_enteredTiles.clear(); }
	// �O�ɓ����Ă��^�C���̖��O
	const std::string& GetPrevTileName() const { return m_prevTileName; }
	// �ړ��t���O���擾
	bool IsMoving() const { return m_isMoving; }
	// �ړ��t���O��ݒ�
	void SetMoving(bool isMoving) { m_isMoving = isMoving; }
	// �����^�C�}�[���L�����ǂ������擾
	bool IsFallTimerActive() const { return m_fallTimerActive; }
	// �����^�C�}�[���L�����ǂ�����ݒ�
	void SetFallTimerActive(bool active) { m_fallTimerActive = active; }
	// �Q�[���N���A�t���O��؂�ւ��鎞�Ԃ��擾����
	float GetGameClearSwitchTime() const { return m_gameClearSwitchTime; }
	// �Q�[���N���A�t���O��؂�ւ��鎞�Ԃ�ݒ肷��
	void SetGameClearSwitchTime(float time) { m_gameClearSwitchTime = time; }
public:
	// �R���X�g���N�^
	MiniCharacter(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// �f�X�g���N�^
	~MiniCharacter();
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
	// private�֐�
	// �^�C���֘A�̏���
	void UpdateTileEvents();
	// �����^�C�}�[�֘A�̏���
	void UpdateFallTimer(float elapsedTime);
	// �d�͂������������W�ړ�
	void Moving(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition);
	// �h�ꉉ�o
	void Shake();
	// ��]�̕��
	void InterpolateRotation(float elapsedTime, const DirectX::SimpleMath::Quaternion& currentAngle);
	//�@���x���X�V����
	void UpdateSpeedByStartTile();
	// �^�C���̒��S�ɂ��邩�ǂ������m�F����
	bool IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon = 0.1f) const;
	// �Q�[���I�[�o�[�A�Q�[���N���A���򏈗�
	void HandleGameOverAndClear(float elapsedTime);
public:
	// �m�[�h�J�E���g�A�b�v������m�[�h�J�E���g���擾����
	static int GetNodeCountAfterCountUp() { return ++s_nodeCount; }
	// �m�[�h�J�E���g���擾����
	static int GetNodeCount() { return s_nodeCount; }
	// ���i�ԍ������Z�b�g����(-1����J�n����)
	static void ResetPartsNumber() { s_partsNumber = -1; }
	// ���i�ԍ����擾����
	static int GetPartsNumber() { return s_partsNumber; }
	// ���i�ԍ����₷
	static void IncrementPartsNumber() { s_partsNumber++; }
private:
	// private�萔
	// �m�[�h�̃J�E���g
	static int s_nodeCount;
	// ���i�J�E���g
	static int s_partsNumber;
	// �d��
	static const float GRAVITY;
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �e
	IComponent* m_parent;
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
	// �O�̃^�C���̈ʒu
	DirectX::SimpleMath::Vector3 m_prevPosition;
	// ���݂̈ʒu
	DirectX::SimpleMath::Vector3 m_currentPosition;
	// ���݂̑��x
	DirectX::SimpleMath::Vector3 m_currentVelocity;
	// ���݂̉�]�p
	DirectX::SimpleMath::Quaternion m_currentAngle;
	// �v���C���[��]�p
	DirectX::SimpleMath::Quaternion m_rotationMiniCharacterAngle;
	// �v���C���[���x
	DirectX::SimpleMath::Vector3 m_miniCharacterVelocity;
	// �v���C���[�̗h��
	DirectX::SimpleMath::Quaternion m_shakeQuaternion;
	// ����
	float m_mass;
	// ���i�z��
	std::vector<std::unique_ptr<IComponent>> m_pMiniCharacterParts;
	//	���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �������^�C���̃Z�b�g
	std::set<const TileBase*> m_enteredTiles;
	// �O�t���[���ň�ԋ߂������^�C���̖��O
	std::string m_prevTileName;
	// ������^�C���̍��W
	DirectX::SimpleMath::Vector3 m_currentTilePosition;
	// �O�����^�C���̍��W
	DirectX::SimpleMath::Vector3 m_prevTilePosition;
	// �������^�C���̃|�C���^
	TileBase* m_enteredTilePtr;
	// ����������t���O
	bool m_hasEnteredTile;
	// �ړ��t���O
	bool m_isMoving;
	// �����^�C�}�[���L����
	bool m_fallTimerActive;
	// �����^�C�}�[
	float m_fallTimer;
	// ��x�����������������s�����邽�߂̃t���O
	bool m_hasFallen;
	// �Q�[���I�[�o�[�t���O��؂�ւ��鎞��
	float m_gameOverSwitchTime;
	// �Q�[���N���A�t���O��؂�ւ��鎞��
	float m_gameClearSwitchTime;
	// �ړ����x
	float m_speed;
};