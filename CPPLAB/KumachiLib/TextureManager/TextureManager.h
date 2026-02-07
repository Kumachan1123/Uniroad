/*
*	@file TextureManager.h
*	@brief テクスチャマネージャークラス
*/
#pragma once
#ifndef TEXTUREMANAGER_DEFINED
#define TEXTUREMANAGER_DEFINED
// 標準ライブラリ
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>

// 前方宣言
class CommonResources;

// テクスチャマネージャークラス
class TextureManager
{
public:
	// アクセサ
	// テクスチャの取得
	ID3D11ShaderResourceView* GetTexture(const std::string& key);
public:
	// public関数
	// コンストラクタ
	TextureManager();
	// デストラクタ
	~TextureManager();
	// テクスチャの初期化
	void Initialize(ID3D11Device1* pDevice);
private:
	// private関数
	// テクスチャの読み込み
	void LoadTexture(const std::string& key, const wchar_t* path);
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイス
	ID3D11Device1* m_pDevice;
	// テクスチャのマップ
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextureMap;
};
#endif // TEXTUREMANAGER_DEFINED