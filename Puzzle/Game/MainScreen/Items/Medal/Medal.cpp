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
	:m_pCommonResources(nullptr) // 共通リソースへのポインタ
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
	m_worldMatrix = Matrix::CreateScale(m_itemInfo.scale) *
		Matrix::CreateFromQuaternion(m_rotation) *
		Matrix::CreateTranslation(m_position);
	// レンダリング
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
