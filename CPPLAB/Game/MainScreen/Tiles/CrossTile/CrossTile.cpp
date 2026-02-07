/*
*	@file CrossTile.cpp
*	@brief 十字タイルの処理を定義するクラス
*/
#include "pch.h"
#include "CrossTile.h"
/*
*	@brief プレイヤーがこのタイルに入ったときの処理
*	@details キャラクターを動かすフラグを立てる。
*	@param character タイルに入ったキャラクターへのポインタ
*	@return なし
*/
void CrossTile::OnEnter(MiniCharacter* character)
{
	// SimpleMath名前空間を使用
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
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
void CrossTile::OnExit(MiniCharacter* character)
{
	// フラグを解除する
	character->ResetEnteredTiles();
}
/*
*	@brief タイルの中心に来たときの処理
*	@details キャラクターがこのタイルの中心に来たときの処理を定義する。
*	@param character タイルの中心に来たキャラクターへのポインタ
*	@return なし
*/
void CrossTile::OnCenterReached(MiniCharacter* character)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
	return;
}
