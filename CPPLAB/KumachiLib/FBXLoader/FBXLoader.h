#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "KumachiLib/FBXMeshData/FBXMeshData.h"
#include "KumachiLib/FBXMaterial/FBXMaterial.h"



class FBXLoader
{
public:

	bool Load(
		const std::string& filePath);


	uint32_t GetMeshCount() const;


	uint32_t GetMaterialCount() const;


	uint32_t GetAnimationCount() const;


	void DebugMeshInfo() const;



	const std::vector<MeshData>& GetMeshData() const;

	const std::vector<FBXMaterial>& GetMaterials() const;

	const aiScene* GetScene() const
	{
		return m_scene;
	}

	const DirectX::SimpleMath::Vector4& GetDiffuseColor() const
	{
		return m_diffuseColor;
	}

	const std::vector<DirectX::SimpleMath::Vector4>& GetMaterialDiffuseColors() const
	{
		return m_materialDiffuseColors;
	}

private:

	Assimp::Importer m_importer;

	std::vector<MeshData> m_meshDatas;

	std::vector<FBXMaterial> m_materials;

	DirectX::SimpleMath::Vector4 m_diffuseColor;
	std::vector<DirectX::SimpleMath::Vector4> m_materialDiffuseColors;

	const aiScene* m_scene = nullptr;
};