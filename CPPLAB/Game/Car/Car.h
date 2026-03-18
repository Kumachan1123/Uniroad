/*
*		@file Car.h
*		@brief 車クラス
*/
#pragma once
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Material/Material.h"

class CommonResources;

class Car
{
public:
	// アクセサ
	// マテリアルを取得する
	Material* GetMaterial() const { return m_material; }
public:
	// public関数
	// コンストラクタ
	Car();
	// デストラクタ defaultで定義
	~Car() = default;
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();


private:
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// モデルへのポインタ
	DirectX::Model* m_pModel;
	// マテリアルへのポインタ
	Material* m_material;
	// ノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMap;
	// 発光マップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_emissiveMap;
	// アンビエントオクルージョンマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ao;
	// 粗さマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roughnessMap;
};
