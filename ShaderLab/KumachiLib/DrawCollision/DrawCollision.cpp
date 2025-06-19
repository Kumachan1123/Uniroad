#include "pch.h"
#include "DrawCollision.h"


// 静的メンバーの初期化
std::unique_ptr<DirectX::BasicEffect> DrawCollision::m_basicEffect = nullptr;
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawCollision::m_pInputLayout = nullptr;
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> DrawCollision::m_primitiveBatch = nullptr;
CommonResources* DrawCollision::m_commonResources = nullptr;

// 初期化
void DrawCollision::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定
	// デバイスとコンテキストを取得
	m_commonResources = commonResources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_pInputLayout.ReleaseAndGetAddressOf()
		)
	);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
}

void DrawCollision::DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// 描画する
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_pInputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	m_primitiveBatch->Begin();

}

void DrawCollision::DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color)
{
	// 境界球を描画
	DX::Draw(m_primitiveBatch.get(), bs, color);
}

void DrawCollision::DrawBoundingBox(DirectX::BoundingBox& bb, DirectX::XMVECTOR color)
{
	// 境界ボックスを描画
	DX::Draw(m_primitiveBatch.get(), bb, color);
}

void DrawCollision::DrawEnd()
{
	m_primitiveBatch->End();
}


void DrawCollision::Finalize()
{
	// 静的メンバーを解放
	m_basicEffect.reset();
	m_pInputLayout.Reset();
	m_primitiveBatch.reset();
	m_commonResources = nullptr;
}
