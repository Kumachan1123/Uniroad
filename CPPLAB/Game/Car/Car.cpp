#include "pch.h"
#include "Car.h"

Car::Car()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_material(nullptr) // マテリアルへのポインタ
{
}

void Car::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスを取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// モデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Car");

	// マテリアルを作る
	m_material = new Material();
	// マテリアルのシェーダーの名前を設定する
	m_material->SetShaderName(L"MetallicNoTex");
	// マテリアルを初期化する
	m_material->Initialize(m_pCommonResources);
	//// ノーマルマップをロードする
	//m_material->SetNormalMap(m_pCommonResources->GetTextureManager()->GetTexture("NormalMap"));
	//// 発光マップをロードする
	//m_material->SetEmissiveMap(m_pCommonResources->GetTextureManager()->GetTexture("EmissiveMap"));
	//// アンビエントオクルージョンマップをロードする
	//m_material->SetAO(m_pCommonResources->GetTextureManager()->GetTexture("AOMap"));
	//// 粗さマップをロードする
	//m_material->SetRoughnessMap(m_pCommonResources->GetTextureManager()->GetTexture("RoughnessMap"));
}

void Car::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}

void Car::Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto commonStates = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(0.1f);
	world *= Matrix::CreateTranslation(0, 0, 0);
	// モデルの描画
	m_pModel->Draw(context, *commonStates, world, view, proj, false, [&]()
				   {
					   // 両面描画
					   ID3D11RasterizerState* rasterizerState[1];
					   rasterizerState[0] = commonStates->CullNone();
					   context->RSSetState(rasterizerState[0]);
					   /// 使う各テクスチャをセット
					   // t0: ベースカラー（デフォルトで多分ついてる）
					   // 半透明描画を可能にする
					   ID3D11BlendState* blendState = commonStates->AlphaBlend();
					   context->OMSetBlendState(blendState, nullptr, 0xffffffff);
					   //// ラップ
					   //ID3D11SamplerState* sampler = commonStates->LinearWrap();
					   //context->PSSetSamplers(0, 1, &sampler);                // s0: ベースカラー用
					   // ノーマルマップ、発光マップ、AOマップ、粗さマップをセットし、シェーダーに渡す
					   m_material->SetShaders(context, commonStates);


				   });
}

void Car::Finalize()
{	// マテリアルの開放
	if (m_material)
	{
		delete m_material;
		m_material = nullptr;
	}
}
