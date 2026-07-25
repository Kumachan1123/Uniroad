/**
 * @file FBXMaterial.h
 * @brief FBXモデル用マテリアル
 */

#pragma once

#include <memory>

#include <DirectXMath.h>

#include "KumachiLib/FBXTexture/FBXTexture.h"


/**
 * @brief FBXモデル用マテリアル
 */
struct FBXMaterial
{
public:

	/**
	 * @brief Diffuseカラー
	 */
	DirectX::SimpleMath::Vector4 DiffuseColor =
	{
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};


	/**
	 * @brief Diffuseテクスチャ
	 */
	std::unique_ptr<FBXTexture> DiffuseTexture;


	/**
	 * @brief テクスチャを持っているか
	 *
	 * @return 存在する場合true
	 */
	bool HasTexture() const
	{
		return DiffuseTexture != nullptr;
	}


	/**
	 * @brief テクスチャ取得
	 *
	 * @return ShaderResourceView
	 */
	ID3D11ShaderResourceView* GetTexture() const
	{
		if (DiffuseTexture == nullptr)
		{
			return nullptr;
		}


		return DiffuseTexture->GetSRV();
	}
};