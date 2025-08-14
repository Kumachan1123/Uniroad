/*
*	@file Shadow.cpp
*	@brief 影ポリゴンを描画するクラスの実装ファイル
*/
#include "pch.h"
#include "Shadow.h"
/*
*	@brief コンストラクタ
*	@details 影ポリゴンを描画するクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Shadow::Shadow()
	: m_pTexture(nullptr) // テクスチャ
	, m_pPrimitiveBatch{} // プリミティブバッチ
	, m_pBasicEffect{} // ベーシックエフェクト
	, m_pInputLayout{} // インプットレイアウト
	, m_pDepthStencilState{} // 深度ステンシルステート
	, m_pCommonResources(nullptr) // 共通リソースへのポインタ
{
}
/*
*	@brief 初期化
*	@details 影ポリゴンを描画するクラスの初期化
*	@param pCommonResources 共通リソースへのポインタ
*	@return なし
*/
void Shadow::Initialize(CommonResources* resources)
{
	// DirectX::名前空間を使用
	using namespace DirectX;
	// 共通リソースへのポインタをセット
	m_pCommonResources = resources;
	// リソースからデバイスを取得
	ID3D11Device* pDevice = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキストを取得
	ID3D11DeviceContext* context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	//// 共通ステートを取得
	//CommonStates* states = m_pCommonResources->GetCommonStates();
	// ベーシックエフェクトを設定(頂点カラーなし、テクスチャ有効、ライティング無効)
	m_pBasicEffect = std::make_unique<BasicEffect>(pDevice);
	m_pBasicEffect->SetVertexColorEnabled(false);
	m_pBasicEffect->SetTextureEnabled(true);
	m_pBasicEffect->SetLightingEnabled(false);
	// 入力レイアウトを設定する
	DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionTexture>(pDevice, m_pBasicEffect.get(), m_pInputLayout.ReleaseAndGetAddressOf()));
	// プリミティブバッチを作成
	m_pPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	// 画像を読み込む
	m_pTexture = resources->GetTextureManager()->GetTexture("Shadow");
	// 深度ステンシルバッファを設定する
	CreateDepthStenciBuffer(pDevice);
}
/*
*	@brief 描画
*	@details 影ポリゴンを描画する
*	@param view ビューマトリックス
*	@param projection プロジェクションマトリックス
*	@param position 影ポリゴンの位置
*	@param radius 影ポリゴンの半径(デフォルトは0.7f)
*/
void Shadow::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::SimpleMath::Vector3& position,
	float radius)
{
	// DirectX・DirectX::SimpleMath名前空間を使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// アルファブレンドを適用する
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);

	// 深度ステンシルステートを適用する
	context->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);	// 参照値：1

	// カリングを設定する
	context->RSSetState(states->CullCounterClockwise());

	// テクスチャサンプラを適用する
	ID3D11SamplerState* sampler = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, &sampler);

	// 入力レイアウトを適用する
	context->IASetInputLayout(m_pInputLayout.Get());

	// エフェクトを設定し適用する
	m_pBasicEffect->SetWorld(Matrix::Identity);
	m_pBasicEffect->SetView(view);
	m_pBasicEffect->SetProjection(projection);
	m_pBasicEffect->SetTexture(m_pTexture.Get());
	m_pBasicEffect->Apply(context);

	// 影ポリゴンの頂点情報を定義する
	VertexPositionTexture vertices[] = {
		VertexPositionTexture(Vector3::Zero,Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3::Zero,Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3::Zero,Vector2(0.0f, 1.0f)),
		VertexPositionTexture(Vector3::Zero,Vector2(1.0f, 1.0f)),
	};
	// 影ポリゴンのインデックス情報を定義する：DrawIndexedで描画するときに必要
	uint16_t indexes[] = { 0, 1, 2, 1, 3, 2 };
	// 影ポリゴンの頂点情報を設定する
	vertices[0].position = Vector3(-radius, 1.0f, -radius) + position;
	vertices[1].position = Vector3(radius, 1.0f, -radius) + position;
	vertices[2].position = Vector3(-radius, 1.0f, radius) + position;
	vertices[3].position = Vector3(radius, 1.0f, radius) + position;
	// 描画開始
	m_pPrimitiveBatch->Begin();
	// 影ポリゴンを描画する
	m_pPrimitiveBatch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indexes,
		_countof(indexes),
		vertices,
		_countof(vertices)
	);

	// 描画終了
	m_pPrimitiveBatch->End();
}
/*
*	@brief 深度ステンシルバッファを設定する
*	@details 影ポリゴンを描画するための深度ステンシルバッファを設定する
*	@param pDevice Direct3Dデバイスへのポインタ
*	@return なし
*/
void Shadow::CreateDepthStenciBuffer(ID3D11Device* pDevice)
{
	// 深度ステンシルステートの設定を行う
	D3D11_DEPTH_STENCIL_DESC desc{};
	// 影：描画先のステンシルバッファと参照値が同じとき影を描画する
	desc.DepthEnable = true;									// 深度テストを行う
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 深度バッファを更新する
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// 深度値以下なら更新する
	desc.StencilEnable = true;									// ステンシルテストを行う
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff
	// 表面
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// 参照値とステンシル値が同値なら
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// OK　何もしない
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG　何もしない
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG　何もしない
	// 裏面も同じ設定
	desc.BackFace = desc.FrontFace;
	// 深度ステンシルステートを作成する
	pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf());

}
