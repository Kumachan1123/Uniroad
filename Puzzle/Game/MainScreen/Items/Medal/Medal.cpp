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
	, m_itemInfo() // アイテム情報
	, m_pModel(nullptr) // モデルへのポインタ
	, m_time(0.0f) // 経過時間
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
	// 時間経過で回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time * DirectX::XM_PI * 2.0f / 5.0f, 0.0f, 0.0f);
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
	// ミニキャラクターのメダル枚数を加算
	miniCharacter->GetParent()->GetCSVItem()->CountMedals();
}

void Medal::OnUse(MiniCharacter* miniCharacter)
{
	UNREFERENCED_PARAMETER(miniCharacter);
}

void Medal::OnDiscard(MiniCharacter* miniCharacter)
{
	UNREFERENCED_PARAMETER(miniCharacter);
}
