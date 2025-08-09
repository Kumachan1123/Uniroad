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
	// 指定されたステージの空のモデルを作成して返す
	DirectX::Model* GetSkyModel(const std::string& stageID);
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
	// 弾モデルにエフェクト設定
	void SetupBulletModelEffects();
	// ステージモデルにエフェクト設定
	void SetupStageModelEffects();
	// JSONファイルの読み込み
	void LoadJsonFile();
	//天球モデルのパスをJSONファイルから読み込む
	void LoadSkyModelsJson();
private:
	// private変数
	// デバイス
	ID3D11Device1* m_pDevice;
	// モデルのマップ
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// モデルパスのマップ
	std::unordered_map<std::string, std::wstring> m_skyModelPathMap;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
#endif // MODEL_MANAGER_DEFINED
