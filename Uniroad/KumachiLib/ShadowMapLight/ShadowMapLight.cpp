/*
*	@file ShadowMapLight.cpp
*	@brief シャドウマップ用ライトクラス
*/
#include "pch.h"
#include "ShadowMapLight.h"
// シャドウマップのサイズ
const int ShadowMapLight::SHADOWMAP_SIZE = 1024;
/*
*	@brief コンストラクタ
*	@details シャドウマップ用ライトクラスのコンストラクタ
*	@param commonResources 共通リソース
*	@return なし
*/
ShadowMapLight::ShadowMapLight(CommonResources* commonResources)
	: m_pCommonResources(commonResources)// 共通リソース
	, m_pDevice(commonResources->GetDeviceResources()->GetD3DDevice())// Direct3Dデバイス
	, m_pVS(nullptr)// 頂点シェーダ
	, m_pPS(nullptr)// ピクセルシェーダ
	, m_pConstantBuffer(nullptr)// 定数バッファ
	, m_pShadowMapRT(nullptr)// レンダーテクスチャ
	, m_pShadowMapDS(nullptr)// 深度ステンシル
	, m_pShadowMapSampler(nullptr)// シャドウマップのサンプラー
	, m_lightTheta(100.0f)// ライトの画角 (100度を初期値として設定
	, m_viewport(commonResources->GetDeviceResources()->GetScreenViewport()) // ビューポート
{
	// 名前空間
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ライトの座標
	m_lightPosition = Vector3(0.0f, 10.0f, 0.0f);
	// ライトの回転
	m_lightQuaternion = Quaternion::CreateFromYawPitchRoll(
		XMConvertToRadians(-90.0f),	// Y軸時計回り90度
		XMConvertToRadians(90.0f),	// X軸反時計回り45度
		0.0f
	);
	LoadShader(m_pDevice);
	CreateConstanBuffer(m_pDevice);
	CreateShadowMapObject(m_pDevice);
}
/*
*	@brief デストラクタ
*	@details シャドウマップ用ライトクラスのデストラクタ
*	@param なし
*	@return なし
*/
ShadowMapLight::~ShadowMapLight()
{
}
/*
*	@brief 更新
*	@details シャドウマップ用ライトクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void ShadowMapLight::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}
/*
*	@brief 影になるモデルを描画
*	@details シャドウマップをレンダリングする
*	@param view ビュー行列
*	@param proj 射影行列
*	@return なし
*/
void ShadowMapLight::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ライトの向いている方向を調整する
	const Vector3 lightDir = Vector3::Transform(Vector3(-1.0f, 0.0f, 1.0f), m_lightQuaternion);
	// ライト空間のビュー行列を計算する
	const Matrix lightView = Matrix::CreateLookAt(
		m_lightPosition,			// eye
		m_lightPosition + lightDir,	// target
		Vector3::UnitY 				// up
	);
	// ライト空間の射影行列を計算する
	const Matrix lightProjection = Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(m_lightTheta),	// fov
		1.0f,	// aspectRatio
		0.1f,	// nearPlane
		1000.0f	// farPlane
	);
	// GPUが使用するリソースのメモリにCPUからアクセスする際に利用する構造体
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// 定数バッファをマップする
	context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// 定数バッファに送るデータを準備する
	cbLight* cb = static_cast<cbLight*>(mappedResource.pData);
	const Matrix viewProj = lightView * lightProjection;
	cb->lightViewProjection = XMMatrixTranspose(viewProj);	// ビュー行列と射影行列を合算したもの
	cb->lightPosition = m_lightPosition;					// ライトの座標
	cb->lightDirection = lightDir;							// ライトが照らす方向
	cb->lightAmbient = Color(0.125f, 0.125f, 0.125f, 0.001f);				// 環境光
	// マップを解除する
	context->Unmap(m_pConstantBuffer.Get(), 0);
	// シャドウマップ用のRTVとDSVを設定する
	auto rtv = m_pShadowMapRT->GetRenderTargetView();
	auto dsv = m_pShadowMapDS->GetDepthStencilView();
	// シャドウマップ用のRTVとDSVをクリアする
	context->ClearRenderTargetView(rtv, Colors::White);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// シャドウマップSRVがバインドされていたら解除する
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv); // SRVスロット1をNULLでクリア
	// RTVとDSVをシャドウマップ用に変更する
	context->OMSetRenderTargets(1, &rtv, dsv);
	// ビューポートをシャドウマップ用に変更する
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);
	// 影になるモデルを描画する
	for (const auto& shadowInfo : m_pShadowInfos)
	{
		// ワールド行列を抽出
		Matrix world = shadowInfo.second;
		// モデルを取得
		DirectX::Model* model = shadowInfo.first;
		// 影を描画
		model->Draw(context, *states, world, lightView, lightProjection, false, [&]
			{
				context->VSSetShader(m_pVSShadowMap.Get(), nullptr, 0);
				context->PSSetShader(m_pPSShadowMap.Get(), nullptr, 0);
			}
		);
	}
	// RTVとDSVを通常描画用の設定に戻す準備
	auto defaultRTV = m_pCommonResources->GetDeviceResources()->GetRenderTargetView();
	auto defaultDSV = m_pCommonResources->GetDeviceResources()->GetDepthStencilView();
	// 描画先に通常描画用のRTVとDSVを設定する
	context->OMSetRenderTargets(1, &defaultRTV, defaultDSV);
	// ビューポートを通常描画用に切り替える
	auto const& defaultVP = m_viewport;
	context->RSSetViewports(1, &defaultVP);
	// リソースの割り当てを解除する
	context->PSSetShaderResources(1, 1, nullsrv);
	// モデルとワールド行列のペアをクリアする
	m_pShadowInfos.clear();

}


