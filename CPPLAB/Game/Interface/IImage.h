/*
*	@file IImage.h
*	@brief 画像のインターフェースクラス
*/
#pragma once
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/Interface/IUI.h"

// 前方宣言
class CommonResources;

// 画像のインターフェースクラス
class IImage : public IUI
{
public:
	// アクセサ
	// 座標を取得
	virtual const DirectX::SimpleMath::Vector2& GetPosition() const = 0;
	// 座標を設定
	virtual void SetPosition(const DirectX::SimpleMath::Vector2& position) = 0;
	// サイズを取得
	virtual const DirectX::SimpleMath::Vector2& GetSize() const = 0;
	// サイズを設定
	virtual void SetSize(const DirectX::SimpleMath::Vector2& size) = 0;
private:
	// private関数
	virtual void UpdateConstantBuffer() = 0;
public:
	// public関数
	// デストラクタ
	virtual ~IImage() = default;
};