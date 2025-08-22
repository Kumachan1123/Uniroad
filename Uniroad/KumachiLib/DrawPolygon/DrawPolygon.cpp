/*
*	@file	DrawPolygon.cpp
*	@brief	板ポリゴン描画用クラス
*	@details 板ポリゴンを描画するためのクラス
*/
#include <pch.h>
#include "DrawPolygon.h"
// シングルトンインスタンスの初期化
std::unique_ptr<DrawPolygon> DrawPolygon::m_pInstance = nullptr;
/*
*	@brief シングルトンインスタンスを取得
*	@details シングルトンインスタンスを取得する
*	@param なし
*	@return シングルトンインスタンス
*/
DrawPolygon* const DrawPolygon::GetInstance()
{
	// インスタンスがない場合
	if (m_pInstance == nullptr)
	{
		// インスタンスを生成
		m_pInstance.reset(new DrawPolygon());
	}
	// インスタンスを返す
	return m_pInstance.get();
}

/*
*	@brief コンストラクタ
*	@details コンストラクタ
*	@param なし
*	@return なし
*/
DrawPolygon::DrawPolygon()
	: m_pPrimitiveBatchTexture(nullptr)// プリミティブバッチ(頂点、テクスチャ)
	, m_pPrimitiveBatchColorTexture(nullptr)// プリミティブバッチ(頂点、色、テクスチャ)
	, m_pStates(nullptr)// コモンステート
	, m_pDeviceContext(nullptr)	// デバイスコンテキスト
	, m_pDR(nullptr)// デバイスリソース
	, m_pDevice(nullptr)// デバイス
{
}
/*
*	@brief デストラクタ
*	@details デストラクタ
*	@param なし
*	@return なし
*/
DrawPolygon::~DrawPolygon()
{
	// プリミティブバッチの解放（頂点、テクスチャ）
	ReleasePositionTexture();
	// プリミティブバッチの解放（頂点、色、テクスチャ）
	ReleasePositionColorTexture();
}
/*
*	@brief 初期化
*	@details 初期化（頂点、テクスチャ）を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスを取得
	m_pDevice = pDR->GetD3DDevice();
	// デバイスコンテキストを取得
	m_pDeviceContext = pDR->GetD3DDeviceContext();
	// プリミティブバッチを作成する
	m_pPrimitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	// コモンステートを作成する
	m_pStates = std::make_unique<CommonStates>(m_pDevice);
}

/*
*	@brief 初期化
*	@details 初期化（頂点、色、テクスチャ）を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスを取得
	m_pDevice = pDR->GetD3DDevice();
	// デバイスコンテキストを取得
	m_pDeviceContext = pDR->GetD3DDeviceContext();
	// プリミティブバッチの作成
	m_pPrimitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());
	// コモンステートを作成する
	m_pStates = std::make_unique<CommonStates>(m_pDevice);
}
/*
*	@brief 描画開始
*	@details 描画開始時に呼び出す関数
*	@param pInputLayout 入力レイアウト
*	@param textures テクスチャ配列
*	@return なし
*/
void DrawPolygon::DrawStart(ID3D11InputLayout* pInputLayout, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures)
{
	// ピクセルシェーダにテクスチャを登録する
	for (int i = 0; i < textures.size(); i++)m_pDeviceContext->PSSetShaderResources(i, 1, textures[i].GetAddressOf());
	// インプットレイアウトの登録
	m_pDeviceContext->IASetInputLayout(pInputLayout);
}
/*
*	@brief 描画前設定
*	@details 描画前に各種ステートを設定する
*	@param ss サンプラーステート
*	@param bs ブレンドステート
*	@param rs ラスタライザーステート
*	@param dss 深度ステンシルステート
*	@return なし
*/
void DrawPolygon::DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss)
{
	// 画像用サンプラーの登録
	ID3D11SamplerState* sampler[1];
	// サンプラーステートの種類を判別して、サンプラーを設定する
	switch (ss)
	{
	case DrawPolygon::SamplerStates::ANISOTROPIC_CLAMP:// アニソトロピッククランプ
		// アニソトロピッククランプのサンプラーを取得
		sampler[0] = m_pStates->AnisotropicClamp();
		break;
	case DrawPolygon::SamplerStates::ANISOTROPIC_WRAP:// アニソトロピックラップ
		// アニソトロピックラップのサンプラーを取得
		sampler[0] = m_pStates->AnisotropicWrap();
		break;
	case DrawPolygon::SamplerStates::LINEAR_CLAMP: // リニアクランプ
		// リニアクランプのサンプラーを取得
		sampler[0] = m_pStates->LinearClamp();
		break;
	case DrawPolygon::SamplerStates::LINEAR_WRAP: // リニアラップ
		// リニアラップのサンプラーを取得
		sampler[0] = m_pStates->LinearWrap();
		break;
	case DrawPolygon::SamplerStates::POINT_CLAMP: // ポイントクランプ
		// ポイントクランプのサンプラーを取得
		sampler[0] = m_pStates->PointClamp();
		break;
	case DrawPolygon::SamplerStates::POINT_WRAP: // ポイントラップ
		// ポイントラップのサンプラーを取得
		sampler[0] = m_pStates->PointWrap();
		break;
	default: // デフォルトはnullptr
		// サンプラーをnullptrに設定
		sampler[0] = nullptr;
		break;
	}
	// ピクセルシェーダにサンプラーを登録する
	m_pDeviceContext->PSSetSamplers(0, 1, sampler);
	// ブレンドステートの登録
	ID3D11BlendState* blendState;
	// ブレンドステートの種類を判別して、ブレンドステートを設定する
	switch (bs)
	{
	case DrawPolygon::BlendStates::ALPHA:// アルファブレンド
		// アルファブレンドのブレンドステートを取得
		blendState = m_pStates->AlphaBlend();
		break;
	case DrawPolygon::BlendStates::ADDITIVE: // 加算ブレンド
		// 加算ブレンドのブレンドステートを取得
		blendState = m_pStates->Additive();
		break;
	case DrawPolygon::BlendStates::OPAQUE: // 不透明
		// 不透明のブレンドステートを取得
		blendState = m_pStates->Opaque();
		break;
	case DrawPolygon::BlendStates::NONPREMULTIPLIED: // 非プリマルチプライド
		// 非プリマルチプライドのブレンドステートを取得
		blendState = m_pStates->NonPremultiplied();
		break;
	default: // デフォルトはnullptr
		// ブレンドステートをnullptrに設定
		blendState = nullptr;
		break;
	}
	// ブレンドステートを設定する
	m_pDeviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	// 深度ステンシルステートの登録
	ID3D11DepthStencilState* depthStencilState;
	// 深度ステンシルステートの種類を判別して、深度ステンシルステートを設定する
	switch (dss)
	{
	case DrawPolygon::DepthStencilStates::DEPTH_DEFAULT:// デフォルト
		// デフォルトの深度ステンシルステートを取得
		depthStencilState = m_pStates->DepthDefault();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_NONE: // 深度なし
		// 深度なしの深度ステンシルステートを取得
		depthStencilState = m_pStates->DepthNone();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ: // 深度読み取り
		// 深度読み取りの深度ステンシルステートを取得
		depthStencilState = m_pStates->DepthRead();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ_REVERSE_Z: // 深度読み取り（逆Z）
		// 深度読み取り（逆Z）の深度ステンシルステートを取得
		depthStencilState = m_pStates->DepthReadReverseZ();
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_REVERSE_Z: // 逆Z
		// 逆Zの深度ステンシルステートを取得
		depthStencilState = m_pStates->DepthReverseZ();
		break;
	default: // デフォルトはnullptr
		// 深度ステンシルステートをnullptrに設定
		depthStencilState = nullptr;
		break;
	}
	// 深度ステンシルステートを設定する
	m_pDeviceContext->OMSetDepthStencilState(depthStencilState, 0);

	// ラスタライザーステートの登録
	ID3D11RasterizerState* rasterizerState;
	// ラスタライザーステートの種類を判別して、ラスタライザーステートを設定する
	switch (rs)
	{
	case DrawPolygon::RasterizerStates::CULL_CLOCKWISE: // 時計回り
		// 時計回りのラスタライザーステートを取得
		rasterizerState = m_pStates->CullClockwise();
		break;
	case DrawPolygon::RasterizerStates::CULL_COUNTERCLOCKWISE: // 反時計回り
		// 反時計回りのラスタライザーステートを取得
		rasterizerState = m_pStates->CullCounterClockwise();
		break;
	case DrawPolygon::RasterizerStates::CULL_NONE: // カリングなし
		// カリングなしのラスタライザーステートを取得
		rasterizerState = m_pStates->CullNone();
		break;
	case DrawPolygon::RasterizerStates::WIREFRAME: // ワイヤーフレーム
		// ワイヤーフレームのラスタライザーステートを取得
		rasterizerState = m_pStates->Wireframe();
		break;
	default: // デフォルトはnullptr
		// ラスタライザーステートをnullptrに設定
		rasterizerState = nullptr;
		break;
	}
	// ラスタライザーステートを設定する
	m_pDeviceContext->RSSetState(rasterizerState);
}
/*
*	@brief リソースの更新
*	@details リソースの更新を行う
*	@param resource 更新するリソース
*	@param pSrcData 更新するデータ
*	@return なし
*/
void DrawPolygon::UpdateSubResources(ID3D11Resource* resource, const void* pSrcData)
{
	// サブリソースの更新を行う
	m_pDeviceContext->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);
}

