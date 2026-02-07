/*
*	@file ILeaf.h
*	@brief ミニキャラクターの末端部品インターフェース
*/
#pragma once
// 自作ヘッダーファイル
#include "IComponent.h"

// ILeafインターフェースを定義する
class ILeaf : public IComponent
{
	// 現在末端部品が固有で持つものはない
};