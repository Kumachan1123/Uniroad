/**
 * @file FBXTexture.cpp
 * @brief FBXモデル用テクスチャ管理
 */

#include "pch.h"

#include "FBXTexture.h"

#include <WICTextureLoader.h>


bool FBXTexture::Load(
	ID3D11Device* device,
	const std::string& filePath)
{
	HRESULT hr =
		DirectX::CreateWICTextureFromFile(
			device,
			std::wstring(
				filePath.begin(),
				filePath.end()).c_str(),
			nullptr,
			m_texture.GetAddressOf());


	return SUCCEEDED(hr);
}


ID3D11ShaderResourceView* FBXTexture::GetSRV() const
{
	return m_texture.Get();
}

bool FBXTexture::LoadFromMemory(
	ID3D11Device* device,
	const uint8_t* data,
	size_t size)
{
	HRESULT hr =
		DirectX::CreateWICTextureFromMemory(
			device,
			data,
			size,
			nullptr,
			m_texture.GetAddressOf());


	return SUCCEEDED(hr);
}