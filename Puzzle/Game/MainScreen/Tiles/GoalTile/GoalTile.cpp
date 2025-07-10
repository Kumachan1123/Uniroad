/*
*	@file GoalTile.cpp
*	@brief ゴールタイルの処理を定義するクラス
*/
#include "pch.h"
#include "GoalTile.h"

/*
*	@brief タイルに入ったときの処理
*	@details キャラクターがこのタイルに入ったときの処理を定義する。
*	@param character タイルに入ったキャラクターへのポインタ
*	@return なし
*/
void GoalTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;

	// ゴールに到達したことを通知

	character->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	// 通過記録
	character->SetEnteredTile(this);
}

/*
*	@brief タイルから出たときの処理
*	@details キャラクターがこのタイルから出たときの処理を定義する。
*	@param character タイルから出たキャラクターへのポインタ
*	@return なし
*/
void GoalTile::OnExit(MiniCharacter* character)
{
	// フラグを解除する
	character->ResetEnteredTiles();
}
