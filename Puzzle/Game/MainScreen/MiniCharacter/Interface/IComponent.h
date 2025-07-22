#pragma once
class CommonResources;
class CSVMap;
class CSVItem;
class NextTiles;
// IComponent�C���^�[�t�F�[�X���`����
class IComponent
{
public:
	// �m�[�h�ԍ����擾����
	virtual int GetNodeNumber() const = 0;
	// ���iID���擾����
	virtual int GetPartID() const = 0;
	// ���i�ԍ����擾����
	virtual int GetPartNumber() const = 0;
	// �e���擾����
	virtual IComponent* GetParent() const = 0;
	// ���݂̈ʒu���擾����
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
	// ���݂̈ʒu��ݒ肷��
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& curretPosition) = 0;
	// ���݂̉�]�p���擾����
	virtual DirectX::SimpleMath::Quaternion GetAngle() const = 0;
	// ���݂̉�]�p��ݒ肷��
	virtual void SetAngle(const DirectX::SimpleMath::Quaternion& angle) = 0;
	// ���ʂ��擾����
	virtual float GetMass() const = 0;
	// ���ʂ�ݒ肷��
	virtual void SetMass(const float& mass) = 0;
	// ���f�����擾����
	virtual DirectX::Model* GetModel() = 0;
	// CSV�}�b�v���擾����
	virtual CSVMap* GetCSVMap() = 0;
	// CSV�A�C�e�����擾���� 
	virtual CSVItem* GetCSVItem() = 0;
	// ���Ɍ����^�C���̃N���X���擾����
	virtual NextTiles* GetNextTiles() = 0;

public:
	// ���z�f�X�g���N�^
	virtual ~IComponent() = default;
	// ����������
	virtual void Initialize(CommonResources* commonResources) = 0;
	// �X�V����
	virtual void Update(float elapsedTime, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& angle) = 0;
	// �`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// �㏈�����s��
	virtual void Finalize() = 0;
};
