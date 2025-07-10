/*
*	@file LeftDownTile.cpp
*	@brief 左下タイルの処理を定義するクラス
*/
#include "pch.h"
#include "LeftDownTile.h"
/*
*	@brief タイルに入ったときの処理
*	@details キャラクターがこのタイルに入ったときの処理を定義する。
*	@param character タイルに入ったキャラクターへのポインタ
*	@return なし
*/
void LeftDownTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// 左方向からタイルに入ったら下に方向転換
	if (character->GetVelocity().x > 0.0f)
		character->SetVelocity(Vector3(0.0f, 0.0f, 1.0f));
	// 下方向からタイルに入ったら左に方向転換
	else if (character->GetVelocity().z < 0.0f)
		character->SetVelocity(Vector3(-1.0f, 0.0f, 0.0f));
	// 通過記録
	character->SetEnteredTile(this);
}
/*
*	@brief タイルから出たときの処理
*	@details キャラクターがこのタイルから出たときの処理を定義する。
*	@param character タイルから出たキャラクターへのポインタ
*	@return なし
*/
void LeftDownTile::OnExit(MiniCharacter* character)
{
	// フラグを解除する
	character->ResetEnteredTiles();
}
