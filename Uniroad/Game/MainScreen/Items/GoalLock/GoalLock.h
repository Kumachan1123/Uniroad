/*
*	@file GoalLock.h
*	@brief ゴールロックのアイテムを定義するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// 前方宣言
class CommonResources;

// ゴールロッククラス
class GoalLock : public ItemBase
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
public:
	// public関数
	// コンストラクタ
	GoalLock();
	// デストラクタ
	~GoalLock() override;
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
	// ゴールロックのモデル
	DirectX::Model* m_pModel;
	// カメラのポインター
	ICamera* m_pCamera;
	// ゴールロックの情報
	ItemInfo m_itemInfo;
	// ゴールロックの位置
	DirectX::SimpleMath::Vector3 m_position;
	// ゴールロックのスケール
	DirectX::SimpleMath::Vector3 m_scale;
	// ゴールロックの回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 時間
	float m_time;
	// 回転速度
	float m_rotationSpeed;
	// ゴールロックが獲得されて消えるまでの時間
	float m_deleteTime;
	// ゴールロックが獲得されたかどうか
	bool m_isCollected;
	// ミニキャラのポインター
	MiniCharacter* m_pMiniCharacter;
	// このアイテムがある行番号
	int m_row;
	// このアイテムがある列番号
	int m_col;

};