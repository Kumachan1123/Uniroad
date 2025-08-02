/*
*	@file GoalLock.cpp
*	@brief ゴールロックの処理を定義するクラス
*/
#include "pch.h"
#include "GoalLock.h"
/*
*	@brief コンストラクタ
*	@details ゴールロックのコンストラクタ
*	@param なし
*	@return なし
*/
GoalLock::GoalLock()
	:m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pMiniCharacter(nullptr) // ミニキャラクターへのポインタ
	, m_itemInfo() // アイテム情報
	, m_row(-1) // 行番号（保存用）
	, m_col(-1) // 列番号（保存用）
	, m_pModel(nullptr) // モデルへのポインタ
	, m_time(0.0f) // 経過時間
	, m_deleteTime(0.0f) // 獲得されて消えるまでの時間
	, m_rotationSpeed(0.0f) // 回転速度
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 回転
	, m_isCollected(false) // 獲得されたかどうか
{
}

/*
*	@brief デストラクタ
*	@details ゴールロックのデストラクタ
*	@param なし
*	@return なし
*/
GoalLock::~GoalLock()
{
	// 何もしない
}

/*
*	@brief 初期化
*	@details ゴールロックの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param info アイテム情報
*	@return なし
*/
void GoalLock::Initialize(CommonResources* resources, const ItemInfo& info)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// アイテム情報を設定
	m_itemInfo = info;
}
/*
*	@brief 更新
*	@details ゴールロックの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void GoalLock::Update(float elapsedTime)
{
	// 経過時間を加算
	m_time += elapsedTime;
}
/*
*	@brief ゴールロックの描画
*	@details ゴールロックの描画処理を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void GoalLock::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
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
*	@details ゴールロックを取得したときの処理を行う
*	@param miniCharacter ミニキャラクターへのポインタ
*	@return なし
*/
void GoalLock::OnGet(MiniCharacter* miniCharacter)
{
	// ミニキャラクターへのポインタを設定
	m_pMiniCharacter = miniCharacter;
	// 獲得されたフラグを設定
	m_isCollected = true;
	// 獲得されて消えるまでの時間をリセット
	m_deleteTime = 0.0f;
}
/*
*	@brief アイテムの使用時処理
*	@details ゴールロックを使用したときの処理を行う
*	@param miniCharacter ミニキャラクターへのポインタ
*	@return なし
*/
void GoalLock::OnUse(MiniCharacter* miniCharacter)
{
	UNREFERENCED_PARAMETER(miniCharacter);
}

/*
*	@brief アイテムの破棄時処理
*	@details ゴールロックを破棄したときの処理を行う
*	@param miniCharacter ミニキャラクターへのポインタ
*	@return なし
*/
void GoalLock::OnDiscard(MiniCharacter* miniCharacter)
{
	// アイテムを削除
	m_pMiniCharacter->GetParent()->GetCSVItem()->RemoveItem(m_row, m_col);
}