/*
*	@file Medal.cpp
*	@brief メダルクラス
*/
#include "pch.h"
#include "Medal.h"
/*
*	@brief コンストラクタ
*	@details メダルクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Medal::Medal()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pCamera(nullptr) // カメラへのポインタ
	, m_pParticle(nullptr) // パーティクルシステムへのポインタ
	, m_pMiniCharacter(nullptr) // ミニキャラクターへのポインタ
	, m_itemInfo() // アイテム情報
	, m_row(-1) // 行番号（保存用）
	, m_col(-1) // 列番号（保存用）
	, m_pModel(nullptr) // モデルへのポインタ
	, m_time(0.0f) // 経過時間
	, m_deleteTime(0.0f) // メダルが獲得されて消えるまでの時間
	, m_rotationSpeed(0.0f) // 回転速度
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 回転
	, m_isCollected(false) // メダルが獲得されたかどうか
{
}
/*
*	@brief デストラクタ
*	@details メダルクラスのデストラクタ
*	@param なし
*	@return なし
*/
Medal::~Medal()
{
	// 何もしない
}
/*
*	@brief 初期化
*	@details メダルクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param info アイテム情報
*	@return なし
*/
void Medal::Initialize(CommonResources* resources, const ItemInfo& info)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// アイテム情報を設定
	m_itemInfo = info;
	// 初期の回転速度を設定
	m_rotationSpeed = DEFAULT_ROTATION_SPEED;
	// パーティクルを作成する
	m_pParticle = std::make_unique<Particle>(Utility::Type::SHINE, 50);
	// パーティクルを初期化する
	m_pParticle->Initialize(m_pCommonResources);
	// 影を作成する
	m_pShadow = std::make_unique<Shadow>();
	// 影にモデルを渡す
	m_pShadow->SetModel(m_pModel);
	// 影を初期化
	m_pShadow->Initialize(m_pCommonResources);
}
/*
*	@brief 更新
*	@details メダルの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Medal::Update(float elapsedTime)
{
	// DirectX::SimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// 時間を加算
	m_time += elapsedTime;
	// 獲得されたら
	if (m_isCollected)
	{
		// 消えるまでの時間を加算
		m_deleteTime += elapsedTime;
		// Y座標を滑らかに変える
		m_position.y += Easing::EaseInCirc(m_deleteTime / 3.0f);
		// 獲得されたら消えるまでの時間が2秒を超えたら
		if (m_deleteTime > 2.0f)
		{
			// アイテムを削除
			OnDiscard(m_pMiniCharacter);
			// 処理を終える
			return;
		}
	}
	// 時間経過で回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time * DirectX::XM_PI * 2.0f / 5.0f * m_rotationSpeed, 0.0f, 0.0f);
	// パーティクルの更新
	m_pParticle->SetParams(SetParticleParams());
	m_pParticle->Update(elapsedTime);
}
/*
*	@brief 当たり判定描画
*	@details メダルの当たり判定を描画する
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void Medal::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX::SimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// モデルが設定されていない場合は何もしない
	if (m_pModel == nullptr) return;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通のステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定
	m_worldMatrix = Matrix::CreateScale(m_itemInfo.scale) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
	// パーティクルのビルボード行列を作成
	m_pParticle->CreateBillboard(GetCamera()->GetTargetPosition(), GetCamera()->GetEyePosition(), GetCamera()->GetUpPosition());
	// パーティクルの描画
	m_pParticle->Render(view, proj);
	// 影用に座標を定義
	Vector3 shadowPosition = m_position + Vector3(0.0f, -2.99f, 0.0f);
	// 影の描画
	if (!m_isCollected)m_pShadow->RenderCircleShadow(view, proj, shadowPosition, 1.0f);
	// モデルの描画
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);

}
/*
*	@brief アイテムの取得時処理
*	@details メダルを取得したとき、取った枚数を加算する
*	@param miniCharacter ミニキャラクターへのポインタ
*	@return なし
*/
void Medal::OnGet(MiniCharacter* miniCharacter)
{
	// 既に獲得されている場合は何もしない
	if (m_isCollected) return;
	// ミニキャラクターのメダル枚数を加算
	miniCharacter->GetParent()->GetCSVItem()->CountMedals();
	// 回転速度を獲得時の速度に設定
	m_rotationSpeed = COLLECTED_ROTATION_SPEED;
	// メダルが獲得されたフラグを設定
	m_isCollected = true;
	// ポインターを設定
	m_pMiniCharacter = miniCharacter;
}
/*
*	@brief アイテムの使用時処理
*	@details メダルを使用したときの処理を行う
*	@param miniCharacter ミニキャラクターへのポインタ
*	@return なし
*/
void Medal::OnUse(MiniCharacter* miniCharacter)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(miniCharacter);
}
/*
*	@brief アイテムの破棄時処理
*	@details メダルを破棄したときの処理を行う
*	@param miniCharacter ミニキャラクターへのポインタ
*	@return なし
*/
void Medal::OnDiscard(MiniCharacter* miniCharacter)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(miniCharacter);
	// アイテムを削除
	m_pMiniCharacter->GetParent()->GetCSVItem()->RemoveItem(m_row, m_col);
}
/*
*	@brief パーティクルのパラメーターを設定する
*	@details メダルのパーティクルのパラメーターを設定する
*	@param なし
*	@return パーティクルのパラメーター
*/
Utility::ParticleParams Medal::SetParticleParams() const
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
		0.5f,									 // Y成分
		sinf(randAngleXY) * sinf(randAngleXZ)	 // Z成分
	);
	// パーティクルのパラメーターを設定
	Utility::ParticleParams params{};
	params.life = 1.0f;
	params.pos = m_position;
	params.velocity = randomVelocity;
	params.accele = Vector3(0.0f, 0.0f, 0.0f);// 加速度
	params.rotateAccele = Vector3::One; // 回転加速度
	params.rotate = Vector3(0.0f, 0.0f, 0.0f); // 初期回転
	params.startScale = Vector3(1.0f, 1.0f, 0.0f); // 初期スケール
	params.endScale = Vector3(0.01f, 0.01f, 0.0f); // 最終スケール（小さくなる）
	params.startColor = Vector4(0, 1, 1, 0.75); // 初期カラー（白）
	params.endColor = Vector4(0, 0.25, 0.75, 0); // 最終カラー（白→透明）
	params.type = Utility::Type::SHINE; // パーティクルのタイプ
	return params;
}
