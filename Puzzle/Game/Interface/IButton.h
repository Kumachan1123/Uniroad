/*
*	@file IButton.h
*	@brief ボタンのインターフェースクラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <string>
#include <vector>
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "KumachiLib/Anchor/Anchor.h"
#include "Game/Interface/IUI.h"

// 前方宣言
class CommonResources;

// ボタンのインターフェースクラス
class IButton : public IUI
{
public:
	// public関数
	// デストラクタ
	virtual ~IButton() = default;

};