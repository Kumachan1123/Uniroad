/*
*		@file   Material.cpp
*		@brief  マテリアルクラス
*/

#include "pch.h"
#include "Material.h"
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
#include "Game/MyResources/MyResources.h"
Material::Material()
	: m_rgb(DirectX::Colors::White) // RGB値を白で初期化
	, m_shaderName(L"Default") // シェーダー名を"Default"で初期化
	, m_noTexture(false) // テクスチャなしフラグをfalseで初期化

{}

void Material::Initialize()
{
	// シェーダを読み込むための変数
	std::vector<uint8_t> blob;
	// 頂点シェーダのパスを指定して読み込む
	std::wstring VSshaderPath = L"Resources/Shaders/Material/VS_" + m_shaderName + L".cso";
	blob = DX::ReadData(VSshaderPath.c_str());
	CreateVertexShader(blob);
	// ピクセルシェーダのパスを指定して読み込む
	std::wstring PSshaderPath = L"Resources/Shaders/Material/PS_" + m_shaderName + L".cso";
	blob = DX::ReadData(PSshaderPath.c_str());
	CreatePixelShader(blob);
}

void Material::Finalize()
{}
/*
*	@brief  各テクスチャをシェーダーにセットし、モデルにシェーダーを渡す
*	@param[in] context デバイスコンテキスト
*	@param[in] commonStates コモンステート
*	@return なし
*/
void Material::SetShaders(
	ID3D11DeviceContext* context,
	DirectX::DX11::CommonStates* commonStates)
{
	// t1: ノーマルマップ
	ID3D11ShaderResourceView* normalMapSRV = m_normalMap.Get(); // ←Material等に保持
	if (normalMapSRV)
		context->PSSetShaderResources(1, 1, &normalMapSRV);     // t1: ノーマルマップ

	// t2: 発光マップ
	ID3D11ShaderResourceView* emissiveMapSRV = m_emissiveMap.Get(); // ←Material等に保持
	if (emissiveMapSRV)
		context->PSSetShaderResources(2, 1, &emissiveMapSRV);     // t2: 発光マップ

	// t3: アンビエントオクルージョンマップ
	ID3D11ShaderResourceView* aoSRV = m_ao.Get(); // ←Material等に保持
	if (aoSRV)
		context->PSSetShaderResources(3, 1, &aoSRV);     // t3: AOマップ

	// t4: 粗さマップ	
	ID3D11ShaderResourceView* roughnessMapSRV = m_roughnessMap.Get(); // ←Material等に保持
	if (roughnessMapSRV)
		context->PSSetShaderResources(4, 1, &roughnessMapSRV);     // t4: 粗さマップ


	//  環境マップSRV・サンプラーをシェーダに渡す
	ID3D11ShaderResourceView* envCubeSRV = GetEnvironmentCubeSRV(); // ←Material等に保持
	if (envCubeSRV)
		context->PSSetShaderResources(10, 1, &envCubeSRV);     // t10: キューブマップ
	ID3D11SamplerState* envSampler = commonStates->LinearWrap(); // or custom sampler
	context->PSSetSamplers(10, 1, &envSampler);                // s10: キューブマップ用

	// 頂点・ピクセルシェーダーセット
	context->VSSetShader(GetVertexShader().Get(), nullptr, 0);
	context->PSSetShader(GetPixelShader().Get(), nullptr, 0);
}

void Material::CreateVertexShader(const std::vector<uint8_t>& blob)
{
	// Direct3Dデバイスを取得
	const auto device = MyResources::Get().GetDeviceResources()->GetD3DDevice();
	// 頂点シェーダーを作成する
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_vs.ReleaseAndGetAddressOf());
}

void Material::CreatePixelShader(const std::vector<uint8_t>& blob)
{
	// Direct3Dデバイスを取得
	const auto device = MyResources::Get().GetDeviceResources()->GetD3DDevice();
	// ピクセルシェーダーを作成する
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf());
}
