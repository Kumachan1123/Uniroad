/*
	@file	GridFloor.cpp
	@brief	グリッド床を描画するクラス
*/
#include "pch.h"
#include "GridFloor.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
mylib::GridFloor::GridFloor(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::FXMVECTOR& color,
	float size,
	size_t divs
)
	:
	m_commonStates(states),
	m_color{ color },
	m_size{ size },
	m_divs{ divs }
{
	assert(device);
	assert(context);
	assert(states);

	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetTextureEnabled(false);

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_pInputLayout.ReleaseAndGetAddressOf()
		)
	);
}

//---------------------------------------------------------
// グリッド床を描画する
//---------------------------------------------------------
void mylib::GridFloor::Render(
	ID3D11DeviceContext* context,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	assert(context);

	// ブレンドステート、デプスステンシルステート、カリングを指定する
	context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);
	context->RSSetState(m_commonStates->CullNone());

	// 変換行列を設定する
	Matrix world{ Matrix::Identity };
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);

	// 入力レイアウトを指定する
	context->IASetInputLayout(m_pInputLayout.Get());

	// グリッド床を描画する
	m_primitiveBatch->Begin();
	DX::DrawGrid(
		m_primitiveBatch.get(),
		Vector3(m_size / 2.0f, 0.0f, 0.0f),	// 横軸
		Vector3(0.0f, 0.0f, m_size / 2.0f),	// 縦軸
		Vector3::Zero,						// origin
		m_divs,								// xdivs
		m_divs,								// ydivs
		m_color								// color
	);
	m_primitiveBatch->End();
}
