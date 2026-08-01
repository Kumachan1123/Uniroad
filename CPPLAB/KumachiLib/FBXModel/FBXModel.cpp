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
	m_state = std::make_unique<DirectX::CommonStates>(MyResources::Get().GetDeviceResources()->GetD3DDevice());

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

	buf.World = world;

	// ワールド行列（逆行列の転置行列を計算するのはFBXShader側で行う）
	buf.WorldInverseTranspose =
		world;

	buf.WorldViewProj =
		world *
		view *
		projection;

	// 色設定
	buf.Color = m_diffuseColor;

	FBXLightBuffer lightBuf = {};
	// ライトの方向を設定
	lightBuf.LightDirection = DirectX::SimpleMath::Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	// ライトの色を設定
	lightBuf.LightColor = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// ライトにカメラの位置を設定
	DirectX::SimpleMath::Matrix cameraWorld = view.Invert();

	DirectX::SimpleMath::Vector3 cameraPosition =
	{
		cameraWorld._41,
		cameraWorld._42,
		cameraWorld._43
	};
	lightBuf.CameraPosition = DirectX::SimpleMath::Vector4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
	// 全構造体をまとめる
	FBXShaderBuffer shaderBuf = {};
	shaderBuf.Transform = buf;
	shaderBuf.Light = lightBuf;

	//OutputDebugStringA("Shader Set Complete\n");

	if (context == nullptr || shader == nullptr)
	{
		return;
	}
	//shader->Set(context, buf);
	ID3D11SamplerState* diffuseSampler[] = { m_state->AnisotropicWrap() };
	ID3D11SamplerState* rampSampler[] = { m_state->PointClamp() };
	context->PSSetSamplers(0, 1, diffuseSampler);
	context->PSSetSamplers(1, 1, rampSampler);
	context->RSSetState(m_state->CullNone());
	context->OMSetBlendState(m_state->AlphaBlend(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_state->DepthDefault(), 0);
//======================================
// 不透明パス
//======================================
	for (const MeshBuffer& meshBuffer : m_meshBuffers)
	{
		UINT stride = sizeof(ModelVertex);
		UINT offset = 0;

		uint32_t materialIndex = meshBuffer.MaterialIndex;

		if (m_materials == nullptr ||
			materialIndex >= m_materials->size())
		{
			continue;
		}

		const FBXMaterial& material =
			(*m_materials)[materialIndex];

		/// 半透明は後で描画
		if (material.Opacity < 0.999f)
		{
			continue;
		}

		shaderBuf.Transform.Color = material.DiffuseColor;
		//		// ディフューズカラーをデバッグ出力
		//OutputDebugStringA(
		//	("Material Diffuse Color: " +
		//	 std::to_string(material.DiffuseColor.x) + ", " +
		//	 std::to_string(material.DiffuseColor.y) + ", " +
		//	 std::to_string(material.DiffuseColor.z) + ", " +
		//	 std::to_string(material.DiffuseColor.w) + "\n").c_str());
		shader->Set(context, shaderBuf);

		context->OMSetBlendState(
			nullptr,
			nullptr,
			0xffffffff);

		context->OMSetDepthStencilState(
			m_state->DepthDefault(),
			0);

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

		ID3D11ShaderResourceView* srv = material.GetTexture();

		if (srv == nullptr)
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

	// 裏面カリング
	D3D11_RASTERIZER_DESC frontCullDesc = {};
	frontCullDesc.FillMode = D3D11_FILL_SOLID;
	frontCullDesc.CullMode = D3D11_CULL_FRONT;
	frontCullDesc.DepthClipEnable = TRUE;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> frontCullState;
	MyResources::Get().GetDeviceResources()->GetD3DDevice()->CreateRasterizerState(
		&frontCullDesc,
		frontCullState.GetAddressOf());

	// 表面カリング
	D3D11_RASTERIZER_DESC backCullDesc = {};
	backCullDesc.FillMode = D3D11_FILL_SOLID;
	backCullDesc.CullMode = D3D11_CULL_BACK;
	backCullDesc.DepthClipEnable = TRUE;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> backCullState;
	MyResources::Get().GetDeviceResources()->GetD3DDevice()->CreateRasterizerState(
		&backCullDesc,
		backCullState.GetAddressOf());

	//======================================
// 半透明パス
//======================================
	for (const MeshBuffer& meshBuffer : m_meshBuffers)
	{
		UINT stride = sizeof(ModelVertex);
		UINT offset = 0;

		uint32_t materialIndex = meshBuffer.MaterialIndex;

		if (m_materials == nullptr ||
			materialIndex >= m_materials->size())
		{
			continue;
		}

		const FBXMaterial& material =
			(*m_materials)[materialIndex];
		if (material.Opacity >= 0.999f)
		{
			continue;
		}

		//OutputDebugStringA(
		//	("Transparent Draw : Material " +
		//	 std::to_string(materialIndex) + "\n").c_str());
		//OutputDebugStringA(
		//	("Stored Alpha : " +
		//	 std::to_string(material.DiffuseColor.w) + "\n").c_str());
		shaderBuf.Transform.Color = material.DiffuseColor;
		shader->Set(context, shaderBuf);

		context->OMSetBlendState(
			m_state->Additive(),
			nullptr,
			0xffffffff);

		context->OMSetDepthStencilState(
			m_state->DepthRead(),
			0);

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

		ID3D11ShaderResourceView* srv = material.GetTexture();

		if (srv == nullptr)
		{
			srv = m_whiteTexture->GetSRV();
		}

		context->PSSetShaderResources(
			0,
			1,
			&srv);

		//======================================
		// 裏面
		//======================================
		context->RSSetState(frontCullState.Get());

		context->DrawIndexed(
			meshBuffer.IndexCount,
			0,
			0);

		//======================================
		// 表面
		//======================================
		context->RSSetState(backCullState.Get());

		context->DrawIndexed(
			meshBuffer.IndexCount,
			0,
			0);
	}

	// ラスタライザーステートを戻す
	context->RSSetState(m_state->CullNone());

	//OutputDebugStringA("Draw\n");
}