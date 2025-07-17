/*
*	@file BlockTile.h
*	@brief 侵入不可のタイルの処理を定義するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// 侵入不可タイルクラス
class BlockTile : public TileBase
{
public:
	// public関数
	// コンストラクタ
	BlockTile() = default;
	// プレイヤーがこのタイルに入ったときの処理
	void OnEnter(MiniCharacter* character) override;
	// プレイヤーがこのタイルから出たときの処理
	void OnExit(MiniCharacter* character) override;
	// プレイヤーがこのタイルの中心に来たときの処理
	void OnCenterReached(MiniCharacter* character) override;
};