/*
*	@file		BGMTexture.h
*	@brief		「BGM」テクスチャクラス
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

// 「BGM」テクスチャクラス
class BGMTexture : public IImage
{
public:
	// アクセサ
	// 位置を取得
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// 位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// サイズを取得
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// サイズを設定
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public関数
	// コンストラクタ
	BGMTexture();
	// デストラクタ
	~BGMTexture();
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
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 画像
	std::unique_ptr<Image> m_pImage;
	// 位置
	DirectX::SimpleMath::Vector2 m_position;
	// サイズ
	DirectX::SimpleMath::Vector2 m_size;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 矩形
	Rect m_rect;
	// 定数バッファ
	SpriteSheetBuffer m_spriteSheetBuffer;
};