/**
 * @file FBXTexture.h
 * @brief FBXモデル用テクスチャ管理
 */

#pragma once

#include <string>

#include <wrl/client.h>
#include <d3d11.h>


/**
 * @brief FBXモデル用テクスチャ
 */
class FBXTexture
{
public:

	/**
	 * @brief テクスチャ読み込み
	 *
	 * @param device DirectXデバイス
	 * @param filePath テクスチャファイルパス
	 *
	 * @return 成功したか
	 */
	bool Load(
		ID3D11Device* device,
		const std::string& filePath);


	/**
	 * @brief シェーダーリソース取得
	 *
	 * @return ShaderResourceView
	 */
	ID3D11ShaderResourceView* GetSRV() const;

	/**
 * @brief メモリ上のテクスチャを読み込む
 * @param device DirectXデバイス
 * @param data 画像データ
 * @param size データサイズ
 * @return 成功したか
 */
	bool LoadFromMemory(
		ID3D11Device* device,
		const uint8_t* data,
		size_t size);
private:

	/**
	 * @brief シェーダーリソース
	 */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};