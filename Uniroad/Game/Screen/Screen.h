/*
*	@file	Screen.h
*	@brief	画面にまつわる定数をまとめたクラス
*/
#pragma once
class Screen
{
public:
	// 定数
	// 画面サイズ
	// 幅
	static const int WIDTH = 1280;
	// 高さ
	static const int HEIGHT = 720;
	// 中央X座標
	static const int CENTER_X = WIDTH / 2;
	// 中央Y座標
	static const int CENTER_Y = HEIGHT / 2;
	// 上端
	static const int TOP = 0;
	// 下端
	static const int BOTTOM = HEIGHT;
	// 左端
	static const int LEFT = 0;
	// 右端
	static const int RIGHT = WIDTH;
public:
	// public関数
	// コンストラクタ
	Screen() = default;
	// デストラクタ
	~Screen() = default;
};
