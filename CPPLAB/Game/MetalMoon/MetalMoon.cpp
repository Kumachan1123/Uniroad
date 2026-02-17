/*
*	@file MetalMoon.cpp
*	@brief MetalMoonクラスの実装
*/
#include "pch.h"
#include "MetalMoon.h"

MetalMoon::MetalMoon()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_material(nullptr) // マテリアルへのポインタ
{
}

void MetalMoon::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// モデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("MetalMoon");

	// マテリアルを作る
	m_material = new Material();
	// マテリアルのシェーダーの名前を設定する
	m_material->SetShaderName(L"Metallic");
	// マテリアルを初期化する
	m_material->Initialize(m_pCommonResources);
}

void MetalMoon::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void MetalMoon::Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto commonStates = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(5);
	world *= Matrix::CreateTranslation(0, 0, 0);
	// モデルの描画
	m_pModel->Draw(context, *commonStates, world, view, proj, false, [&]()
				   {
					   // 両面描画
					   ID3D11RasterizerState* rasterizerState[1];
					   rasterizerState[0] = commonStates->CullNone();
					   context->RSSetState(rasterizerState[0]);

					   // 頂点・ピクセルシェーダーセット
					   context->VSSetShader(m_material->GetVertexShader().Get(), nullptr, 0);
					   context->PSSetShader(m_material->GetPixelShader().Get(), nullptr, 0);

					   //  環境マップSRV・サンプラーをシェーダに渡す
					   ID3D11ShaderResourceView* envCubeSRV = m_material->GetEnvironmentCubeSRV(); // ←Material等に保持
					   if (envCubeSRV)
						   context->PSSetShaderResources(1, 1, &envCubeSRV);     // t1: キューブマップ
					   ID3D11SamplerState* envSampler = commonStates->LinearWrap(); // or custom sampler
					   context->PSSetSamplers(1, 1, &envSampler);                // s1: キューブマップ用
				   });
}

void MetalMoon::Finalize()
{
	// マテリアルの開放
	if (m_material)
	{
		delete m_material;
		m_material = nullptr;
	}
}
