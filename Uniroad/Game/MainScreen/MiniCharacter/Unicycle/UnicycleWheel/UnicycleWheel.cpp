/*
*	@file UnicycleWheel.cpp
*	@brief UnicycleWheelクラスの実装
*/
#include "pch.h"
#include "UnicycleWheel.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleBody/UnicycleBody.h"

// 一輪車のタイヤの半径
const float UnicycleWheel::WHEEL_RADIUS = 1.0f;
// プレイシーン以外でのタイヤの回転速度
const float UnicycleWheel::WHEEL_SPEED = 3.0f;

/*
*	@brief コンストラクタ
*	@details 一輪車のタイヤクラスのコンストラクタ
*	@param parent 親コンポーネントへのポインタ
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度（ラジアン）
*	@return なし
*/
UnicycleWheel::UnicycleWheel(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_pParent(parent)// 親コンポーネント
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// ノード番号
	, m_partNumber(MiniCharacter::GetPartsNumber()) // 部品番号
	, m_partID(MiniCharacter::UNICYCLE_WHEEL) // 部品ID
	, m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_time(0.0f) // 時間
	, m_wheelAngle(0.0f) // 一輪車のタイヤの角度
	, m_initialPosition(initialPosition)// 初期位置
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// 初期角度
	, m_currentPosition{} // 現在の位置
	, m_currentAngle{} // 現在の回転角
	, m_rotationMiniCharacterAngle{} // プレイヤー回転角
	, m_mass{} // 質量
	, m_MiniCharacterVelocity{}// プレイヤー速度
{
}
/*
*	@brief デストラクタ
*	@details 一輪車のタイヤクラスのデストラクタ
*	@param なし
*	@return なし
*/
UnicycleWheel::~UnicycleWheel()
{
	// 後処理を行う
	Finalize();
}
/*
*	@brief 一輪車のタイヤクラスの初期化を行う
*	@details 一輪車のタイヤクラスの初期化を行い、必要なリソースを設定する。
*	@param commonResources 共通リソースへのポインタ
*	@return なし
*/
void UnicycleWheel::Initialize(CommonResources* commonResources)
{
	// 必要な名前空間を使用
	using namespace DirectX;
	// 共通リソースをセット
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Wheel");
}
/*
*	@brief 一輪車のタイヤクラスの更新を行う
*	@details 一輪車のタイヤクラスの更新を行い、位置や角度を更新する。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の回転角
*	@return なし
*/
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
	// 進行速度
	float wheelSpeed = m_MiniCharacterVelocity.Length();
	// 親がMiniCharacterの時じゃない（プレイシーンじゃない）場合
	if (!dynamic_cast<MiniCharacter*>(pMiniCharacter))
	{
		// 速度ベクトルを設定
		m_MiniCharacterVelocity = Vector3::Forward;
		// 進行速度を3倍にする
		wheelSpeed = m_MiniCharacterVelocity.Length() * WHEEL_SPEED;
	}
	// 進んだ距離 = 速度 × 時間、回転角度 = 距離 ÷ 半径
	float deltaAngle = wheelSpeed * elapsedTime / WHEEL_RADIUS;
	// 累積回転角度に加算
	m_wheelAngle += deltaAngle;
	// 回転クォータニオン生成（右回転）
	Quaternion wheelRotation = Quaternion::CreateFromAxisAngle(Vector3::Right, m_wheelAngle);
	// 合成して最終回転に
	m_currentAngle = wheelRotation * m_currentAngle;
	// 時間経過でホイールを回転させる
	m_currentAngle = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_time * wheelSpeed) * m_currentAngle;
	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(Vector3::One) * // スケール
		Matrix::CreateFromQuaternion(m_currentAngle) * // 回転
		Matrix::CreateTranslation(m_currentPosition); // 位置
	// ベースを取得
	auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent()->GetParent()->GetParent());
	// シャドウマップにモデルを登録する
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
}
/*
*	@brief 一輪車のタイヤクラスの描画を行う
*	@details 一輪車のタイヤクラスの描画を行い、ワールド行列を設定してモデルを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void UnicycleWheel::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストとステートを取得する
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();

	// モデルを描画する
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
}
/*
*	@brief 一輪車のタイヤクラスの後処理を行う
*	@details 一輪車のタイヤクラスの後処理を行い、リソースを解放する。
*	@param なし
*	@return なし
*/
void UnicycleWheel::Finalize()
{
	// モデルが存在する場合は解放する
	if (m_pModel)m_pModel = nullptr;
}
