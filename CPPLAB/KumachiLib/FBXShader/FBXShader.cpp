/**
 * @file FBXShader.cpp
 * @brief FBXモデル用シェーダー
 */

#include "pch.h"
#include "FBXShader.h"

#include <fstream>


bool FBXShader::Create(
	ID3D11Device* device)
{
	/// 頂点シェーダー読み込み
	std::ifstream vsFile(
		"Resources/Shaders/FBX/VS_FBX.cso",
		std::ios::binary);

	if (!vsFile)
	{
		return false;
	}

	std::vector<char> vsData(
		std::istreambuf_iterator<char>(vsFile),
		{});

	vsFile.close();


	/// ピクセルシェーダー読み込み
	std::ifstream psFile(
		"Resources/Shaders/FBX/PS_FBX.cso",
		std::ios::binary);

	if (!psFile)
	{
		return false;
	}

	std::vector<char> psData(
		std::istreambuf_iterator<char>(psFile),
		{});

	psFile.close();


	/// 頂点シェーダー生成
	if (FAILED(device->CreateVertexShader(
		vsData.data(),
		vsData.size(),
		nullptr,
		m_vertexShader.GetAddressOf())))
	{
		return false;
	}


	/// ピクセルシェーダー生成
	if (FAILED(device->CreatePixelShader(
		psData.data(),
		psData.size(),
		nullptr,
		m_pixelShader.GetAddressOf())))
	{
		return false;
	}


	/// 入力レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0	},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0	},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0	}
	};


	if (FAILED(device->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), m_inputLayout.GetAddressOf())))
	{
		return false;
	}

	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.ByteWidth =
		sizeof(FBXBuffer);

	bufferDesc.Usage =
		D3D11_USAGE_DEFAULT;

	bufferDesc.BindFlags =
		D3D11_BIND_CONSTANT_BUFFER;


	if (FAILED(device->CreateBuffer(
		&bufferDesc,
		nullptr,
		m_transformBuffer.GetAddressOf())))
	{
		return false;
	}
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter =
		D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplerDesc.AddressU =
		D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.AddressV =
		D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.AddressW =
		D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.ComparisonFunc =
		D3D11_COMPARISON_ALWAYS;

	samplerDesc.MinLOD = 0;

	samplerDesc.MaxLOD =
		D3D11_FLOAT32_MAX;


	if (FAILED(device->CreateSamplerState(
		&samplerDesc,
		m_samplerState.GetAddressOf())))
	{
		return false;
	}
	return true;
}


void FBXShader::Set(
	ID3D11DeviceContext* context,
	const FBXBuffer& transform)
{
	if (context == nullptr)
	{
		return;
	}


	context->IASetInputLayout(
		m_inputLayout.Get());


	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0);


	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0);


	context->PSSetSamplers(
		0,
		1,
		m_samplerState.GetAddressOf());


	FBXBuffer bufferData = {};

	bufferData.World =
		transform.World.Transpose();


	bufferData.WorldInverseTranspose =
		transform.World.Invert().Transpose();

	bufferData.WorldViewProj =
		transform.WorldViewProj.Transpose();

	bufferData.Color =
		transform.Color;


	context->UpdateSubresource(
		m_transformBuffer.Get(),
		0,
		nullptr,
		&bufferData,
		0,
		0);


	context->VSSetConstantBuffers(
		0,
		1,
		m_transformBuffer.GetAddressOf());

	context->PSSetConstantBuffers(
		0,
		1,
		m_transformBuffer.GetAddressOf());
}