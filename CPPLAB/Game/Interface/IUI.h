/*
*	@file		IUI.h
*	@brief	　UIのインターフェースクラス
*/
#pragma once



// UIのインターフェースクラス
class IUI
{
public:
	// public関数
	// デストラクタ
	virtual ~IUI() = default;
	// 初期化
	virtual void Initialize(int width, int height) = 0;
	// 更新
	virtual void Update(const float elapsedTime) = 0;
	// 描画
	virtual void Render() = 0;

};