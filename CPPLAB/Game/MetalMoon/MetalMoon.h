/*
*		@file MetalMoon.h
*		@brief メタルムーンクラス(メタリックなモデルのテストクラス
*/
#pragma once
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Material/Material.h"

class CommonResources;

class MetalMoon
{
public:
	// アクセサ
	void SetEnvironmentMap(ID3D11ShaderResourceView* cubemap) { m_environmentMap = cubemap; }
	// マテリアルにアクセス
	Material* GetMaterial() const { return m_material; }
public:
	// public関数
	// コンストラクタ
	MetalMoon();
	// デストラクタ defaultで定義
	~MetalMoon() = default;
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();

	// private関数
private:

	// privateメンバ変数
private:
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// モデルポインター
	DirectX::Model* m_pModel;
	// マテリアル
	Material* m_material;
	// 環境マップ
	ID3D11ShaderResourceView* m_environmentMap;
};
