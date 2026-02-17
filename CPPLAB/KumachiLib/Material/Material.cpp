/*
*		@file   Material.cpp
*		@brief  マテリアルクラス
*/

#include "pch.h"
#include "Material.h"

Material::Material()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_rgb(DirectX::Colors::White) // RGB値を白で初期化
{
}

void Material::Initialize(CommonResources* resources)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
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
{
}

void Material::CreateVertexShader(const std::vector<uint8_t>& blob)
{
	// Direct3Dデバイスを取得
	const auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// 頂点シェーダーを作成する
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_vs.ReleaseAndGetAddressOf());
}

void Material::CreatePixelShader(const std::vector<uint8_t>& blob)
{
	// Direct3Dデバイスを取得
	const auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// ピクセルシェーダーを作成する
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf());
}
