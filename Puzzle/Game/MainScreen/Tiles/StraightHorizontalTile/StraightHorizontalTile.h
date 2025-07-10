/*
*	@file StraightHorizontalTile.h
*	@brief 直進（横）タイルの処理を定義するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
// 直進（横）タイルクラス
class StraightHorizontalTile :public TileBase
{
public:
	StraightHorizontalTile() = default;
	// プレイヤーがこのタイルに入ったときの処理
	void OnEnter(MiniCharacter* character) override;
	// プレイヤーがこのタイルから出たときの処理
	void OnExit(MiniCharacter* character) override;

};

