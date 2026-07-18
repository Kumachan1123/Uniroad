/*
*	@file ModelManager.cpp
*	@brief モデルマネージャークラス
*/
#include <pch.h>
#include "ModelManager.h"
// 標準ライブラリ
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief コンストラクタ
*	@details モデルマネージャークラスのコンストラクタ
*	@param なし
*	@return なし
*/
ModelManager::ModelManager()
	: m_pDevice(nullptr) // デバイス
	, m_pModelMap() // モデルのマップ
	, m_pEffectFactory(nullptr) // エフェクトファクトリー
{}
/*
*	@brief デストラクタ
*	@details モデルマネージャークラスのデストラクタ
*	@param なし
*	@return なし
*/
ModelManager::~ModelManager()
{
	// モデルのマップをクリア
	m_pModelMap.clear();
	// エフェクトファクトリーをリセット
	m_pEffectFactory.reset();
	// デバイスをnullptrに設定
	m_pDevice = nullptr;
}
/*
*	@brief モデルの初期化
*	@details 各種モデルのロードとエフェクトの設定を行う
*	@param なし
*	@return なし
*/
void ModelManager::Initialize(ID3D11Device1* pDevice)
{
	// デバイスを設定
	m_pDevice = pDevice;
	// エフェクトファクトリーの作成
	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pDevice);
	// エフェクトの共有を無効にする
	m_pEffectFactory->SetSharing(false);
	// JSONファイルの読み込み
	LoadJsonFile();
}
/*
*	@brief JSONファイルの読み込み
*	@details モデルのパスをJSONファイルから読み込む
*	@param なし
*	@return なし
*/
void ModelManager::LoadJsonFile()
{
	// nlohmann::jsonのエイリアスを定義
	using json = nlohmann::json;
	// 標準名前空間を使用
	using namespace std;
	//読み込むファイルの名前を作成
	string filename = "Resources/Jsons/Models.json";
	//ファイルを開く
	ifstream ifs(filename.c_str());
	// ファイルが正常に開けなかったら強制終了
	if (!ifs.good())return;
	//jsonオブジェクト
	json j;
	//ファイルから読み込む
	ifs >> j;
	//ファイルを閉じる
	ifs.close();
	// JSONの各アイテムに対してループ
	for (const auto& item : j.items())
	{
		// キー
		std::string key = item.key();
		// キーをワイド文字列に変換
		std::wstring wkey(key.begin(), key.end());
		// 値（ファイルパス）
		std::string path = item.value();
		// 文字列変換
		std::wstring wpath(path.begin(), path.end());
		// モデルのディレクトリ
		std::wstring modelDirectory = L"Resources/Models/" + wkey;
		// モデルのディレクトリを指定
		m_pEffectFactory->SetDirectory(modelDirectory.c_str());
		// モデルを読み込む
		m_pModelMap[key] = DirectX::Model::CreateFromCMO(m_pDevice, wpath.c_str(), *m_pEffectFactory);
		// 敵弾モデルのエフェクトを設定する
		m_pModelMap[key]->UpdateEffects([&](DirectX::IEffect* effect)
										{
											// エフェクトをBasicEffectにキャスト
											auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
											// 各ライトを無効にする
											basicEffect->SetLightEnabled(0, false);
											basicEffect->SetLightEnabled(1, false);
											basicEffect->SetLightEnabled(2, false);
										});
	}
}
/*
*	@brief モデルを取得する
*	@details 指定されたキーに対応するモデルを取得する
*	@param key モデルのキー
*	@return 指定されたキーに対応するモデルのポインタ。見つからない場合はnullptrを返す
*/
DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	// キーに対応するモデルを検索
	auto it = m_pModelMap.find(key);
	// 見つかった場合はモデルを返す
	if (it != m_pModelMap.end())	return it->second.get();
	// 見つからなかった場合はnullptrを返す
	return nullptr;
}
