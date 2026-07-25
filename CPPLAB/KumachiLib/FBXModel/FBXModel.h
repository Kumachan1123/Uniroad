/**
 * @file FBXModel.h
 * @brief FBXモデル
 */

#pragma once

#include <memory>
#include <vector>

#include <d3d11.h>
#include <wrl/client.h>
#include "KumachiLib/FBXMeshData/FBXMeshData.h"
#include "KumachiLib/FBXMaterial/FBXMaterial.h"

class FBXShader;

/**
 * @brief FBXモデル
 */
class FBXModel
{
public:
	/**
	* @色を設定
	*/
	void SetDiffuseColor(const DirectX::SimpleMath::Vector4& color)
	{
		m_diffuseColor = color;
	}

	void SetMaterialDiffuseColors(const std::vector<DirectX::SimpleMath::Vector4>& colors)
	{
		m_materialDiffuseColors = colors;
	}

	/**
	* @brief コンストラクタ
	*/
	FBXModel();

	/**
	 * @brief モデル生成
	 * @param meshDatas FBXLoaderから取得したメッシュ
	 * @return 成功したらtrue
	 */
	bool Create(
		ID3D11Device* device,
		const std::vector<MeshData>& meshDatas,
		const std::vector<FBXMaterial>& materials);

	/**
	 * @brief 描画
	 */
	void Draw(ID3D11DeviceContext* context, FBXShader* shader,
			  DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

private:

	/**
	 * @brief GPUへ送るメッシュ
	 */
	struct MeshBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;

		Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

		UINT VertexCount = 0;

		UINT IndexCount = 0;

		UINT MaterialIndex = 0;
	};

	std::vector<MeshBuffer> m_meshBuffers;

	const std::vector<FBXMaterial>* m_materials = nullptr;

	DirectX::SimpleMath::Vector4 m_diffuseColor;

	std::vector<DirectX::SimpleMath::Vector4> m_materialDiffuseColors;

	std::vector<MeshData> m_meshDatas;

	std::unique_ptr<FBXTexture> m_whiteTexture;
};