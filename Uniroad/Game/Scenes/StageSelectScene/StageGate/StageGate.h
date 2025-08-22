/*
*	@file StageGate.h
*	@brief ステージゲートクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// ステージゲートクラス
class StageGate
{
public:
	// アクセサ
	// 座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// シャドウマップライトを取得する
	ShadowMapLight* GetShadowMapLight() const { return m_pShadowMapLight; }
	// シャドウマップライトを設定する
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) { m_pShadowMapLight = shadowMapLight; }

public:
	// public関数
	// コンストラクタ
	StageGate(CommonResources* resources);
	// デストラクタ
	~StageGate();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// シャドウマップライト
	ShadowMapLight* m_pShadowMapLight;
	// モデルポインター
	DirectX::Model* m_pModel;
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;

};