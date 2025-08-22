/*
*	@file BackButton.h
*	@brief ステージセレクト画面のボタンを管理するクラスのヘッダーファイル
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

// 前方宣言
class CommonResources;

// ステージセレクト画面のボタンクラス
class BackButton : public IButton
{
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
public:
	// public関数
	// コンストラクタ
	BackButton();
	// デストラクタ
	~BackButton();
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
	static const int NONE_BUTTON_INDEX;
private:
	// private定数
	// 表示位置（左上）
	static const std::vector<DirectX::SimpleMath::Vector2> POSITIONS;
	// 表示サイズ
	static const std::vector<DirectX::SimpleMath::Vector2> SIZES;

private:
	// private変数
	// 共通リソース 
	CommonResources* m_pCommonResources;
	// タイトルに戻るボタン
	std::unique_ptr<Button> m_pBackButton;
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
	// 当たったボタンの番号
	int m_hitButtonIndex;
	// 押されたボタンの番号
	int m_pressedButtonIndex;
	// ボタンが押されたフラグ
	bool m_isPressed;
	// 各ボタンのホバー時の拡大率
	std::vector<float> m_hoverScales;

};