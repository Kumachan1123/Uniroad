/*
*		@file	 Stage.cpp
*		@brief	 ステージクラス
*/
#include "pch.h"
#include "Stage.h"
/*
*		@brief	 コンストラクタ
*		@note	 共通リソースへのポインタとモデルへのポインタをnullptrで初期化
*		@return	 なし
*/
Stage::Stage()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
{
}
/*
*		@brief	 初期化
*		@note	 モデルを取得し、モデルにデフォルトのフォグを設定
*		@param[in] resources 共通リソースへのポインタ
*		@return	 なし
*/
void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// モデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Area1");
	// モデルにデフォルトのフォグを設定
	m_pModel->UpdateEffects([&](IEffect* effect)
							{
								auto fog = dynamic_cast<BasicEffect*>(effect);
								if (fog)
								{
									fog->SetFogEnabled(true);
									fog->SetFogStart(500.0f);
									fog->SetFogEnd(1000.0f);
									fog->SetFogColor(DirectX::Colors::GhostWhite);

								}

							});
	// マテリアルを作る
	m_material = new Material();
	// マテリアルのシェーダーの名前を設定する
	m_material->SetShaderName(L"Stage");
	// マテリアルを初期化する
	m_material->Initialize(m_pCommonResources);
}
/*
*		@brief	 更新
*		@note	 今回は特に何もしない
*		@param[in] elapsedTime 経過時間
*		@return	 なし
*/
void Stage::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*		@brief	 描画
*		@note	 ワールド行列を設定してモデルを描画。描画の際に両面描画にする
*		@param[in] view ビュー行列
*		@param[in] proj 射影行列
*		@return	 なし
*/
void Stage::Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto commonStates = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(.5f);
	world *= Matrix::CreateTranslation(0.0f, -150.0f, 0.0f);
	// モデルの描画
	m_pModel->Draw(context, *commonStates, world, view, proj, false, [&]()
				   {
					   // 両面描画にする
					   ID3D11RasterizerState* rasterizerState[1];
					   rasterizerState[0] = commonStates->CullNone();
					   context->RSSetState(rasterizerState[0]);
					   // 頂点シェーダーを設定する
					   context->VSSetShader(m_material->GetVertexShader().Get(), nullptr, 0);
					   // ピクセルシェーダーを設定する
					   context->PSSetShader(m_material->GetPixelShader().Get(), nullptr, 0);

				   });
}

/*
*		@brief	 終了
*		@note	 今回は特に何もしない
*		@return	 なし
*/
void Stage::Finalize()
{
	// マテリアルの開放
	if (m_material)
	{
		delete m_material;
		m_material = nullptr;
	}
}
