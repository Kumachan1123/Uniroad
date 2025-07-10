/*
*	@file RightUpTile.h
*	@brief 右上タイルの処理を定義するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// 右上タイルクラス
class RightUpTile : public TileBase
{
public:
	RightUpTile() = default;
	// プレイヤーがこのタイルに入ったときの処理
	void OnEnter(MiniCharacter* character) override;
	// プレイヤーがこのタイルから出たときの処理
	void OnExit(MiniCharacter* character) override;
};