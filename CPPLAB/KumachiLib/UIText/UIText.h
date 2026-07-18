/*
*	@file UIText.h
*	@brief UIテキストクラス
*/
#pragma once
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
	float scale = 1.0f; // テキストのスケール
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
	// 次へ要求
	void RequestNext() { m_requestNext = true; }
	// ここで止めてね命令
	void InsertWaitPoint() { m_lines.push_back(L"__WAIT__"); }
	// 全部表示し終わったか
	bool IsFinishedAll() const { return m_finishedAll; }
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
	// アライメントに基づいて位置を計算する
	DirectX::SimpleMath::Vector2 CalculateAlignedPosition();
	// 点滅処理
	void UpdateCursorBlink(float deltaTime);
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
	// 最大表示行数
	size_t m_maxDisplayLines = 3;
	// 今待ち中？
	bool m_waitForInput = false;
	// 外からの「次へ」要求
	bool m_requestNext = false;
	// 一番最後の行を表示したら
	bool m_finishedAll = false;

};
