/*
*	@file CrossTile.cpp
*	@brief 十字タイルの処理を定義するクラス
*/
#include "pch.h"
#include "CrossTile.h"

void CrossTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// キャラクターを動かすフラグを立てる
	character->SetMoving(true);
	// 通過記録
	character->SetEnteredTile(this);
}

void CrossTile::OnExit(MiniCharacter* character)
{
	// フラグを解除する
	character->ResetEnteredTiles();
}

void CrossTile::OnCenterReached(MiniCharacter* character)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
	return;
}
