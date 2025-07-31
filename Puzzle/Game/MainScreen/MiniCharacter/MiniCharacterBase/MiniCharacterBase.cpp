#include <pch.h>
#include "MiniCharacterBase.h"

MiniCharacterBase::MiniCharacterBase(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber{}
	, m_partNumber{}
	, m_partID{}
	, m_nodes{}
	, m_initialPosition(initialPosition)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition(initialPosition)
	, m_currentAngle{}
	, m_mass(0.0f)
	, m_pCommonResources(nullptr)
	, m_pCSVMap(nullptr)
	, m_pCSVItem(nullptr)
	, m_pNextTiles(nullptr)
	, m_pPlaneArea(nullptr)
	, m_isMoving(false)
{
}

MiniCharacterBase::~MiniCharacterBase()
{
}

void MiniCharacterBase::Initialize(CommonResources* commonResources)
{
	m_pCommonResources = commonResources;
}


void MiniCharacterBase::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// ���݂̈ʒu���X�V����
	m_currentPosition = currentPosition;
	// ���݂̉�]�p���X�V����
	m_currentAngle = currentAngle;
	// �m�[�h���X�V����
	for (auto& node : m_nodes)
	{
		node->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}

void MiniCharacterBase::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	MiniCharacterPart->Initialize(m_pCommonResources);
	m_nodes.emplace_back(std::move(MiniCharacterPart));

}

void MiniCharacterBase::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i���폜����
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
		[&MiniCharacterPart](const std::unique_ptr<IComponent>& node) {
			return node.get() == MiniCharacterPart.get();
		});
	// �폜���镔�i�����������ꍇ�͕��i���폜����
	if (it != m_nodes.end())
	{
		m_nodes.erase(it);
	}
}

void MiniCharacterBase::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{	// �m�[�h��`�悷��
	for (auto& node : m_nodes)
	{
		//�m�[�h��`�悷��
		node->Render(view, proj);
	}
	// ---�f�o�b�O�\��---
	const auto debugString = m_pCommonResources->GetDebugString();

	// ���W�\��
	debugString->AddString("MiniCharacter Position: (%f, %f, %f)",
		m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z);
}

void MiniCharacterBase::Finalize()
{	// �m�[�h���N���A����
	m_nodes.clear();
}
