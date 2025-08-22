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
	// public関数
	// デストラクタ
	virtual ~IUI() = default;
	// 初期化
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	// 更新
	virtual void Update(const float elapsedTime) = 0;
	// 描画
	virtual void Render() = 0;

};