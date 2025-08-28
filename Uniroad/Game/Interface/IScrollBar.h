/*
*	@file IScrollBar.h
*	@brief スクロールバーのインターフェースクラス
*/
#pragma once
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/Interface/IImage.h"

// 前方宣言
class CommonResources;

// スクロールバーのインターフェースクラス
class IScrollBar : public IImage
{
public:
	// アクセサ

public:
	// public関数
	// デストラクタ
	virtual ~IScrollBar() = default;
};