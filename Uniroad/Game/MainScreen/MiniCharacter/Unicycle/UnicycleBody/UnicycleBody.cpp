/*
*	@file UnicycleBody.cpp
*	@brief 一輪車の車体クラスの実装
*/
#include "pch.h"
#include "UnicycleBody.h"
#include "Game/MainScreen/MiniCharacter/Unicycle/UnicycleWheel/UnicycleWheel.h"
/*
*	@brief コンストラクタ
*	@details 一輪車の車体クラスのコンストラクタ
*	@param parent 親コンポーネントへのポインタ
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度（ラジアン）
*	@return なし
*/
UnicycleBody::UnicycleBody(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)// 親コンポーネントへのポインタ
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// ノード番号
	, m_partNumber(MiniCharacter::GetPartsNumber())// 部品番号 
	, m_partID(MiniCharacter::UNICYCLE_BODY)// 部品ID
	, m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_initialPosition(initialPosition)// 初期位置
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// 初期角度
	, m_currentPosition{} // 現在の位置
	, m_currentAngle{} // 現在の回転角
	, m_rotationMiniCharacterAngle{} // プレイヤー回転角
	, m_mass{} // 質量
	, m_MiniCharacterVelocity{} // プレイヤー速度
	, m_worldMatrix{} // ワールド行列
	, m_time(0.0f)// 時間
{
}
/*
*	@brief デストラクタ
*	@details 一輪車の車体クラスのデストラクタ
*	@param なし
*	@return なし
*/
UnicycleBody::~UnicycleBody()
{
	// 後処理を行う
	Finalize();
}
/*
*	@brief 一輪車の車体クラスの初期化を行う
*	@details 一輪車の車体クラスの初期化を行い、必要なリソースを設定する。
*	@param commonResources 共通リソースへのポインタ
*	@return なし
*/
void UnicycleBody::Initialize(CommonResources* commonResources)
{
	// 共通リソースへのポインタが有効であることを確認する
	assert(commonResources);
	// 共通リソースへのポインタを設定する
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Unicycle_Body");
	// タイヤを追加
	Attach(std::make_unique<UnicycleWheel>(this, MiniCharacterParameters::UNICYCLE_WHEEL_POSITION, 0.0f));
}
/*
*	@brief 一輪車の車体クラスの更新を行う
*	@details 一輪車の車体クラスの更新を行い、現在の位置と回転角を更新する。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の回転角
*	@return なし
*/
void UnicycleBody::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition;
	// 現在の回転角を更新する
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// ベースを取得する
	const auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent()->GetParent());
	// シャドウマップにモデルを登録する
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
	// 「胴体」部品を更新する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
}
/*
*	@brief 一輪車の車体クラスの描画を行う
*	@details 一輪車の車体クラスの描画を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void UnicycleBody::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX名前空間のSimpleMathを使用する
	using namespace DirectX::SimpleMath;
	// Direct3Dデバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(Vector3::One) *
		Matrix::CreateFromQuaternion(m_currentAngle) *
		Matrix::CreateTranslation(m_currentPosition);
	// モデルを描画する
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
	// 部品を描画する
	for (auto& part : m_pMiniCharacterParts)part->Render(view, proj);
}
/*
*	@brief 一輪車の車体クラスに部品を追加する
*	@details 一輪車の車体クラスに部品を追加する
*	@param MiniCharacterPart 追加する部品のコンポーネント
*	@return なし
*/
void UnicycleBody::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品を初期化する
	MiniCharacterPart->Initialize(m_pCommonResources);
	// 部品を追加する
	m_pMiniCharacterParts.emplace_back(std::move(MiniCharacterPart));
}
/*
*	@brief 一輪車の車体クラスから部品を削除する
*	@details 一輪車の車体クラスから部品を削除する
*	@param MiniCharacterPart 削除する部品のコンポーネント
*	@return なし
*/
void UnicycleBody::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 今は何もしない
}
/*
*	@brief 一輪車の車体クラスの後処理を行う
*	@details 一輪車の車体クラスの後処理を行う
*	@param なし
*	@return なし
*/
void UnicycleBody::Finalize()
{
	// 今は何もしない
}
