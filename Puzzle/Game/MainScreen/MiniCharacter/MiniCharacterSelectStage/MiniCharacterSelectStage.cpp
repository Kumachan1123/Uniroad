/*
*	@file MiniCharacterSelectStage.cpp
*	@brief ミニキャラクターの処理を定義するクラス
*	@details ミニキャラクターの位置、角度、速度などを管理し、タイルのイベントを処理する。
*/
#include <pch.h>
#include "MiniCharacterSelectStage.h"
// ノードカウンター
int MiniCharacterSelectStage::s_nodeCount = 0;
// 部品カウンター
int MiniCharacterSelectStage::s_partsNumber = 0;
/*
*	@brief コンストラクタ
*	@details ミニキャラクターの初期位置と角度を設定し、必要な変数を初期化する。
*	@param parent 親コンポーネント
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度（ラジアン）
*	@return なし
*/
MiniCharacterSelectStage::MiniCharacterSelectStage(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_parent(parent)// 親コンポーネント
	, m_nodeNumber(MiniCharacterSelectStage::GetNodeCountAfterCountUp())// ノード番号
	, m_partNumber(MiniCharacterSelectStage::GetPartsNumber())// 部品番号
	, m_partID(MiniCharacterSelectStage::MINICHARACTER)// 部品ID
	, m_pCommonResources(nullptr) // 共通リソース
	, m_enteredTilePtr(nullptr)// 入ったタイルのポインタ
	, m_hasEnteredTile(false)// 入った直後フラグ
	, m_isMoving(true)// 移動フラグ
	, m_fallTimer(0.0f)// 落下タイマー
	, m_fallTimerActive(false)// 落下タイマーが有効かどうか
	, m_hasFallen(false)// 一度だけ落下処理を実行させるためのフラグ
	, m_initialPosition(initialPosition)// 初期位置
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// 初期角度
	, m_currentPosition{}// 現在の位置
	, m_currentVelocity{}// 現在の速度
	, m_currentAngle{}// 現在の回転角
	, m_destinationPosition(initialPosition)// 目的地の位置
	, m_prevTileName("Start")// 前フレームで一番近かったタイルの名前
	, m_rotationMiniCharacterAngle{}// プレイヤー回転角
	, m_mass{}// 質量
	, m_miniCharacterVelocity{}// プレイヤー速度
{
}
/*
*	@brief デストラクタ
*	@details ミニキャラクターの後処理を行う。
*	@param なし
*	@return なし
*/
MiniCharacterSelectStage::~MiniCharacterSelectStage()
{
	// ミニキャラクターの後処理を行う
	Finalize();
}
/*
*	@brief ミニキャラクターの初期化を行う
*	@details ミニキャラクターの初期化を行い、必要なリソースを設定する。
*	@param resources 共通リソース
*	@return なし
*/
void MiniCharacterSelectStage::Initialize(CommonResources* resources)
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// 共通リソースが存在することを確認する
	assert(resources);
	// 共通リソースを設定する
	m_pCommonResources = resources;
	// 現在位置に反映
	m_currentPosition = m_initialPosition;
	// ヒツジパーツをアタッチ
	Attach(std::make_unique<Sheep>(this, Vector3(0.0f, 3.5f, 0.0f), 0.0f));
	// 影を作成
	m_pShadow = std::make_unique<Shadow>();
	// 影の初期化
	m_pShadow->Initialize(m_pCommonResources);
}
/*
*	@brief プレイヤーの位置と角度を更新する
*	@details プレイヤーの位置と角度を更新する。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の角度
*	@return なし
*/
void MiniCharacterSelectStage::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 必要な名前空間を使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(currentPosition);
	// 回転の補間
	InterpolateRotation(currentAngle);
	// 親コンポーネントのポインターに変換
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// 親コンポーネントが存在することを確認
	assert(parent);
	// 目的地を設定
	if (parent->GetPlaneArea()->GetHitPlaneIndex() > -1 && parent->GetPlaneArea()->IsHitPlane())
		m_destinationPosition = parent->GetPlaneArea()->GetPlanePosition(parent->GetPlaneArea()->GetHitPlaneIndex());
	// 目的地にむかって速度を更新する
	m_currentVelocity = (m_destinationPosition - m_currentPosition) * elapsedTime * 3.0f;
	m_currentVelocity.y = 0.0f;
	// 目的地に到着したら速度をゼロにする
	if ((m_destinationPosition - m_currentPosition).Length() <= 0.0001f)
		m_currentVelocity = Vector3::Zero;
	// プレイヤーの位置を更新する
	m_currentPosition += m_currentVelocity;
	m_currentPosition.y = m_initialPosition.y;
	// 親にカメラに渡すための座標を渡す
	parent->SetCameraPosition(m_currentPosition);
	// 部品を更新する　
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
}
/*
*	@brief プレイヤーの部品を追加する
*	@details プレイヤーの部品を追加する。
*	@param MiniCharacterPart 追加する部品
*	@return なし
*/
void MiniCharacterSelectStage::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品番号を取得
	MiniCharacterPart->Initialize(m_pCommonResources);
	// 部品番号を設定
	m_pMiniCharacterParts.push_back(std::move(MiniCharacterPart));
}

