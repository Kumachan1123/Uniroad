/*
*	@file UIText.h
*	@brief UIテキストクラス
*/
#pragma once
#include <string>
#include <memory>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DeviceResources.h>

/*
*	@brief テキストアライメント列挙型
*/
enum class TextAlignment
{
	LEFT,
	CENTER,
	RIGHT
};

/*
*	@brief  テキスト描画情報
*/
struct TextInfo
{
	std::string text; // 描画するテキスト
	DirectX::SimpleMath::Vector2 position; // テキストの位置
	DirectX::SimpleMath::Color color; // テキストの色
	float scale; // テキストのスケール
};

// UIテキストクラス
class UIText
{
public:
	// アクセサ
	// テキストの設定
	void SetText(const std::string& text) { m_textInfo.text = text; }
	// 位置の設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_textInfo.position = position; }
	// 色の設定
	void SetColor(const DirectX::SimpleMath::Color& color) { m_textInfo.color = color; }
	// スケールの設定
	void SetScale(float scale) { m_textInfo.scale = scale; }
	// テキストアライメントの設定
	void SetAlignment(TextAlignment alignment) { m_alignment = alignment; }
public:

	// コンストラクタ
	UIText();
	// デストラクタ
	~UIText();
	// 初期化
	void Initialize(DX::DeviceResources* pDR);

	// 描画
	void Render();
	// 後処理
	void Finalize();
private:
	// privateメンバ関数
	DirectX::SimpleMath::Vector2 CalculateAlignedPosition();
private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// テキスト描画情報
	TextInfo m_textInfo;
	// テキストアライメント
	TextAlignment m_alignment;

};