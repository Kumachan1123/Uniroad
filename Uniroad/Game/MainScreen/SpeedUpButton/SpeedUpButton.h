/*
*	@file	SpeedUpButton.h
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
class SpeedUpButton : public IButton
{
private:
	//// private構造体
	//// シェーダーに渡す定数バッファ
	//struct ConstBuffer
	//{
	//	DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
	//	DirectX::SimpleMath::Matrix matView;    // ビュー行列
	//	DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
	//	DirectX::SimpleMath::Vector4 count;     // カウント
	//	DirectX::SimpleMath::Vector4 height;    // 高さ
	//	DirectX::SimpleMath::Vector4 width;     // 幅
	//};
public:
	// アクセサ
	// スピードアップボタンが押されたかどうかを取得
	bool IsPressed() const override { return m_isPressed; }
	// スピードアップボタンが押されたかどうかを設定
	void SetPressed(bool isPressed) override { m_isPressed = isPressed; }
	// 座標を取得
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// 座標を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// サイズを取得
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// サイズを設定
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public関数
	// コンストラクタ
	SpeedUpButton();
	// デストラクタ
	~SpeedUpButton();
	// 初期化 
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新する
	void Update(float elapsedTime)override;
	// 描画する
	void Render()override;
private:
	// private関数
	// 定数バッファを更新
	void UpdateConstantBuffer()override;
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
	Rect m_buttonRect;
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
	bool m_isHit;
	// ボタンが押されたフラグ
	bool m_isPressed;
};