/*
*	@brief シェーダの読み込み
*	@details シャドウマップ用のシェーダを読み込む
*	@param device Direct3Dデバイス
*	@return なし
*/
void ShadowMapLight::LoadShader(ID3D11Device* device)
{
	// シェーダを読み込むためのblob
	std::vector<uint8_t> blob;
	// ライト＋影の頂点シェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/VS_Light.cso");
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_pVS.ReleaseAndGetAddressOf());
	// ライト＋影のピクセルシェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/PS_Light.cso");
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_pPS.ReleaseAndGetAddressOf());
	// シャドウマップ用頂点シェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/VS_ShadowMap.cso");
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_pVSShadowMap.ReleaseAndGetAddressOf());
	// シャドウマップ用ピクセルシェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/PS_ShadowMap.cso");
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_pPSShadowMap.ReleaseAndGetAddressOf());
}
/*
*	@brief 定数バッファの作成
*	@details シャドウマップ用の定数バッファを作成する
*	@param device Direct3Dデバイス
*	@return なし
*/
void ShadowMapLight::CreateConstanBuffer(ID3D11Device* device)
{
	// 定数バッファ用のバッファオブジェクトを作成する
	D3D11_BUFFER_DESC bufferDesc{};
	// バッファサイズを設定
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(cbLight));
	// バッファの使用方法を設定
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	// バッファのバインドフラグを設定
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// バッファのCPUアクセスフラグを設定
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// バッファの構造を設定
	device->CreateBuffer(&bufferDesc, nullptr, m_pConstantBuffer.ReleaseAndGetAddressOf());

}
/*
*	@brief シャドウマップ用の各種オブジェクトを作成する
*	@details シャドウマップ用のレンダーテクスチャと深度ステンシルを作成する
*	@param device Direct3Dデバイス
*	@return なし
*/
void ShadowMapLight::CreateShadowMapObject(ID3D11Device* device)
{
	// シャドウマップの大きさを指定する
	const RECT rectShadow = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };
	// レンダーテクスチャを作成する
	m_pShadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	// Direct3Dデバイスを設定する
	m_pShadowMapRT->SetDevice(device);
	// シャドウマップの大きさを設定する
	m_pShadowMapRT->SetWindow(rectShadow);
	// 深度ステンシルを作成する
	m_pShadowMapDS = std::make_unique<mylib::DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	// Direct3Dデバイスを設定する
	m_pShadowMapDS->SetDevice(device);
	// シャドウマップの大きさを設定する
	m_pShadowMapDS->SetWindow(rectShadow);
	// シャドウマップ用のサンプラーを作成する
	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	// サンプラーの設定を行う
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	// アドレスモードを設定
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	// 比較関数を設定
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&samplerDesc, m_pShadowMapSampler.ReleaseAndGetAddressOf());
}

/*
*	@brief モデル描画時のラムダ式内で呼ぶ処理をまとめた関数
*	@details モデル描画時に必要な設定を行う
*	@param context Direct3Dデバイスコンテキスト
*	@param states 共通ステート
*	@return なし
*/
void ShadowMapLight::ApplyShader(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states)
{
	m_pSRV = m_pShadowMapRT->GetShaderResourceView();
	// 定数バッファを指定する
	ID3D11Buffer* cbuf[] = { m_pConstantBuffer.Get() };
	context->VSSetConstantBuffers(1, 1, cbuf);
	context->PSSetConstantBuffers(1, 1, cbuf);
	// シェーダを設定する
	context->VSSetShader(m_pVS.Get(), nullptr, 0);
	context->PSSetShader(m_pPS.Get(), nullptr, 0);
	// サンプラーステートを指定する
	ID3D11SamplerState* sampler[] = { states->LinearWrap(), m_pShadowMapSampler.Get() };
	context->PSSetSamplers(0, 2, sampler);
	// 作成した深度マップをシェーダリソースとして設定する
	context->PSSetShaderResources(1, 1, &m_pSRV);
}