/*
*	@brief プレイヤーの部品を削除する
*	@details プレイヤーの部品を削除する(今は何もしない)
*	@param MiniCharacterPart 削除する部品
*	@return なし
*/
void MiniCharacterSelectStage::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品を削除する処理は今は何もしない
}
/*
*	@brief プレイヤーを描画する
*	@details プレイヤーを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void MiniCharacterSelectStage::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 部品を描画する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Render(view, proj);
	// 影を描画する
	m_pShadow->Render(view, proj, m_currentPosition, 1.0f);
#ifdef _DEBUG
	// ---デバッグ表示---
	const auto debugString = m_pCommonResources->GetDebugString();
	// 座標表示
	debugString->AddString("MiniCharacter Position: (%f, %f, %f)",
		m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
	// 速度表示
	debugString->AddString("MiniCharacter Velocity: (%f, %f, %f)",
		m_currentVelocity.x, m_currentVelocity.y, m_currentVelocity.z);
#endif // DEBUG



}
/*
*	@brief プレイヤーの後処理を行う
*	@details プレイヤーの後処理を行う(今は何もしない)。
*	@param なし
*	@return なし
*/
void MiniCharacterSelectStage::Finalize()
{
	// 何もしない
}

/*
*	@brief プレイヤーの回転を補間する
*	@details プレイヤーの回転を補間して、滑らかな回転を実現する。
*	@param currentAngle 現在の回転角
*	@return なし
*
*/
void MiniCharacterSelectStage::InterpolateRotation(const DirectX::SimpleMath::Quaternion& currentAngle)
{
	using namespace DirectX::SimpleMath;
	// 目標回転を計算（速度ベクトルから）
	Quaternion targetQuat;


	// 現在の速度がゼロでない場合、回転を計算
	if (m_currentVelocity.LengthSquared() > 0.0001f)
	{
		// 現在の速度ベクトルからヨー角を計算
		float yaw = atan2f(m_currentVelocity.x, m_currentVelocity.z);
		// ヨー角からクォータニオンを作成
		targetQuat = Quaternion::CreateFromYawPitchRoll(yaw, 0.0f, 0.0f);
	}
	else
	{
		// 速度がゼロの場合は、回転なし
		targetQuat = Quaternion::Identity;
	}
	// 現在の回転角を更新する
	float rotateSpeed = 0.05f;
	// 滑らかに回転させるために、現在の回転角と目標回転角を補間
	m_rotationMiniCharacterAngle = Quaternion::Slerp(m_rotationMiniCharacterAngle, targetQuat, rotateSpeed);
	// 揺れを加味した回転を適用
	m_currentAngle = currentAngle * m_initialAngle * m_rotationMiniCharacterAngle * m_shakeQuaternion;
}

/*
*	@brief タイルの中心にいるかどうかを判定する
*	@details プレイヤーの位置がタイルの中心に近いかどうかを判定する。
*	@param charPos プレイヤーの位置
*	@param tileCenter タイルの中心位置
*	@param epsilon 判定の許容誤差
*	@return タイルの中心にいる場合はtrue、そうでない場合はfalse
*/
bool MiniCharacterSelectStage::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	//// タイルの中心とプレイヤーの位置の距離を計算
	float distance = (charPos - tileCenter).Length();
	//// 距離が許容誤差以下であれば、タイルの中心にいると判断
	return distance < epsilon;
}

