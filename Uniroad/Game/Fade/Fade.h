/*
*	@file Fade.h
*	@brief フェードクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"
#include "Game/Interface/IImage.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
#include "KumachiLib/Animaiton/Animation.h"

// 前方宣言
class CommonResources;

// フェードクラス
class Fade : public IImage
{
public:
	// public列挙型
	// フェード状態
	enum class FadeState
	{
		None,// なし
		// 以下、フェードイン・アウトの開始状態
		FadeIn,// フェードイン
		FadeOut,// フェードアウト
		// 以下、フェードイン・アウトの途中状態
		FadeInEnd,// フェードイン終了
		FadeOutEnd,// フェードアウト終了
	};
public:
	// アクセサ
	// フェード状態の取得
	void SetState(FadeState state) { m_fadeState = state; }
	// フェード状態の設定
	FadeState GetState() const { return m_fadeState; }
	// ロゴの位置を取得
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ロゴの位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// ロゴのサイズを取得
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// ロゴのサイズを設定
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public関数
	// コンストラクタ
	Fade();
	// デストラクタ
	~Fade();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新
	void Update(float elapsedTime)override;
	// 画像を表示
	void Render()override;
private:
	// private関数
	// 定数バッファを更新
	void UpdateConstantBuffer()override;
	// フェードアウト
	void FadeOut(float elapsedTime);
	// フェードイン
	void FadeIn(float elapsedTime);
public:
	// public定数
	// フェード最小値
	static const float FADE_MIN;
	// フェード最大値
	static const float FADE_MAX;
	// フェード速度
	static const float FADE_SPEED;
	// フェード初期値
	static const float FADE_INIT;
	// フェードの滑らかさ
	static const float FADE_SMOOTHNESS;
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
	// フェード状態
	FadeState m_fadeState;
	// 画像
	std::unique_ptr<Image> m_pImage;
	// ロゴの矩形
	Rect m_rect;
	// フェード時間
	float m_fadeTime;
	// 定数バッファ
	FadeBuffer m_fadeBuffer;
	// 座標
	DirectX::SimpleMath::Vector2 m_position;
	// サイズ
	DirectX::SimpleMath::Vector2 m_size;
};