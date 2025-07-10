/*
*	@file StraightVerticalTile.h
*	@brief 直進（縦）タイルの処理を定義するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// 直進（縦）タイルクラス
class StraightVerticalTile : public TileBase
{
public:
	// プレイヤーがこのタイルに入ったときの処理
	void OnEnter(MiniCharacter* character) override;
	// プレイヤーがこのタイルから出たときの処理
	void OnExit(MiniCharacter* character) override;

};

