/*
*	@file StageGate.cpp
*	@brief ステージゲートクラス
*/
#include "pch.h"
#include "StageGate.h"
/*
*	@brief コンストラクタ
*	@details ステージゲートクラスのコンストラクタ
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
StageGate::StageGate(CommonResources* resources)
	: m_pCommonResources(resources) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルポインター
{
}
/*
*	@brief デストラクタ
*	@details ステージゲートクラスのデストラクタ
*	@param なし
*	@return なし
*/
StageGate::~StageGate()
{
	// 終了処理を呼び出す
	Finalize();
}
/*
*	@brief 初期化
*	@details ステージゲートクラスの初期化を行う
*	@param なし
*	@return なし
*/
void StageGate::Initialize()
{
	// モデルの取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageGate");
}
/*
*	@brief 更新
*	@details ステージゲートクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void StageGate::Update(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*	@brief 描画
*	@details ステージゲートクラスの描画を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void StageGate::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	const auto& context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	const auto& states = m_pCommonResources->GetCommonStates();
	m_pModel->Draw(context, *states, Matrix::Identity * Matrix::CreateTranslation(m_position), view, proj, false);
}
/*
*	@brief 終了
*	@details ステージゲートクラスの終了を行う
*	@param なし
*	@return なし
*/
void StageGate::Finalize()
{
	// モデルの解放
	m_pModel = nullptr;
}
