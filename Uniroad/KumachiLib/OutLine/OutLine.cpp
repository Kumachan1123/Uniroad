/*
*	@file OutLine.cpp
*	@brief 輪郭線描画クラス
*/
#include "pch.h"
#include "OutLine.h"
/*
*	@brief コンストラクタ
*	@details 輪郭線描画クラスのコンストラクタ
*	@param commonResources 共通リソース
*	@return なし
*/
OutLine::OutLine(CommonResources* commonResources)
	: m_pCommonResources(commonResources)// 共通リソース
	, m_pDevice(commonResources->GetDeviceResources()->GetD3DDevice())// Direct3Dデバイス
	, m_pVS(nullptr)// 頂点シェーダ
	, m_pPS(nullptr)// ピクセルシェーダ
	, m_viewport(commonResources->GetDeviceResources()->GetScreenViewport()) // ビューポート
{
	// シェーダの読み込み
	LoadShader(m_pDevice);
	// 定数バッファの作成
	CreateConstanBuffer(m_pDevice);
}
/*
*	@brief デストラクタ
*	@details 輪郭線描画クラスのデストラクタ
*	@param なし
*	@return なし
*/
OutLine::~OutLine()
{
	// 共通リソースをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 輪郭になるモデルを描画
*	@details 輪郭になるモデルを描画する
*	@param view ビュー行列
*	@param proj 射影行列
*	@return なし
*/
void OutLine::RenderOutLine(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectXの名前空間を使用
	using namespace DirectX;
	// SimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// 必要なリソースを取得する
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ワールドビュー射影行列の計算
	Matrix ViewProj = view * proj;
	// コンスタントバッファのマップ
	D3D11_MAPPED_SUBRESOURCE mappedResourceO{};
	context->Map(m_pOutlineConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceO);
	// コンスタントバッファに送るデータを設定
	cbOutline* cbO = static_cast<cbOutline*>(mappedResourceO.pData);
	// 行列は転置する
	cbO->WorldViewProj = ViewProj.Transpose();
	// アウトラインの色
	cbO->OutlineColor = Vector4{ 0.0f, 0.0f, 0.0f, 1.0f };
	// アウトラインの太さ
	cbO->OutlineThickness = OUTLINE_WIDTH;
	// FOVのθ
	cbO->FOVTheta = XMConvertToRadians(m_fovTheta);
	// アンマップ
	context->Unmap(m_pOutlineConstantBuffer.Get(), 0);
	// 輪郭になるモデルを描画する
	for (const auto& outLineInfo : m_pOutLineInfos)
	{
		// モデルを取得
		DirectX::Model* model = outLineInfo.first;
		// ワールド行列を抽出
		Matrix world = outLineInfo.second;
		// 輪郭を描画する
		model->Draw(context, *m_pCommonResources->GetCommonStates(), world, view, proj, false, [&]
			{
				// コンスタントバッファを設定
				ID3D11Buffer* cbuf[] = { m_pOutlineConstantBuffer.Get() };
				// 各シェーダーに定数バッファを設定する
				context->VSSetConstantBuffers(2, 1, cbuf);
				context->PSSetConstantBuffers(2, 1, cbuf);
				// シェーダを設定する
				context->VSSetShader(m_pVS.Get(), nullptr, 0);
				context->PSSetShader(m_pPS.Get(), nullptr, 0);
				// サンプラーステートを指定する
				ID3D11SamplerState* sampler[] = { m_pCommonResources->GetCommonStates()->LinearWrap() };
				// カリング設定
				ID3D11RasterizerState* rasterizerState;
				// 裏面描画にする
				rasterizerState = m_pCommonResources->GetCommonStates()->CullClockwise();
				// ラスタライザーステートを指定する
				context->RSSetState(rasterizerState);
				// ブレンドステートを指定する
				ID3D11BlendState* blendState = m_pCommonResources->GetCommonStates()->AlphaBlend();
				// ブレンドステートを設定する
				context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
				// 深度ステンシルステートを指定する
				ID3D11DepthStencilState* depthStencilState = m_pCommonResources->GetCommonStates()->DepthDefault();
				// 深度ステンシルステートを設定する
				context->OMSetDepthStencilState(depthStencilState, 0);
				// サンプラーステートをピクセルシェーダーに設定する
				context->PSSetSamplers(0, 1, sampler);
			}
		);
	}
	// モデルとワールド行列のペアをクリアする
	m_pOutLineInfos.clear();
}
/*
*	@brief シェーダー読込
*	@details シェーダーを読み込む
*	@param device Direct3Dデバイス
*	@return なし
*/
void OutLine::LoadShader(ID3D11Device* device)
{
	// シェーダを読み込むための変数
	std::vector<uint8_t> blob;
	// 頂点シェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/Outline/VS_Outline.cso");
	// 頂点シェーダーを作成する
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_pVS.ReleaseAndGetAddressOf());
	// ライト用のピクセルシェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/Outline/PS_Outline.cso");
	// ピクセルシェーダーを作成する
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_pPS.ReleaseAndGetAddressOf());
}
/*
*	@brief 定数バッファの作成
*	@details 輪郭線描画用の定数バッファを作成する
*	@param device Direct3Dデバイス
*	@return なし
*/
void OutLine::CreateConstanBuffer(ID3D11Device* device)
{
	// 定数バッファ用のバッファオブジェクトを作成する
	D3D11_BUFFER_DESC bufferDesc = {};
	// cbOutline 用のバッファ作成
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(cbOutline));
	// バッファの使用方法を設定
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	// バッファのバインドフラグを設定
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// バッファのCPUアクセスフラグを設定
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// バッファの構造を設定
	device->CreateBuffer(&bufferDesc, nullptr, m_pOutlineConstantBuffer.ReleaseAndGetAddressOf());
}
