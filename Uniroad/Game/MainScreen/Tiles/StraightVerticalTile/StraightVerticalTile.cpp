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
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;

	// 縦方向のみ侵入可能
	Vector3 v = character->GetVelocity();
	if (v == Vector3::Backward || v == Vector3::Forward)
	{
		// 侵入可能なら通過記録のみ
		character->SetEnteredTile(this);
		// 速度ベクトルはそのまま
		// キャラクターを動かすフラグを立てる
		character->SetMoving(true);
	}
	else
	{
		// 侵入不可なら停止
		character->SetVelocity(Vector3::Zero);
		// 通過記録はしない
	}
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
/*
*	@brief タイルの中心に来たときの処理
*	@details キャラクターがこのタイルの中心に来たときの処理を定義する。
*	@param character タイルの中心に来たキャラクターへのポインタ
*	@return なし
*/
void StraightVerticalTile::OnCenterReached(MiniCharacter* character)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
	return;
}
