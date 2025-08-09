/*
*	@file TileBase.h
*	@brief タイルのベースクラス
*/
#pragma once

// 前方宣言
class MiniCharacter;

// タイルベースクラス
class TileBase
{
public:
	// public関数
	// デストラクタ
	virtual ~TileBase() = default;
	// タイルに入ったときの処理
	virtual void OnEnter(MiniCharacter* miniCharacter) = 0;
	// タイルから出たときの処理
	virtual void OnExit(MiniCharacter* miniCharacter) = 0;
	// タイルの中心に来た時の処理
	virtual void OnCenterReached(MiniCharacter* miniCharacter) = 0;

};