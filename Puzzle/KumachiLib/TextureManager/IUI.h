/*
*	@file		IUI.h
*	@brief	　UIのインターフェースクラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "KumachiLib/Anchor/Anchor.h"
// 前方宣言
class CommonResources;

// UIのインターフェースクラス
class IUI
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
	virtual ~IUI() = default;
	// 初期化
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	// 更新
	virtual void Update(const float elapsedTime) = 0;
	// 描画
	virtual void Render() = 0;
	// UI追加	
	virtual void Add(
		const std::string& key					// テクスチャのキー
		, const DirectX::SimpleMath::Vector2& position	// 位置
		, const DirectX::SimpleMath::Vector2& scale	// スケール
		, KumachiLib::ANCHOR anchor				// アンカー
		, UIType type) = 0;			// UIの種類	
};