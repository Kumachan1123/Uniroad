#include"pch.h"
#include "FBXModel.h"
// DirectX
#include <DeviceResources.h>
#include <d3d11.h>
#include <SimpleMath.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include "KumachiLib/FBXShader/FBXShader.h"
#include <CommonStates.h>

#include "Game/MyResources/MyResources.h"

FBXModel::FBXModel()
{
	m_diffuseColor = DirectX::SimpleMath::Vector4(
		1.0f,
		1.0f,
		1.0f,
		1.0f);
}

bool FBXModel::Create(
	ID3D11Device* device,
	const std::vector<MeshData>& meshDatas,
	const std::vector<FBXMaterial>& materials)
{
	if (meshDatas.empty())
	{
		return false;
	}

	m_whiteTexture = std::make_unique<FBXTexture>();

	m_whiteTexture->Load(
		device,
		"Resources/Textures/White.png");
	// 元データ保持
	m_meshDatas = meshDatas;
	m_materials = &materials;

	m_meshBuffers.resize(meshDatas.size());





	/// メッシュ情報をコピー
	for (size_t meshIndex = 0; meshIndex < meshDatas.size(); ++meshIndex)
	{
		MeshBuffer& meshBuffer = m_meshBuffers[meshIndex];

		meshBuffer.VertexCount =
			static_cast<UINT>(meshDatas[meshIndex].Vertices.size());

		meshBuffer.IndexCount =
			static_cast<UINT>(meshDatas[meshIndex].Indices.size());

		meshBuffer.MaterialIndex =
			meshDatas[meshIndex].MaterialIndex;
		/// 頂点バッファ設定
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth =
			static_cast<UINT>(sizeof(ModelVertex) * meshDatas[meshIndex].Vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = meshDatas[meshIndex].Vertices.data();

		HRESULT hr = device->CreateBuffer(
			&vertexBufferDesc,
			&vertexData,
			meshBuffer.VertexBuffer.GetAddressOf());

		if (FAILED(hr))
		{
			OutputDebugStringA("VertexBuffer Create Failed.\n");
			return false;
		}

		/// インデックスバッファ設定
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth =
			static_cast<UINT>(sizeof(uint32_t) * meshDatas[meshIndex].Indices.size());
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = meshDatas[meshIndex].Indices.data();

		hr = device->CreateBuffer(
			&indexBufferDesc,
			&indexData,
			meshBuffer.IndexBuffer.GetAddressOf());

		if (FAILED(hr))
		{
			OutputDebugStringA("IndexBuffer Create Failed.\n");
			return false;
		}

	}
	OutputDebugStringA(
		("MeshBuffer Count : " +
		 std::to_string(m_meshBuffers.size()) +
		 "\n").c_str());

	OutputDebugStringA("VertexBuffer Create Success.\n");
	OutputDebugStringA("IndexBuffer Create Success.\n");

	//CreateMaterial(device, m_fbxLoader->GetScene());
	return true;
}
void FBXModel::Draw(ID3D11DeviceContext* context, FBXShader* shader,
					DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//OutputDebugStringA("FBXModel Draw Start\n");

	if (context == nullptr ||
		shader == nullptr)
	{
		OutputDebugStringA("FBXModel Draw Invalid\n");
		return;
	}

	FBXBuffer buf = {};

	//// （テスト）Y方向に少し移動
	//world *= DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 5.0f, 0.0f);

	buf.WorldViewProj =
		world *
		view *
		projection;

	// 色設定
	buf.Color = m_diffuseColor;

	//OutputDebugStringA("Shader Set Complete\n");

	if (context == nullptr || shader == nullptr)
	{
		return;
	}
	const auto pStates = std::make_unique<DirectX::CommonStates>(MyResources::Get().GetDeviceResources()->GetD3DDevice());
	//shader->Set(context, buf);
	ID3D11SamplerState* sampler[] = { pStates->AnisotropicClamp() };
	context->PSSetSamplers(0, 1, sampler);
	context->RSSetState(pStates->CullClockwise());
	context->OMSetBlendState(pStates->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(pStates->DepthDefault(), 0);
	for (const MeshBuffer& meshBuffer : m_meshBuffers)
	{
		UINT stride = sizeof(ModelVertex);
		UINT offset = 0;
			// 色設定
		buf.Color = m_materialDiffuseColors[meshBuffer.MaterialIndex];
		shader->Set(context, buf);
		//OutputDebugStringA(
		//	("Draw Index Count : " +
		//	 std::to_string(meshBuffer.IndexCount) +
		//	 "\n").c_str());


		context->IASetVertexBuffers(
			0,
			1,
			meshBuffer.VertexBuffer.GetAddressOf(),
			&stride,
			&offset);


		context->IASetIndexBuffer(
			meshBuffer.IndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,
			0);


		context->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		uint32_t materialIndex =
			meshBuffer.MaterialIndex;

		ID3D11ShaderResourceView* srv = nullptr;


		if (m_materials != nullptr &&
			materialIndex < m_materials->size())
		{
			const FBXMaterial& material =
				(*m_materials)[materialIndex];

			if (material.DiffuseTexture)
			{
				srv = material.DiffuseTexture->GetSRV();
			}
		}

		/// テクスチャが無い場合は白テクスチャを使う
		if (srv == nullptr &&
			m_whiteTexture != nullptr)
		{
			srv = m_whiteTexture->GetSRV();
		}

		context->PSSetShaderResources(
			0,
			1,
			&srv);

		context->DrawIndexed(
			meshBuffer.IndexCount,
			0,
			0);
	}

	OutputDebugStringA("Draw\n");
}