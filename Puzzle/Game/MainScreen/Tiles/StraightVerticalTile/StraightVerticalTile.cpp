/*
*	@file StraightVerticalTile.cpp
*	@brief 直進（縦）タイルの処理を定義するクラス
*/
#include <pch.h>
#include "StraightVerticalTile.h"
/*
*	@brief タイルに入ったときの処理
*	@details キャラクターがこのタイルに入ったときの処理を定義する。
*	@param character タイルに入ったキャラクターへのポインタ
*	@return なし
*/
void StraightVerticalTile::OnEnter(MiniCharacter* character)
{
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// 縦方向に進む
	character->SetVelocity(character->GetVelocity());
	// 通過記録
	character->SetEnteredTile(this);
}
/*
*	@brief タイルから出たときの処理
*	@details キャラクターがこのタイルから出たときの処理を定義する。
*	@param character タイルから出たキャラクターへのポインタ
*	@return なし
*/
void StraightVerticalTile::OnExit(MiniCharacter* character)
{
	// フラグを解除する
	character->ResetEnteredTiles();
}
