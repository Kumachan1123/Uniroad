/**
 * @file FBXShader.h
 * @brief FBXモデル用シェーダー
 */

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
/**
 * @brief FBX用変換行列
 */
struct FBXBuffer
{
	/**
	* @brief ワールド行列(だけ）
	*/
	DirectX::SimpleMath::Matrix World;

	/**
	 * @brief ワールド行列の逆行列の転置行列
	 */
	DirectX::SimpleMath::Matrix WorldInverseTranspose;

	/**
	 * @brief ワールドビュープロジェクション行列
	 */
	DirectX::SimpleMath::Matrix WorldViewProj;
	/**
	*  @brief 色
	*/
	DirectX::SimpleMath::Vector4 Color;
};
/**
 * @brief FBX用ライト情報
 */
struct FBXLightBuffer
{
	DirectX::SimpleMath::Vector4 LightDirection;
	DirectX::SimpleMath::Vector4 LightColor;
	DirectX::SimpleMath::Vector4 CameraPosition;
};
/**
 * @brief シェーダーに渡す構造体をさらにまとめた構造体
 */
struct FBXShaderBuffer
{
	FBXBuffer Transform;
	FBXLightBuffer Light;

};
class FBXTexture;
/**
 * @brief FBXモデル用シェーダー
 */
class FBXShader
{
public:

	/**
	 * @brief シェーダー生成
	 */
	bool Create(
		ID3D11Device* device);

	/**
	 * @brief シェーダー設定
	 */
	void Set(
		ID3D11DeviceContext* context, const FBXShaderBuffer& shaderBuffer);

private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_transformBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	std::unique_ptr<FBXTexture> m_rampTexture;
};