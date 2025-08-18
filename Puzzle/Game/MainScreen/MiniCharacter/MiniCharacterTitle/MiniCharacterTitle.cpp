/*
*	@file MiniCharacterTitle.cpp
*	@brief タイトルシーン用のミニキャラを管理するクラス
*/
#include "pch.h"
#include "MiniCharacterTitle.h"
// ノードカウンター
int MiniCharacterTitle::s_nodeCount = 0;
// 部品カウンター
int MiniCharacterTitle::s_partsNumber = 0;

/*
*	@brief コンストラクタ
*	@details タイトルシーン用のミニキャラの初期位置と角度を設定し、必要な変数を初期化する。
*	@param parent 親コンポーネント
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度（ラジアン）
*	@return なし
*/
MiniCharacterTitle::MiniCharacterTitle(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_parent(parent)// 親コンポーネント
	, m_nodeNumber(MiniCharacterTitle::GetNodeCountAfterCountUp())// ノード番号
	, m_partNumber(MiniCharacterTitle::GetPartsNumber())// 部品番号
	, m_partID(MiniCharacterTitle::MINICHARACTER)// 部品ID
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
	, m_titleAnimationState(NONE)// タイトルシーンにおけるアニメーションステート
{
}
/*
*	@brief デストラクタ
*	@details タイトルシーン用のミニキャラの後処理を行う。
*	@param なし
*	@return なし
*/
MiniCharacterTitle::~MiniCharacterTitle()
{
	Finalize();
}
/*
*	@brief タイトルシーン用のミニキャラの初期化を行う
*	@details タイトルシーン用のミニキャラの初期化を行い、必要なリソースを設定する。
*	@param commonResources 共通リソース
*	@return なし
*/
void MiniCharacterTitle::Initialize(CommonResources* commonResources)
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// 共通リソースが存在することを確認する
	assert(commonResources);
	// 共通リソースを設定する
	m_pCommonResources = commonResources;
	// 現在位置に反映
	m_currentPosition = m_initialPosition;
	// ヒツジパーツをアタッチ
	Attach(std::make_unique<Sheep>(this, Vector3(0.0f, 3.5f, 0.0f), 0.0f));
	// 影を作成
	m_pShadow = std::make_unique<Shadow>();
	// 影の初期化
	m_pShadow->Initialize(m_pCommonResources);
	// パーティクルを作成する
	m_pParticle = std::make_unique<Particle>(Utility::Type::STEAM, 1.0f);
	// パーティクルを初期化する
	m_pParticle->Initialize(m_pCommonResources);
}
/*
*	@brief タイトルシーン用のミニキャラの更新処理を行う
*	@details タイトルシーン用のミニキャラの位置や角度を更新する。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の角度
*	@return なし
*/
void MiniCharacterTitle::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
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
	// タイトルシーン用アニメーション
	ExecuteAnimation(elapsedTime);
	// 部品を更新する　
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	// パーティクルの更新

	m_pParticle->SetParams(SetParticleParams());
	m_pParticle->Update(elapsedTime);

}
/*
*	@brief タイトルシーン用のミニキャラの追加処理をする
*	@details タイトルシーン用のミニキャラに部品を追加する。
*	@param MiniCharacterPart 追加する部品
*	@return なし
*/
void MiniCharacterTitle::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品番号を取得
	MiniCharacterPart->Initialize(m_pCommonResources);
	// 部品番号を設定
	m_pMiniCharacterParts.push_back(std::move(MiniCharacterPart));
}
/*
*	@brief タイトルシーン用のミニキャラの部品を削除する
*	@details タイトルシーン用のミニキャラから部品を削除する。
*	@param MiniCharacterPart 削除する部品
*	@return なし
*/
void MiniCharacterTitle::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品を削除する処理は今は何もしない
}
/*
*	@brief タイトルシーン用のミニキャラの描画処理を行う
*	@details タイトルシーン用のミニキャラを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void MiniCharacterTitle::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{

	// 部品を描画する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Render(view, proj);
	// 影を描画する
	m_pShadow->Render(view, proj, m_currentPosition, 1.0f);
	// 親コンポーネントのポインターに変換
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// パーティクルのビルボード行列を作成
	m_pParticle->CreateBillboard(parent->GetCamera()->GetTargetPosition(), parent->GetCamera()->GetEyePosition(), parent->GetCamera()->GetUpPosition());
	// パーティクル描画
	m_pParticle->Render(parent->GetCamera()->GetViewMatrix(), parent->GetCamera()->GetProjectionMatrix());

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
*	@brief タイトルシーン用のミニキャラの後処理を行う
*	@details タイトルシーン用のミニキャラの後処理を行う。
*	@param なし
*	@return なし
*/
void MiniCharacterTitle::Finalize()
{
}
/*
*	@brief タイトルシーン用のミニキャラの回転を補間する
*	@details タイトルシーン用のミニキャラの回転を補間する。
*	@param currentAngle 現在の回転角
*	@return なし
*/
void MiniCharacterTitle::InterpolateRotation(const DirectX::SimpleMath::Quaternion& currentAngle)
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
*	@brief タイルの中心にいるかどうかを確認する
*	@details タイルの中心にいるかどうかを確認する。
*	@param charPos キャラクターの位置
*	@param tileCenter タイルの中心位置
*	@param epsilon 許容誤差
*	@return タイルの中心にいる場合はtrue、そうでない場合はfalse
*/
bool MiniCharacterTitle::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	//// タイルの中心とプレイヤーの位置の距離を計算
	float distance = (charPos - tileCenter).Length();
	//// 距離が許容誤差以下であれば、タイルの中心にいると判断
	return distance < epsilon;
}
/*
*	@brief タイトルシーン用のアニメーションを実行する
*	@details タイトルシーン用のアニメーションを実行する。
*	@param elapsedTime 経過時間
*	@return なし
*/
void MiniCharacterTitle::ExecuteAnimation(float elapsedTime)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// 目標座標を初期化
	// STARTでつかう目標座標
	Vector3 targetPosition(0.0f, -0.45f, 0.0f);
	// CONTINUEでつかう目標座標
	Vector3 goalPosition(10.0f, -0.45f, 0.0f);
	// アニメーションの実行
	switch (GetTitleAnimationState())
	{
	case TitleAnimation::START:
		// 開始アニメーションの処理
		// 現在の位置を目標座標に向けて補間
		m_currentPosition = Vector3::Lerp(m_currentPosition, targetPosition, elapsedTime);
		// 目的地に到達したらアニメーションを待機状態に変更
		if ((m_currentPosition - targetPosition).Length() < 0.1f)SetTitleAnimationState(WAIT);
		break;
	case TitleAnimation::WAIT:
		// 待機アニメーションの処理
		break;
	case TitleAnimation::CONTINUE:
		// アニメーション再開の処理
		// 現在の位置を目標座標に向けて補間
		m_currentPosition = Vector3::Lerp(m_currentPosition, goalPosition, elapsedTime * 4.0f);
		// 目的地に到達したらアニメーションを待機状態に変更
		if ((m_currentPosition - goalPosition).Length() < 0.5f)
			SetTitleAnimationState(END);
		break;
	case TitleAnimation::END:
		// アニメーション終了なので何もしない
		break;
	case TitleAnimation::NONE:
		// アニメーションがない場合は何もしない
		return;
	}
}
/*
*	@brief パーティクルパラメータを設定する
*	@details パーティクルパラメータを設定する
*	@param なし
*	@return パーティクルのパラメータ
*/
Utility::ParticleParams MiniCharacterTitle::SetParticleParams() const
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 乱数の設定
	std::random_device seed;
	// メルセンヌ・ツイスタ法
	std::default_random_engine engine(seed());
	// ランダムな角度
	std::uniform_real_distribution<> angleDist(0, XM_2PI);
	// ランダムな速度の範囲を設定
	std::uniform_real_distribution<> speedDist(0.5f, 2.0f);
	// XY平面上のランダムな角度
	float randAngleXY = static_cast<float>(angleDist(engine));
	// XZ平面上のランダムな角度
	float randAngleXZ = static_cast<float>(angleDist(engine));
	// ランダムな速度
	float speed = static_cast<float>(speedDist(engine));
	// ランダムな方向の速度ベクトル
	Vector3 randomVelocity = speed * Vector3(
		cosf(randAngleXY) * sinf(randAngleXZ),	 // X成分s
		1.0f,									 // Y成分
		sinf(randAngleXY) * sinf(randAngleXZ)	 // Z成分
	);
	// パーティクルのパラメーターを設定
	Utility::ParticleParams params{};
	params.life = 1.0f;
	params.pos = m_currentPosition + Vector3(0.0f, 1.0f, 0.0f);
	params.velocity = randomVelocity;
	params.accele = Vector3(-5.0f, 0.0f, 0.0f);// 加速度
	params.rotateAccele = Vector3::One; // 回転加速度
	params.rotate = Vector3(0.0f, 0.0f, 0.0f); // 初期回転
	params.startScale = Vector3(1.0f, 1.0f, 0.0f); // 初期スケール
	params.endScale = Vector3(0.01f, 0.01f, 0.0f); // 最終スケール（小さくなる）
	params.startColor = Vector4(1, 1, 1, 1); // 初期カラー（白）
	params.endColor = Vector4(1, 1, 0, 0); // 最終カラー（白→透明）
	params.type = Utility::Type::STEAM; // パーティクルのタイプ
	return params;
}
/*
*	@brief タイトルシーン用のアニメーション状態を取得する
*	@details タイトルシーン用のアニメーション状態を取得する。
*	@param なし
*	@return タイトルアニメーションの状態
*/
TitleAnimation MiniCharacterTitle::GetTitleAnimationState() const
{
	// 親コンポーネントのポインターに変換
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// 親コンポーネントが存在することを確認
	assert(parent);
	// アニメーション状態を返す
	return parent->GetTitleAnimationState();

}
/*
*	@brief タイトルシーン用のアニメーション状態を設定する
*	@details タイトルシーン用のアニメーション状態を設定する。
*	@param state タイトルアニメーションの状態
*	@return なし
*/
void MiniCharacterTitle::SetTitleAnimationState(TitleAnimation state)
{
	// 親コンポーネントのポインターに変換
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// 親コンポーネントが存在することを確認
	assert(parent);
	// アニメーション状態を設定する
	parent->SetTitleAnimationState(state);
}