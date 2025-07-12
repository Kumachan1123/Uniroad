#pragma once
//// 自作ヘッダーファイル
//#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
class MiniCharacter;
// タイルベースクラス
class TileBase
{
public:
	// public関数
	virtual ~TileBase() = default;
	// タイルに入ったときの処理
	virtual void OnEnter(MiniCharacter* miniCharacter) = 0;
	// タイルから出たときの処理
	virtual void OnExit(MiniCharacter* miniCharacter) = 0;
	// タイルの中心に来た時の処理
	virtual void OnCenterReached(MiniCharacter* miniCharacter) = 0;

};