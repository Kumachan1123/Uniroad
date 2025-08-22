/*
*	@file RightUpTile.cpp
*	@brief 右上タイルの処理を定義するクラス
*/
#include "pch.h"
#include "RightUpTile.h"
/*
*	@brief タイルに入ったときの処理
*	@details キャラクターがこのタイルに入ったときの処理を定義する。
*	@param character タイルに入ったキャラクターへのポインタ
*	@return なし
*/
void RightUpTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// 侵入不可なら停止
	if (character->GetVelocity().x > 0.0f || character->GetVelocity().z < 0.0f)
		character->SetVelocity(Vector3::Zero);
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
void RightUpTile::OnExit(MiniCharacter* character)
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
void RightUpTile::OnCenterReached(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 右方向からタイルに入ったら上に方向転換
	if (character->GetVelocity().x < 0.0f)
		character->SetVelocity(Vector3(0.0f, 0.0f, -1.0f));
	// 上方向からタイルに入ったら左に方向転換
	else if (character->GetVelocity().z > 0.0f)
		character->SetVelocity(Vector3(1.0f, 0.0f, 0.0f));

}


