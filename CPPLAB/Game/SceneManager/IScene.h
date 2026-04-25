/*
*	@file IScene.h
*	@brief シーンのインターフェイスクラス
*/
#pragma once

// 前方宣言
class CommonResources;

// シーンのインターフェイスクラス
class IScene
{
public:
	// シーンID
	enum class SceneID : unsigned int
	{
		NONE,// なし
		REHABILI,// ゲームづくりのリハビリ
	};
public:
	// publicメンバ関数
	// デストラクタ
	virtual ~IScene() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update(float elapsedTime) = 0;
	// 描画
	virtual void Render() = 0;
	// 終了
	virtual void Finalize() = 0;
	// 次のシーンIDを取得する
	virtual SceneID GetNextSceneID() const = 0;
	// ステージ番号を取得する
	virtual int GetStageNumber() const = 0;
	// ステージ番号を設定する
	virtual void SetStageNumber(int stageNumber) = 0;
};
