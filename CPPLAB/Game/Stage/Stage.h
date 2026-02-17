/*
*		@file Stage.h
*		@brief ステージクラス
*/
#pragma once
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Material/Material.h"

class CommonResources;

class Stage
{
	// アクセサ


	// public関数
public:
	// コンストラクタ
	Stage();
	// デストラクタ defaultで定義
	~Stage() = default;
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

};