/*
*	@brief 板ポリゴン描画
*	@details 板ポリゴンを描画する(VertexPositionTexture用)
*	@param vertices 頂点情報(VertexPositionTexture)
*	@return なし
*/
void DrawPolygon::DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices)
{
	// 描画開始
	m_pPrimitiveBatchTexture->Begin();
	// 四角形を描画
	m_pPrimitiveBatchTexture->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	// 描画終了
	m_pPrimitiveBatchTexture->End();

}
/*
*	@brief 板ポリゴン描画
*	@details 板ポリゴンを描画する(VertexPositionColorTexture用)
*	@param topology プリミティブトポロジー
*	@param vertices 頂点情報(VertexPositionColorTexture)
*	@param count 頂点数
*	@return なし
*/
void DrawPolygon::DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count)
{
	// 描画開始
	m_pPrimitiveBatchColorTexture->Begin();
	// 頂点を描画
	m_pPrimitiveBatchColorTexture->Draw(topology, vertices, count);
	// 描画終了
	m_pPrimitiveBatchColorTexture->End();
}
/*
*	@brief 解放
*	@details 解放（頂点、テクスチャ）を行う
*	@param なし
*	@return なし
*/
void DrawPolygon::ReleasePositionTexture()
{
	// プリミティブバッチ(頂点、テクスチャ)の解放
	m_pPrimitiveBatchTexture.reset();
	// コモンステートの解放
	m_pStates.reset();
	// デバイスコンテキストの解放
	m_pDeviceContext = nullptr;
	// デバイスリソースの解放
	m_pDR = nullptr;
	// デバイスの解放
	m_pDevice = nullptr;
}
/*
*	@brief 解放
*	@details 解放（頂点、色、テクスチャ）を行う
*	@param なし
*	@return なし
*/
void DrawPolygon::ReleasePositionColorTexture()
{
	// プリミティブバッチ(頂点、色、テクスチャ)の解放
	m_pPrimitiveBatchColorTexture.reset();
	// コモンステートの解放
	m_pStates.reset();
	// デバイスコンテキストの解放
	m_pDeviceContext = nullptr;
	// デバイスリソースの解放
	m_pDR = nullptr;
	// デバイスの解放
	m_pDevice = nullptr;
}
/*
*	@brief シェーダーにバッファを送る
*	@details シェーダーにバッファを送る
*	@param startSlot シェーダーの開始スロット
*	@param numBuffers 送るバッファの数
*	@param ppBuffer 送るバッファの配列
*	@return なし
*/
void DrawPolygon::SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	// 頂点シェーダーにバッファを送る
	m_pDeviceContext->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	// ジオメトリシェーダーにバッファを送る
	m_pDeviceContext->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
	// ピクセルシェーダーにバッファを送る
	m_pDeviceContext->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer);
}
/*
*	@brief シェーダーをセットする
*	@details シェーダーをセットする
*	@param shaders シェーダーの構造体
*	@param ppClassInstances クラスインスタンスの配列
*	@param nubClassInstances クラスインスタンスの数
*	@return なし
*/
void DrawPolygon::SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	// 頂点シェーダーをセットする
	if (shaders.vs.Get() != nullptr) m_pDeviceContext->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);
	// ジオメトリシェーダーをセットする
	if (shaders.gs.Get() != nullptr) m_pDeviceContext->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances);
	// ピクセルシェーダーをセットする
	if (shaders.ps.Get() != nullptr) m_pDeviceContext->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances);
}
/*
*	@brief シェーダーを解放する
*	@details シェーダーを解放する
*	@param なし
*	@return なし
*/
void DrawPolygon::ReleaseShader()
{
	// 頂点シェーダーを解放
	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);
	// ジオメトリシェーダーを解放
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	// ピクセルシェーダーを解放
	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0);
}