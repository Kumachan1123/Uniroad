#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Items/ItemBase/ItemBase.h"

class Medal : public ItemBase
{
public:
	// コンストラクタ
	Medal() = default;
	// 初期化
	void Initialize() override;
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

};