/*
*	@file GoalTile.h
*	@brief ゴールタイルの処理を定義するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/MainScreen/Tiles/TileBase/TileBase.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"

// ゴールタイルクラス
class GoalTile : public TileBase
{
public:
	// public関数
	// コンストラクタ
	GoalTile() = default;
	// プレイヤーがこのタイルに入ったときの処理
	void OnEnter(MiniCharacter* character) override;
	// プレイヤーがこのタイルから出たときの処理
	void OnExit(MiniCharacter* character) override;
	// プレイヤーがこのタイルの中心に来たときの処理
	void OnCenterReached(MiniCharacter* character) override;
private:
	// private定数
	static constexpr float GAME_CLEAR_SWITCH_TIME = 5.0f;
};