/*
*	@file IButton.h
*	@brief ボタンのインターフェースクラス
*/
#pragma once
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/Interface/IImage.h"

// 前方宣言
class CommonResources;

// ボタンのインターフェースクラス
class IButton : public IImage
{
public:
	// アクセサ
	// スピードアップボタンが押されたかどうかを取得
	virtual bool IsPressed() const = 0;
	// スピードアップボタンが押されたかどうかを設定
	virtual void SetPressed(bool isPressed) = 0;
public:
	// public関数
	// デストラクタ
	virtual ~IButton() = default;

};