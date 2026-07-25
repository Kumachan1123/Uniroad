/**
 * @file FBXLoader.cpp
 * @brief FBXファイルの読み込み
 */
#include "pch.h"
#include "FBXLoader.h"
#include "KumachiLib/FBXTexture/FBXTexture.h"
#include "Game/MyResources/MyResources.h"
#include <DeviceResources.h>

bool FBXLoader::Load(const std::string& filePath)
{
	m_scene = m_importer.ReadFile(
		filePath,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_LimitBoneWeights |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_FlipUVs |
		//aiProcess_MakeLeftHanded |
		aiProcess_FindInvalidData |
		aiProcess_FindDegenerates |
		aiProcess_OptimizeMeshes |
		aiProcess_PreTransformVertices);
	if (m_scene == nullptr)
	{
		return false;
	}

	m_meshDatas.clear();
	m_materials.clear();

	{
			/// シーンが読み込まれていなければ空のデータを返す
		if (m_scene == nullptr)
		{
			return {};
		}

		/// 全メッシュを格納する配列
		m_meshDatas.clear();

		/// メッシュを順番に処理
		for (unsigned int meshIndex = 0; meshIndex < m_scene->mNumMeshes; ++meshIndex)
		{
			/// Assimpのメッシュを取得
			const aiMesh* mesh = m_scene->mMeshes[meshIndex];

			/// エンジン用メッシュ
			MeshData meshData;

			/// 頂点をコピー
			for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
			{
				ModelVertex vertex;

				/// 座標
				vertex.Position =
				{
					mesh->mVertices[vertexIndex].x,
					mesh->mVertices[vertexIndex].y,
					mesh->mVertices[vertexIndex].z
				};

				/// 法線
				if (mesh->HasNormals())
				{
					vertex.Normal =
					{
						mesh->mNormals[vertexIndex].x,
						mesh->mNormals[vertexIndex].y,
						mesh->mNormals[vertexIndex].z
					};
				}

				/// UV
				if (mesh->HasTextureCoords(0))
				{
					vertex.UV =
					{
						mesh->mTextureCoords[0][vertexIndex].x,
						mesh->mTextureCoords[0][vertexIndex].y
					};

				}

				meshData.Vertices.emplace_back(vertex);
			}

			/// インデックスをコピー
			for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
			{
				const aiFace& face = mesh->mFaces[faceIndex];

				for (unsigned int index = 0; index < face.mNumIndices; ++index)
				{
					meshData.Indices.emplace_back(face.mIndices[index]);
				}
			}




			/// マテリアル番号
			meshData.MaterialIndex = mesh->mMaterialIndex;

			/// 配列へ追加
			m_meshDatas.emplace_back(std::move(meshData));

		}
	}
	{
		m_materials.clear();




		/// マテリアル数分処理
		for (unsigned int i = 0; i < m_scene->mNumMaterials; ++i)
		{
			FBXMaterial material;


			aiMaterial* aiMat =
				m_scene->mMaterials[i];


			aiString path;

			aiColor3D color;

			if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
			{
				material.DiffuseColor =
				{
					color.r,
					color.g,
					color.b,
					1.0f
				};
			}
			/// Diffuseテクスチャ取得
			if (aiMat->GetTexture(
				aiTextureType_DIFFUSE,
				0,
				&path) == AI_SUCCESS)
			{

				material.DiffuseTexture =
					std::make_unique<FBXTexture>();


				const char* texturePath =
					path.C_Str();


				/**
				 * Embedded Texture
				 */
				if (texturePath[0] == '*')
				{
					unsigned int index =
						std::stoi(texturePath + 1);


					const aiTexture* texture =
						m_scene->mTextures[index];


					/**
					 * 圧縮画像
					 */
					if (texture->mHeight == 0)
					{
						material.DiffuseTexture
							->LoadFromMemory(
								MyResources::Get()
								.GetDeviceResources()
								->GetD3DDevice(),

								reinterpret_cast<const uint8_t*>(
									texture->pcData),

								texture->mWidth);
					}

				}
				/**
				 * External Texture
				 */
				else
				{
					/// 同じ名前の埋め込みテクスチャを探す
					for (unsigned int textureIndex = 0;
						 textureIndex < m_scene->mNumTextures;
						 ++textureIndex)
					{
						const aiTexture* texture =
							m_scene->mTextures[textureIndex];

						if (path == texture->mFilename)
						{
							/// 圧縮画像(PNG DDS JPGなど)
							if (texture->mHeight == 0)
							{
								material.DiffuseTexture->LoadFromMemory(
									MyResources::Get()
									.GetDeviceResources()
									->GetD3DDevice(),

									reinterpret_cast<const uint8_t*>(texture->pcData),

									texture->mWidth);
							}

							break;
						}
					}
				}
	/*			OutputDebugStringA(path.C_Str());
				OutputDebugStringA("\n");*/
			}

			if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
			{
				m_diffuseColor =
				{
					color.r,
					color.g,
					color.b,
					1.0f
				};
				m_materialDiffuseColors.emplace_back(m_diffuseColor);

			}
			{
				OutputDebugStringA(
					("DiffuseColor : " +
					 std::to_string(color.r) + ", " +
					 std::to_string(color.g) + ", " +
					 std::to_string(color.b) + "\n").c_str());
			}
			OutputDebugStringA(
				("MaterialIndex : " + std::to_string(i) + "\n").c_str());

			OutputDebugStringA(
				("TexturePath : " + std::string(path.C_Str()) + "\n").c_str());
			m_materials.emplace_back(std::move(material));
		}

	}
	OutputDebugStringA(
		("Embedded Texture Count : " +
		 std::to_string(m_scene->mNumTextures) +
		 "\n").c_str());
	for (unsigned int i = 0; i < m_scene->mNumTextures; i++)
	{
		const aiTexture* tex = m_scene->mTextures[i];

		OutputDebugStringA(
			("Embedded[" + std::to_string(i) + "]\n").c_str());

		OutputDebugStringA(
			("mFilename : " +
			 std::string(tex->mFilename.C_Str()) +
			 "\n").c_str());

		OutputDebugStringA(
			("Width : " +
			 std::to_string(tex->mWidth) +
			 "\n").c_str());

		OutputDebugStringA(
			("Height : " +
			 std::to_string(tex->mHeight) +
			 "\n").c_str());
	}

	{

		OutputDebugStringA("GetRoot");

		const aiNode* root = m_scene->mRootNode;

		OutputDebugStringA(root->mName.C_Str());
		OutputDebugStringA("\n");
		for (unsigned int i = 0; i < root->mNumChildren; i++)
		{
			OutputDebugStringA(root->mChildren[i]->mName.C_Str());
			OutputDebugStringA("\n");
		}
		const aiNode* node = m_scene->mRootNode->mChildren[0];

		const aiMatrix4x4& m = node->mTransformation;

		char text[512];

		sprintf_s(
			text,
			"Node Matrix\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n",
			m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4);

		OutputDebugStringA(text);
		OutputDebugStringA("EndGetRoot");

	}
	return true;
}

