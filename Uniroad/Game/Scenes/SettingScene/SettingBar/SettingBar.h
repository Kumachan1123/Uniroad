/*
*	@file SettingBar.h
*	@brief 設定画面のバークラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ScrollBar/ScrollBar.h"
#include "Game/Interface/IScrollBar.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
#include "KumachiLib/Animaiton/Animation.h"
#include "KumachiLib/Image/Image.h"

// 前方宣言
class CommonResources;

// 設定画面のバークラス
class SettingBar : public IScrollBar
{
public:
	// アクセサ
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
	SettingBar();
	// デストラクタ
	~SettingBar();
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
	static const std::vector<DirectX::SimpleMath::Vector2> BAR_POSITIONS;
	static const std::vector<DirectX::SimpleMath::Vector2> CONTROLLER_POSITIONS;
	// 表示サイズ
	static const std::vector<DirectX::SimpleMath::Vector2> BAR_SIZES;
	static const std::vector<DirectX::SimpleMath::Vector2> CONTROLLER_SIZES;
private:
	// private変数
	// 共通リソース 
	CommonResources* m_pCommonResources;
	// 設定バーの棒部分の矩形
	std::vector<Rect> m_barRects;
	// 設定バーの操作部分の矩形
	std::vector<Rect> m_controllerRects;
	// スクロールバー
	std::unique_ptr<ScrollBar> m_pBGMBar;
	std::unique_ptr<ScrollBar> m_pSEBar;
	// スクロールバー配列
	std::vector<std::unique_ptr<ScrollBar>> m_bars;
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
	// スクロールバーの当たった地点X
	std::vector<float> m_ratioX;
};