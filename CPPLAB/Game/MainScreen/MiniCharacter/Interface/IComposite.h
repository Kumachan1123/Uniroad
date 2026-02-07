/*
*	@file	IComposite.h
*	@brief	ミニキャラクターの合成部品インターフェース
*/
#pragma once
// 自作ヘッダーファイル
#include "IComponent.h"

// ICompositeインターフェースを定義する
class IComposite : public IComponent
{
public:
	// public関数
	// 部品を追加する
	virtual void Attach(std::unique_ptr<IComponent> turretParts) = 0;
	// 部品を削除する
	virtual void Detach(std::unique_ptr<IComponent> turretPart) = 0;
};