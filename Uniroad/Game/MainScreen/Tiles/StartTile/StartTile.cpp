/*
*	@file StartTile.cpp
*	@brief スタートタイルの処理を定義するクラス
*/
#include "pch.h"
#include "StartTile.h"
/*
*	@brief タイルに入ったときの処理
*	@details キャラクターがこのタイルに入ったときの処理を定義する。
*	@param character タイルに入ったキャラクターへのポインタ
*	@return なし
*/
void StartTile::OnEnter(MiniCharacter* character)
{
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// 縦方向に進む
	character->SetVelocity(character->GetVelocity());
	// キャラクターを動かすフラグを立てる
	character->SetMoving(true);
	// 通過記録
	character->SetEnteredTile(this);
}
/*
*	@brief タイルから出たときの処理
*	@details キャラクターがこのタイルから出たときの処理を定義する。
*	@param character タイルから出たキャラクターへのポインタ
*	@return なし
*/
void StartTile::OnExit(MiniCharacter* character)
{

	// フラグを解除する
	character->ResetEnteredTiles();
}

void StartTile::OnCenterReached(MiniCharacter* character)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
}
