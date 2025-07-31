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
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageSelect");
}

void StageSelect::Update(float elapsedTime)
{
}

void StageSelect::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	const auto& context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	const auto& states = m_pCommonResources->GetCommonStates();
	const auto& device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// モデルの描画
	m_pModel->Draw(context, *states, Matrix::Identity, view, proj);
}

void StageSelect::Finalize()
{
}
