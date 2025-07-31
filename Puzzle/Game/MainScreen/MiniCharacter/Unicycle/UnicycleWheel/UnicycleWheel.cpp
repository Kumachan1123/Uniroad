#include "pch.h"
#include "UnicycleWheel.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleBody/UnicycleBody.h"
#include "KumachiLib/Easing/Easing.h"

UnicycleWheel::UnicycleWheel(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())
	, m_partNumber(MiniCharacter::GetPartsNumber())
	, m_partID(MiniCharacter::UNICYCLE_WHEEL)
	, m_pCommonResources(nullptr)
	, m_pModel(nullptr)
	, m_time(0.0f)
	, m_initialPosition(initialPosition)
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))
	, m_currentPosition{}
	, m_currentAngle{}
	, m_rotationMiniCharacterAngle{}
	, m_mass{}
	, m_MiniCharacterVelocity{}
{
	//m_pMiniCharacter = dynamic_cast<MiniCharacterSelectStage*>(parent->GetParent()->GetParent());
}

UnicycleWheel::~UnicycleWheel()
{
	Finalize();
}

void UnicycleWheel::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	assert(commonResources);
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Wheel");
}

void UnicycleWheel::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// 親を取得する
	const auto pMiniCharacter = m_pParent->GetParent()->GetParent();

	// 時間を加算
	m_time += elapsedTime;
	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition;
	// 現在の回転角を更新する
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// IsMoving() が true の場合のみこれ以降の処理を行う
	if (!pMiniCharacter || !pMiniCharacter->IsMoving())return;

	// キャラクターそのものから速度ベクトルを取得する
	m_MiniCharacterVelocity = pMiniCharacter->GetVelocity();
	// 速度に応じてホイールの回転速度を変える
	float wheelSpeed = m_MiniCharacterVelocity.Length() * elapsedTime * 60;
	// 親がMiniCharacterの時じゃない（プレイシーンじゃない）場合ホイールの回転速度を調整する
	if (!dynamic_cast<MiniCharacter*>(pMiniCharacter))wheelSpeed = 1.0f * elapsedTime * 30;
	// 時間経過でホイールを回転させる
	m_currentAngle = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_time * wheelSpeed) * m_currentAngle;
}

void UnicycleWheel::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();

	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(1) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition);
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);

}

void UnicycleWheel::Finalize()
{
}
