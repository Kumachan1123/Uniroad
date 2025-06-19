/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once

// 前方宣言
class CommonResources;

class IScene
{
public:
	// シーンＩＤ
	enum class SceneID : unsigned int
	{
		NONE,
		LAB,
		/*TITLE,
		SETTING,
		STAGESELECT,
		PLAY,
		CLEAR,
		GAMEOVER*/
	};


public:
	virtual ~IScene() = default;

	virtual void Initialize(CommonResources* resources) = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;


	virtual SceneID GetNextSceneID() const = 0;
};
