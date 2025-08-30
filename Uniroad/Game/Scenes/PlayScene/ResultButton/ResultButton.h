/*
*	@file	ResultButton.h
*	@brief	リザルト画面のボタンを管理するクラスのヘッダーファイル
*/
#pragma once
// 標準ライブラリ
#include <vector>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Button/Button.h"
#include "Game/Interface/IButton.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
#include "KumachiLib/Animaiton/Animation.h"
#include "KumachiLib/FileCounter/FileCounter.h"

// 前方宣言
class CommonResources;

// リザルト画面のボタンクラス
class ResultButton : public IButton
{
public:
	// 列挙型
	// シーンID
	enum SceneID
	{
		NONE = -1, // 無効なシーンID
		SELECT_STAGE = 0,// ステージ選択
		REPLAY = 1,// リプレイ
	};
public:
	// アクセサ
	// ボタンが押されたかどうかを取得
	bool IsPressed() const override { return m_isPressed; }
	// ボタンが押されたかどうかを設定
	void SetPressed(bool isPressed) override { m_isPressed = isPressed; }
	// 座標を取得
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// 座標を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// サイズを取得
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// サイズを設定
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
	// ヒットしたボタンのインデックスを取得
	int GetHitButtonIndex()const { return (int)(m_hitButtonIndex); }
	// 押されたボタンの番号を取得
	int GetPressedButtonIndex() const { return m_pressedButtonIndex; }
	// シーンから結果を受け取る
	void SetResult(bool gameOver, bool gameClear);
	// シーンからステージ番号を受け取る
	void SetStageNum(int stageNum) { m_stageNum = stageNum; }
	// ステージ番号を取得
	int GetStageNum() const { return m_stageNum; }
	// このクラスが有効かどうかを取得
	bool IsEnable() const { return m_enable; }
	// このクラスが有効かどうかを設定
	void SetEnable(bool enable) { m_enable = enable; }
	// シーン番号取得
	SceneID GetSceneNum() const { return m_num; }
	// シーン番号設定
	void SetSceneNum(SceneID num) { m_num = num; }
public:
	// public関数
	// コンストラクタ
	ResultButton();
	// デストラクタ
	~ResultButton();
	// 初期化 
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新する
	void Update(float elapsedTime)override;
	// 描画する
	void Render()override;
private:
	// private関数
	// アニメーションシーケンスを作成
	void CreateAnimationSequence();
	// 定数バッファを更新
	void UpdateConstantBuffer()override;
public:
	// public定数
	// ボタンが押されていない状態のインデックス
	static constexpr int NONE_BUTTON_INDEX = -1;
private:
	// private定数
	// 表示位置（左上）
	static const std::vector<DirectX::SimpleMath::Vector2> POSITIONS;
	// 表示サイズ
	static const std::vector<DirectX::SimpleMath::Vector2> SIZES;
	// ホバー時の拡大率を定義
	static constexpr float SCALE_ON = 1.125f;
	// ホバーしていないときの拡大率を定義
	static constexpr float SCALE_OFF = 1.0f;
	// 補間係数
	static constexpr float SCALE_SPEED = 8.0f;
	// アニメーション関連
	// アニメーション初期待機
	static constexpr float ANIMATION_WAIT_TIME = 1.0f;
	// アニメーション移動時間
	static constexpr float ANIMATION_MOVE_DURATION = 1.0f;
	// アニメーション開始Y座標
	static constexpr float ANIMATION_START_Y = 2.5f;
	// アニメーション第1移動オフセット
	static constexpr float ANIMATION_MOVE_OFFSET_1 = 0.5f;
	// アニメーション第2移動オフセット
	static constexpr float ANIMATION_MOVE_OFFSET_2 = 0.1f;
	// アニメーション終了Y座標
	static constexpr float ANIMATION_END_Y = 1.5f;
private:
	// private変数
	// 共通リソース 
	CommonResources* m_pCommonResources;
	// リトライ/次のステージへ進むボタン
	std::unique_ptr<Button> m_pRetryNextStageButton;
	// ステージセレクトに進むボタン
	std::unique_ptr<Button> m_pToStageSelectButton;
	// ボタン配列
	std::vector<std::unique_ptr<Button>> m_buttons;
	// アニメーション
	std::unique_ptr<Animation> m_pAnimation;
	// ボタンの矩形
	std::vector<Rect> m_buttonRects;
	// 定数バッファ
	SpriteSheetBuffer m_spriteSheetBuffer;
	// 座標
	DirectX::SimpleMath::Vector2 m_position;
	// サイズ
	DirectX::SimpleMath::Vector2 m_size;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 当たり判定
	std::vector<bool> m_isHit;
	// 前フレームの当たり判定
	std::vector<bool>m_prevIsHit;
	// 当たったボタンの番号
	int m_hitButtonIndex;
	// 押されたボタンの番号
	int m_pressedButtonIndex;
	// ボタンが押されたフラグ
	bool m_isPressed;
	// 各ボタンのホバー時の拡大率
	std::vector<float> m_hoverScales;
	// ゲームクリアか
	bool m_gameClear;
	// ゲームオーバーか
	bool m_gameOver;
	// 今のステージ番号
	int m_stageNum;
	// このクラスが有効かどうか
	bool m_enable;
	// シーンID
	SceneID m_num;
};