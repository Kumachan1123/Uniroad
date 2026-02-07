/*
*	@file TitleLogo.h
*	@brief タイトルロゴクラス
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

// タイトルロゴクラス
class TitleLogo : public IImage
{
public:
	// アクセサ
	// ロゴの位置を取得
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ロゴの位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// ロゴのサイズを取得
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// ロゴのサイズを設定
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
	// アニメーションを進める
	void AdvanceAnimation() { m_pAnimation->AdvanceSequence(); }
public:
	// public関数
	// コンストラクタ
	TitleLogo();
	// デストラクタ
	~TitleLogo();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新
	void Update(float elapsedTime)override;
	// 画像を表示
	void Render()override;
private:
	// private関数
	// アニメーションシーケンスを作成
	void CreateAnimationSequence();
	// 定数バッファを更新
	void UpdateConstantBuffer()override;
private:
	// private定数
	// 表示位置（左上）
	static constexpr DirectX::SimpleMath::Vector2 POSITION = { 0.125f, 0.15f };
	// 表示サイズ
	static constexpr DirectX::SimpleMath::Vector2 SIZE = { 0.55f / 2.5f, 0.35f / 2.5f };
	// 拡大にかける秒数
	static constexpr float ENLARGE_DURATION = 0.5f;
	// 待機時間
	static constexpr float WAIT_DURATION = 2.0f;
	// 縮小・移動にかける秒数
	static constexpr float SHRINK_MOVE_DURATION = 0.5f;
	// 退出アニメ時間
	static constexpr float EXIT_MOVE_DURATION = 0.5f;
	// 画面中央
	static constexpr   DirectX::SimpleMath::Vector2 CENTER_POS = { 0.5f, 0.5f };
	// 拡大時サイズ
	static constexpr   DirectX::SimpleMath::Vector2 ENLARGE_SIZE = { 0.55f, 0.35f };
	// 退出時オフセット
	static constexpr   DirectX::SimpleMath::Vector2 EXIT_OFFSET = { -1.5f, 0.0f };
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 画像
	std::unique_ptr<Image> m_pImage;
	// アニメーション
	std::unique_ptr<Animation> m_pAnimation;
	// ロゴの位置
	DirectX::SimpleMath::Vector2 m_position;
	// ロゴのサイズ
	DirectX::SimpleMath::Vector2 m_size;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// ロゴの矩形
	Rect m_logoRect;
	// 定数バッファ
	SpriteSheetBuffer m_spriteSheetBuffer;
};