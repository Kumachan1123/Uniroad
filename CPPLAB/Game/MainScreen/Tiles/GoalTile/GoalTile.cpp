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
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
	// 2回目以降は何もしない
	if (character->HasEnteredTile(this)) return;
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
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
	// フラグを解除する
	character->ResetEnteredTiles();
}
/*
*	@brief タイルの中心に来たときの処理
*	@details キャラクターがこのタイルの中心に来たときの処理を定義する。
*	@param character タイルの中心に来たキャラクターへのポインタ
*	@return なし
*/
void GoalTile::OnCenterReached(MiniCharacter* character)
{
	// SimpleMath名前空間を使用
	using namespace DirectX::SimpleMath;
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(character);
	// キャラクターの速度をゼロにする
	character->SetVelocity(Vector3::Zero);
	// キャラクターを動かすフラグを止める
	character->SetMoving(false);
	// 表情をハッピーにする
	character->SetExpression(MiniCharacter::Expression::HAPPY);
	// 5秒後にゲームクリアのフラグを立てる
	if (character->GetGameClearSwitchTime() > GAME_CLEAR_SWITCH_TIME)
	{
		// ベースクラスのポインターを取得
		const auto& pMinicharacterBase = dynamic_cast<MiniCharacterBase*>(character->GetParent());
		// ベースクラスのポインターが取れたらゲームクリアフラグを立てる
		if (pMinicharacterBase)	pMinicharacterBase->SetGameClear(true);
		// 取れなかったらエラー
		else assert(false && "MiniCharacterBase is not set correctly.");
	}
}
