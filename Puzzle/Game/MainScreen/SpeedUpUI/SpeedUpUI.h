/*
*	@file	SpeedUpUI.h
*	@brief	スピードアップボタンクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/MouseClick/MouseClick.h"
#include "Game/Interface/IButton.h"
#include "KumachiLib/Button/Button.h"

// 前方宣言
class CommonResources;

// スピードアップボタンクラス
class SpeedUpUI : public IButton
{
public:
	// アクセサ
	// スピードアップボタンが押されたかどうかを取得
	bool IsPressed() const { return m_isPressed; }
	// スピードアップボタンが押されたかどうかを設定
	void SetPressed(bool isPressed) { m_isPressed = isPressed; }
public:
	// public関数
	// コンストラクタ
	SpeedUpUI();
	// デストラクタ
	~SpeedUpUI();
	// 初期化 
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新する
	void Update(float elapsedTime)override;
	// 描画する
	void Render()override;
private:
	// private定数
	// 表示位置（左上）
	static const DirectX::SimpleMath::Vector2 POSITION;
	// 表示サイズ
	static const DirectX::SimpleMath::Vector2 SIZE;
private:
	// private変数
	// 共通リソース 
	CommonResources* m_pCommonResources;
	// スピードアップボタン
	std::unique_ptr<Button> m_pButton;
	// ボタンの矩形
	Button::ButtonRect m_buttonRect;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 当たり判定
	bool m_isHit;
	// ボタンが押されたフラグ
	bool m_isPressed;
};
