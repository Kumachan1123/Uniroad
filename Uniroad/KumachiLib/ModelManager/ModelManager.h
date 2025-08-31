/*
*	@file ModelManager.h
*	@brief モデルマネージャークラス
*/
#pragma once
#ifndef MODEL_MANAGER_DEFINED
#define MODEL_MANAGER_DEFINED
// 標準ライブラリ
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>

// モデルマネージャークラス
class ModelManager
{
public:
	// アクセサ
	// モデルの取得
	DirectX::Model* GetModel(const std::string& key);
public:
	// public関数
	// コンストラクタ
	ModelManager();
	// デストラクタ
	~ModelManager();
	// モデルの初期化
	void Initialize(ID3D11Device1* pDevice);
private:
	// private関数
	// JSONファイルの読み込み
	void LoadJsonFile();
private:
	// private変数
	// デバイス
	ID3D11Device1* m_pDevice;
	// モデルのマップ
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
#endif // MODEL_MANAGER_DEFINED
