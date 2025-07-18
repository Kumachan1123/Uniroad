/*
*	@file Medal.h
*	@brief メダルクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
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
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// メダルのモデル
	DirectX::Model* m_pModel;
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
};