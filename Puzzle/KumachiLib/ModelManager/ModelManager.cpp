/*
*	@file ModelManager.cpp
*	@brief モデルマネージャークラス
*/
#include <pch.h>
#include "ModelManager.h"
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
	, m_skyModelPathMap() // 天球モデルパスのマップ
	, m_pEffectFactory(nullptr) // エフェクトファクトリー
{
}
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
	//// 天球モデルのパスをJSONファイルから読み込む
	//LoadSkyModelsJson();
	//// 弾モデルの作成
	//SetupBulletModelEffects();
	//// ステージモデルの作成
	//SetupStageModelEffects();
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
		// 値（ファイルパス）
		std::string path = item.value();
		// 文字列変換
		std::wstring wpath(path.begin(), path.end());
		// モデルのディレクトリを指定
		m_pEffectFactory->SetDirectory(L"Resources/Models");
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
				basicEffect->SetLightEnabled(2, true);
			});
	}

}
void ModelManager::LoadSkyModelsJson()
{
	// nlohmann::jsonのエイリアスを定義
	using json = nlohmann::json;
	// 標準名前空間を使用
	using namespace std;
	//読み込むファイルの名前を作成
	string filename = "Resources/Jsons/SkyModels.json";
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
		// 値（ファイルパス）
		std::string path = item.value();
		// 文字列変換
		std::wstring wpath(path.begin(), path.end());
		// モデルパスをマップに追加
		m_skyModelPathMap[key] = wpath;
	}
}
/*
*	@brief 弾モデルのエフェクト設定
*	@details 自弾と敵弾のモデルにエフェクトを設定する
*	@param なし
*	@return なし
*/
void ModelManager::SetupBulletModelEffects()
{
	// 自弾モデルのエフェクトを設定する
	m_pModelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// エフェクトをBasicEffectにキャスト
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			// 拡散色を設定
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);
			// 自発光色を設定
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});
	// 敵弾モデルのエフェクトを設定する
	m_pModelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// エフェクトをBasicEffectにキャスト
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			// 拡散色を設定
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, 0.2f, 0, 1));
			// 環境光の色を設定
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red);
			// 自発光色を設定
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato);
		});
}

/*
*	@brief ステージモデルのエフェクト設定
*	@details ステージモデルにエフェクトを設定する
*	@param なし
*	@return なし
*/
void ModelManager::SetupStageModelEffects()
{
	// ステージモデルのエフェクトを設定する
	m_pModelMap["Stage"]->UpdateEffects([](DirectX::IEffect* effect)
		{
			// エフェクトをBasicEffectにキャスト
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			// エフェクトがnullptrの場合は処理を終える
			if (!basicEffect)return;
			// 各ライトを無効にする
			basicEffect->SetLightEnabled(0, false);
			basicEffect->SetLightEnabled(1, false);
			basicEffect->SetLightEnabled(2, false);
			// モデルを自発光させる
			basicEffect->SetEmissiveColor(DirectX::Colors::White);
		}
	);
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
/*
*	@brief 指定されたステージの空のモデルを作成して返す
*	@details ステージIDに応じた空のモデルを取得する
*	@param stageID ステージID
*	@return 指定されたステージの空のモデルのポインタ。見つからない場合はnullptrを返す
*/
DirectX::Model* ModelManager::GetSkyModel(const std::string& stageID)
{
	// ステージIDに対応する空のモデルのパスを検索
	auto it = m_skyModelPathMap.find(stageID);
	// 見つからなかった場合はnullptrを返す
	if (it == m_skyModelPathMap.end())	return nullptr;
	// モデルを保持
	m_pModelMap["Sky"] = DirectX::Model::CreateFromCMO(m_pDevice, it->second.c_str(), *m_pEffectFactory);
	// モデルを返す
	return m_pModelMap["Sky"].get();
}
