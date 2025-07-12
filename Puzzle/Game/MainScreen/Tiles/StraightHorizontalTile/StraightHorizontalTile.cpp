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
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// 横方向（x軸±方向）のみ侵入可能
	Vector3 v = character->GetVelocity();
	if (v == Vector3::Left || v == Vector3::Right)
	{
		// 侵入可能なら通過記録のみ
		character->SetEnteredTile(this);
		// 速度ベクトルはそのまま
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
void StraightHorizontalTile::OnExit(MiniCharacter* character)
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
void StraightHorizontalTile::OnCenterReached(MiniCharacter* character)
{
	return;
}