uint32_t FBXLoader::GetMeshCount() const
{
	if (m_scene == nullptr)
	{
		return 0;
	}

	return static_cast<uint32_t>(m_scene->mNumMeshes);
}

uint32_t FBXLoader::GetMaterialCount() const
{
	if (m_scene == nullptr)
	{
		return 0;
	}

	return static_cast<uint32_t>(m_scene->mNumMaterials);
}

uint32_t FBXLoader::GetAnimationCount() const
{
	if (m_scene == nullptr)
	{
		return 0;
	}

	return static_cast<uint32_t>(m_scene->mNumAnimations);
}




void FBXLoader::DebugMeshInfo() const
{
	if (m_scene == nullptr)
	{
		OutputDebugStringA("FBX is not loaded.\n");
		return;
	}

	for (unsigned int meshIndex = 0; meshIndex < m_scene->mNumMeshes; ++meshIndex)
	{
		const aiMesh* mesh = m_scene->mMeshes[meshIndex];

		std::string text;

		text += "=====================================\n";
		text += "Mesh Index : " + std::to_string(meshIndex) + "\n";
		text += "Vertices   : " + std::to_string(mesh->mNumVertices) + "\n";
		text += "Faces      : " + std::to_string(mesh->mNumFaces) + "\n";
		text += "Has Normal : ";
		text += mesh->HasNormals() ? "Yes\n" : "No\n";
		text += "Has UV : ";
		text += mesh->HasTextureCoords(0) ? "Yes\n" : "No\n";
		text += "Has Bones : ";
		text += mesh->HasBones() ? "Yes\n" : "No\n";
		text += "Indices : ";
		text += std::to_string(mesh->mNumFaces * 3) + "\n";
		const unsigned int maxVertexCount = std::min(mesh->mNumVertices, 5u);

		for (unsigned int vertexIndex = 0; vertexIndex < maxVertexCount; ++vertexIndex)
		{
			const aiVector3D& vertex = mesh->mVertices[vertexIndex];

			text +=
				"Vertex[" + std::to_string(vertexIndex) + "] : (" +
				std::to_string(vertex.x) + ", " +
				std::to_string(vertex.y) + ", " +
				std::to_string(vertex.z) + ")\n";
		}

		OutputDebugStringA(text.c_str());
	}
}



const std::vector<MeshData>& FBXLoader::GetMeshData() const
{
	return m_meshDatas;
}

const std::vector<FBXMaterial>& FBXLoader::GetMaterials() const
{

	return m_materials;
}
