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
	// モデルの取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("StageSelect");
	// 深度ステンシルの設定
	CreateDepthStencilBuffer(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
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
			// ブレンドステートを設定する
			context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
			// 深度ステンシルステートを設定する
			context->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
			// カリングを設定する
			context->RSSetState(states->CullNone());
			// テクスチャサンプラを適用する
			ID3D11SamplerState* sampler = states->PointWrap();
			context->PSSetSamplers(0, 1, &sampler);
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
	// 深度ステンシルステートを解放する
	m_pDepthStencilState.Reset();
}
/*
*	@brief 深度ステンシルバッファを作成する
*	@details ステージセレクトの深度ステンシルバッファを作成する
*	@param pDevice Direct3Dデバイスへのポインタ
*	@return なし
*/
void StageSelect::CreateDepthStencilBuffer(ID3D11Device* pDevice)
{
	// 深度ステンシル情報を定義する
	D3D11_DEPTH_STENCIL_DESC desc = {};
	// 床：床描画時にステンシルバッファの値をインクリメントする
	desc.DepthEnable = true;									// 深度テストを行う
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 深度バッファを更新する
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// 深度値以下なら更新する

	desc.StencilEnable = true;									// ステンシルテストを行う
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff（マスク値）
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff（マスク値）

	// ポリゴンの表面の設定
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// 裏面も同じ設定
	desc.BackFace = desc.FrontFace;
	// 深度ステンシルステートを作成する
	DX::ThrowIfFailed(pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf()));
}
