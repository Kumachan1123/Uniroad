/*
*	@file TextureManager.cpp
*	@brief テクスチャマネージャークラスの実装
*/
#include <pch.h>
#include "TextureManager.h"
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief コンストラクタ
*	@details テクスチャマネージャークラスのコンストラクタ
*	@param なし
*	@return なし
*/
TextureManager::TextureManager()
	: m_pCommonResources(nullptr)// 共通リソース
	, m_pDevice(nullptr)// デバイス
	, m_pTextureMap()// テクスチャマップ
{
}
/*
*	@brief デストラクタ
*	@details テクスチャマネージャークラスのデストラクタ
*	@param なし
*	@return なし
*/
TextureManager::~TextureManager()
{
	// テクスチャマップの解放
	for (auto& pair : m_pTextureMap)pair.second.Reset();
	// マップのクリア
	m_pTextureMap.clear();
}
/*
*	@brief テクスチャの初期化
*	@details テクスチャマネージャークラスの初期化を行う
*	@param pDevice デバイス
*	@return なし
*/
void TextureManager::Initialize(ID3D11Device1* pDevice)
{
	// nlohmann::jsonのエイリアスを定義
	using json = nlohmann::json;
	// 標準名前空間を使用
	using namespace std;
	// デバイスの設定
	m_pDevice = pDevice;
	//読み込むファイルの名前を作成
	string filename = "Resources/Jsons/textures.json";
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
		// 読み込み関数
		LoadTexture(key, wpath.c_str());
	}
}
/*
*	@brief テクスチャの読み込み
*	@details 指定されたキーとパスでテクスチャを読み込み、マップに追加する
*	@param key テクスチャのキー
*	@param path テクスチャのパス
*	@return なし
*/
void TextureManager::LoadTexture(const std::string& key, const wchar_t* path)
{
	// テクスチャ格納用
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(m_pDevice, path, nullptr, texture.ReleaseAndGetAddressOf());
	// マップに追加
	m_pTextureMap[key] = texture;

}
/*
*	@brief テクスチャの取得
*	@details 指定されたキーのテクスチャを取得する
*	@param key テクスチャのキー
*	@return 指定されたキーのテクスチャへのポインタ
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& key)
{
	// 指定されたキーのテクスチャを取得
	return m_pTextureMap[key].Get();
}