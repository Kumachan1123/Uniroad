/*
*	@file IGameUI.h
*	@brief ゲームUIのインターフェースクラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "KumachiLib/Anchor/Anchor.h"
#include "Game/Interface/IUI.h"

// 前方宣言
class CommonResources;

// ゲームUIのインターフェースクラス
class IGameUI : public IUI
{
public:
	// 列挙型
	//UIの種類
	enum class UIType
	{
		TILE,	// タイル
		ITEM,	// アイテム
		BUTTON,	// ボタン
	};
public:
	// public関数
	// デストラクタ
	virtual ~IGameUI() = default;
	// UI追加	
	virtual void Add(
		const std::string& key					// テクスチャのキー
		, const DirectX::SimpleMath::Vector2& position	// 位置
		, const DirectX::SimpleMath::Vector2& scale	// スケール
		, KumachiLib::ANCHOR anchor				// アンカー
		, UIType type) = 0;			// UIの種類	
};