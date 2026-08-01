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

	DirectX::SimpleMath::Vector4 DiffuseColor =
	{
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	std::unique_ptr<FBXTexture> DiffuseTexture;

	/// 半透明か
	bool IsTransparent = false;

	/// 不透明度
	float Opacity = 1.0f;
	// 顔か
	bool IsSkin = false;
	bool HasTexture() const
	{
		return DiffuseTexture != nullptr;
	}

	ID3D11ShaderResourceView* GetTexture() const
	{
		return DiffuseTexture ?
			DiffuseTexture->GetSRV() :
			nullptr;
	}
};