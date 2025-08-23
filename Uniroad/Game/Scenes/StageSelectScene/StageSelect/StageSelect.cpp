/*
*	@file StageSelect.cpp
*	@brief ステージセレクトシーンのステージセレクトクラス
*/
#include "pch.h"
#include "StageSelect.h"
/*
*	@brief コンストラクタ
*	@details ステージセレクトクラスのコンストラクタ
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
StageSelect::StageSelect(CommonResources* resources)
	: m_pCommonResources(resources) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルポインター
	, m_pShadowMapLight(nullptr) // シャドウマップライトへのポインタ
{
}
/*
*	@brief デストラクタ
*	@details ステージセレクトクラスのデストラクタ
*	@param なし
*	@return なし
*/
StageSelect::~StageSelect()
{

	// 共通リソースの解放は不要
}
/*
*	@brief 初期化
*	@details ステージセレクトクラスの初期化を行う
*	@param なし
*	@return なし
*/
void StageSelect::Initialize()
{
	// モデルの取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageSelect");
}
/*
*	@brief 更新
*	@details ステージセレクトクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void StageSelect::Update(float elapsedTime)
{
	// 更新処理は特にない
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// シャドウマップライトの更新
	m_pShadowMapLight->Update(elapsedTime);
}
/*
*	@brief 描画
*	@details ステージセレクトクラスの描画を行う
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void StageSelect::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	const auto& context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	const auto& states = m_pCommonResources->GetCommonStates();
	// モデルの描画
	m_pModel->Draw(context, *states, Matrix::Identity, view, proj, false, [&]
		{
			// 影を加味したライティング
			m_pShadowMapLight->ApplyShader(context, states);
		});
}
/*
*	@brief 終了
*	@details ステージセレクトクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void StageSelect::Finalize()
{
	// モデルが存在する場合は解放する
	if (m_pModel) m_pModel = nullptr;
}
