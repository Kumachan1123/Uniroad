/*
*	@file BlockTile.cpp
*	@brief 侵入不可のタイルの処理を定義するクラス
*/
#include "pch.h"
#include "BlockTile.h"

/*
*	@brief プレイヤーがこのタイルに入ったときの処理
*	@details 侵入不可のタイルなので、何もしない。
*	@param character タイルに入ったキャラクター
*	@return なし
*/
void BlockTile::OnEnter(MiniCharacter* character)
{
	using namespace DirectX::SimpleMath;
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
	// キャラクターを動かすフラグを折る
	character->SetMoving(false);
	// キャラクターの速度ベクトルをゼロにする
	character->SetVelocity(Vector3::Zero);
	// 通過記録
	character->SetEnteredTile(this);
}
/*
*	@brief プレイヤーがこのタイルから出たときの処理
*	@details 侵入不可のタイルなので、何もしない。
*	@param character タイルから出たキャラクター
*	@return なし
*/
void BlockTile::OnExit(MiniCharacter* character)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
}
/*
*	@brief プレイヤーがこのタイルの中心に来たときの処理
*	@details 侵入不可のタイルなので、何もしない。
*	@param character タイルの中心に来たキャラクター
*	@return なし
*/
void BlockTile::OnCenterReached(MiniCharacter* character)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
}
