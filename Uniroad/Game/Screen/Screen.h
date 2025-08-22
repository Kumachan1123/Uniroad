/*
	@file	Screen.h
	@brief	��ʂɂ܂��萔���܂Ƃ߂��N���X
*/
#pragma once

class Screen
{
public:
	static const int WIDTH = 1280;
	static const int HEIGHT = 720;

	/*static const int WIDTH = 1920;
	static const int HEIGHT = 1080;*/

	/*static const int WIDTH = 2560;
	static const int HEIGHT = 1440;*/

	//static const int WIDTH = 854;
	//static const int HEIGHT = 480;

	static const int CENTER_X = WIDTH / 2;
	static const int CENTER_Y = HEIGHT / 2;

	static const int TOP = 0;
	static const int BOTTOM = HEIGHT;
	static const int LEFT = 0;
	static const int RIGHT = WIDTH;

	static const int WIDTH_BUTTON = 1920;
	static const int HEIGHT_BUTTON = 1080;

	static const int CENTER_X_BUTTON = WIDTH_BUTTON / 2;
	static const int CENTER_Y_BUTTON = HEIGHT_BUTTON / 2;

	Screen() = default;
	~Screen() = default;
};
