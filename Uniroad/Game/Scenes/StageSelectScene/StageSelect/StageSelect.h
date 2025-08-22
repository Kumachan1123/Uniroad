/*
*	@file StageSelect.h
*	@brief ステージセレクトシーンのステージセレクトクラス
*/
#pragma once

// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// ステージセレクトモデルクラス
class StageSelect
{
public:
	// アクセサ
	// シャドウマップライトを取得する
	ShadowMapLight* GetShadowMapLight() const { return m_pShadowMapLight; }
	// シャドウマップライトを設定する
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) { m_pShadowMapLight = shadowMapLight; }
public:
	// public関数
	// コンストラクタ
	StageSelect(CommonResources* resources);
	// デストラクタ
	~StageSelect();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();
private:
	// private関数
	// 深度ステンシルバッファを設定する
	void CreateDepthStencilBuffer(ID3D11Device* pDevice);
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// モデルポインター
	DirectX::Model* m_pModel;
	// シャドウマップライト
	ShadowMapLight* m_pShadowMapLight;
	// 深度ステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
};