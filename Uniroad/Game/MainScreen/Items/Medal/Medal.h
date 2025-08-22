/*
*	@file Medal.h
*	@brief メダルクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
#include "KumachiLib/Easing/Easing.h"
#include "Game/Particle/Particle.h"
#include "Game/Particle/Utility.h"
#include "Game/MainScreen/Shadow/Shadow.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"


// 前方宣言
class CommonResources;

// メダルクラス
class Medal : public ItemBase
{
public:
	// アクセサ
	// アイテムの位置を取得する
	const DirectX::SimpleMath::Vector3& GetPosition() const override { return m_position; }
	// アイテムの位置を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// アイテムのモデルを取得する
	DirectX::Model* GetModel() const override { return m_pModel; }
	// アイテムのモデルを設定する
	void SetModel(DirectX::Model* model) override { m_pModel = model; }
	// アイテムのワールド行列を取得する
	const DirectX::SimpleMath::Matrix& GetWorldMatrix() const override { return m_worldMatrix; }
	// アイテムのワールド行列を設定する
	void SetWorldMatrix(const DirectX::SimpleMath::Matrix& world) override { m_worldMatrix = world; }
	// アイテムのある行番号を取得する
	int GetRow() const override { return m_row; }
	// アイテムのある行番号を設定する
	void SetRow(int row) override { m_row = row; }
	// アイテムのある列番号を取得する
	int GetCol() const override { return m_col; }
	// アイテムのある列番号を設定する
	void SetCol(int col) override { m_col = col; }
	// カメラを取得する
	ICamera* GetCamera() const override { return m_pCamera; }
	// カメラを設定する
	void SetCamera(ICamera* camera) override { m_pCamera = camera; }
	// シャドウマップライトを取得する
	ShadowMapLight* GetShadowMapLight() const override { return m_pShadowMapLight; }
	// シャドウマップライトを設定する
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) override { m_pShadowMapLight = shadowMapLight; }

public:
	// public関数
	// コンストラクタ
	Medal();
	// デストラクタ
	~Medal() override;
	// 初期化
	void Initialize(CommonResources* resources, const ItemInfo& info) override;
	// 更新
	void Update(float elapsedTime) override;
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// アイテムを取得したときの処理
	void OnGet(MiniCharacter* miniCharacter) override;
	// アイテムを使用したときの処理
	void OnUse(MiniCharacter* miniCharacter) override;
	// アイテムを破棄したときの処理
	void OnDiscard(MiniCharacter* miniCharacter) override;
private:
	// private関数
	// パーティクルのパラメーターを設定する
	Utility::ParticleParams SetParticleParams() const;
private:
	// 定数
	// デフォルトの回転速度
	static constexpr float DEFAULT_ROTATION_SPEED = 1.0f;
	// 獲得されて消えるまでの回転速度
	static constexpr float COLLECTED_ROTATION_SPEED = 100.0f;
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// メダルのモデル
	DirectX::Model* m_pModel;
	// カメラのポインター
	ICamera* m_pCamera;
	// シャドウマップライト
	ShadowMapLight* m_pShadowMapLight;
	// パーティクル
	std::unique_ptr<Particle> m_pParticle;
	// 影
	std::unique_ptr<Shadow> m_pShadow;
	// メダルの情報
	ItemInfo m_itemInfo;
	// メダルの位置
	DirectX::SimpleMath::Vector3 m_position;
	// メダルのスケール
	DirectX::SimpleMath::Vector3 m_scale;
	// メダルの回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 時間
	float m_time;
	// 回転速度
	float m_rotationSpeed;
	// メダルが獲得されて消えるまでの時間
	float m_deleteTime;
	// メダルが獲得されたかどうか
	bool m_isCollected;
	// ミニキャラのポインター
	MiniCharacter* m_pMiniCharacter;
	// このアイテムがある行番号
	int m_row;
	// このアイテムがある列番号
	int m_col;
};