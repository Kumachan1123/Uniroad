/*
*	@file ItemBase.h
*	@brief アイテムのベースクラス
*/
#pragma once

// 前方宣言
class MiniCharacter;

// アイテムベースクラス
class ItemBase
{
	// public関数
public:
	// デストラクタ
	virtual ~ItemBase() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update(float elapsedTime) = 0;
	// 描画
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// アイテムを取得したときの処理
	virtual void OnGet(MiniCharacter* miniCharacter) = 0;
	// アイテムを使用したときの処理
	virtual void OnUse(MiniCharacter* miniCharacter) = 0;
	// アイテムを破棄したときの処理
	virtual void OnDiscard(MiniCharacter* miniCharacter) = 0;
};