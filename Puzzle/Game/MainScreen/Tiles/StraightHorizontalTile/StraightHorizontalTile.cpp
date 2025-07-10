/*
 *	@file StraightHorizontalTile.cpp
 *	@brief 直進（横）タイルの処理を定義するクラス
 */
#include <pch.h>
#include "StraightHorizontalTile.h"
 /*
 *	@brief タイルに入ったときの処理
 *	@details キャラクターがこのタイルに入ったときの処理を定義する。
 *	@param character タイルに入ったキャラクターへのポインタ
 *	@return なし
 */
void StraightHorizontalTile::OnEnter(MiniCharacter* character)
{
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// 横方向に進む
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
void StraightHorizontalTile::OnExit(MiniCharacter* character)
{
	// フラグを解除する
	character->ResetEnteredTiles();
}
