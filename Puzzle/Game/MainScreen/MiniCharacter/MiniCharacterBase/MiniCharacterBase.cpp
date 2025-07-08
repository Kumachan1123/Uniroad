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
	, m_currentPosition(0.0f, 0.0f, 0.0f)
	, m_currentAngle{}
	, m_mass(0.0f)
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
	// 現在の位置を更新する
	m_currentPosition = currentPosition;
	// 現在の回転角を更新する
	m_currentAngle = currentAngle;
	// ノードを更新する
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
	// 部品を削除する
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
		[&MiniCharacterPart](const std::unique_ptr<IComponent>& node) {
			return node.get() == MiniCharacterPart.get();
		});
	// 削除する部品が見つかった場合は部品を削除する
	if (it != m_nodes.end())
	{
		m_nodes.erase(it);
	}
}

void MiniCharacterBase::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{	// ノードを描画する
	for (auto& node : m_nodes)
	{
		//ノードを描画する
		node->Render(view, proj);
	}
}

void MiniCharacterBase::Finalize()
{	// ノードをクリアする
	m_nodes.clear();
}
