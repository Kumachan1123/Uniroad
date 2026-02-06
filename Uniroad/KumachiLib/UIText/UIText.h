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
	std::wstring text; // 描画するテキスト
	DirectX::SimpleMath::Vector2 position; // テキストの位置
	DirectX::SimpleMath::Color color; // テキストの色
	float scale; // テキストのスケール
};

// UIテキストクラス
class UIText
{
public:
	// アクセサ

	// 文字列の追加
	void AddString(const std::wstring& text) { m_lines.push_back(text); }
	// 全クリア
	void Clear()
	{
		m_lines.clear();
		m_currentCharIndex = 0;
	}
	// 位置の設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_textInfo.position = position; }
	// 色の設定
	void SetColor(const DirectX::SimpleMath::Color& color) { m_textInfo.color = color; }
	// スケールの設定
	void SetScale(float scale) { m_textInfo.scale = scale; }
	// テキストアライメントの設定
	void SetAlignment(TextAlignment alignment) { m_alignment = alignment; }
	// 点滅間隔の設定
	void SetCursorBlinkInterval(float interval) { m_cursorBlinkInterval = interval; }
public:

	// コンストラクタ
	UIText();
	// デストラクタ
	~UIText();
	// 初期化
	void Initialize(DX::DeviceResources* pDR);
	// 更新
	void Update(float deltaTime);
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
	// テキスト全体
	std::wstring m_fullText;
	// 表示したい行リスト
	std::vector<std::wstring> m_lines;
	// 実際に表示中
	std::vector<std::wstring> m_displayLines;
	// 今タイプ中の行
	size_t m_currentLine;
	// 行間
	float m_fontHeight;

	// 今表示されている文字数
	size_t m_currentCharIndex;
	// 経過時間
	float m_elapsedTime;
	// １文字表示にかかる時間（秒）
	float m_charInterval;
	// 文字表示が全部終わったか
	bool m_isComplete;
	// ▼の点滅状態
	bool m_cursorVisible;
	// ▼の点滅用タイマー
	float m_cursorTimer;
	// 点滅速度（秒）
	float m_cursorBlinkInterval;

};
/*
if not exist "$(ProjectDir)Resources\Fonts" mkdir "$(ProjectDir)Resources\Fonts"

if not exist "$(ProjectDir)Resources\Fonts\PixelMplus12.spritefont" (
	"$(ProjectDir)Tools\MakeSpriteFont.exe" "PixelMplus12" "$(ProjectDir)Resources\Fonts\PixelMplus12.spritefont"
	/FontSize:32
	/CharacterRegion:32-126
	/CharacterRegion:12352-12543
	/CharacterRegion:19968-40959
	/FastPack
)
動く
if not exist "$(ProjectDir)Resources\Fonts" mkdir "$(ProjectDir)Resources\Fonts"

if not exist "$(ProjectDir)Resources\Fonts\PixelMplus12.spritefont" (
	"$(ProjectDir)Tools\MakeSpriteFont.exe" "PixelMplus12" "$(ProjectDir)Resources\Fonts\PixelMplus12.spritefont" /FontSize:32 /CharacterRegion:32-126 /CharacterRegion:12352-12543 /CharacterRegion:19968-40959 /FastPack
)


*/