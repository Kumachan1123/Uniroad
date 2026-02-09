/*
*	@file		: UIText.cpp
*	@brief		: UIテキストクラス
*/
#include "pch.h"
#include "UIText.h"
/*
*	@brief コンストラクタ
*	@details テキスト描画情報とアライメントの初期化を行う
*	@return なし
*/
UIText::UIText()
	:m_alignment(TextAlignment::LEFT)
	, m_currentCharIndex(0)
	, m_elapsedTime(0.0f)
	, m_charInterval(0.05f)
	, m_isComplete(false)
	, m_cursorVisible(false)
	, m_requestNext(false)
	, m_waitForInput(false)
	, m_finishedAll(false)
	, m_cursorTimer(0.0f)
	, m_cursorBlinkInterval(0.5f)
	, m_fullText(L"")
	, m_spriteBatch(nullptr)
	, m_spriteFont(nullptr)
	, m_textInfo()
	, m_fontHeight(50.0f)
	, m_currentLine(0)
{
	// 文字の初期化
	m_textInfo.text = L"";
	// 位置の初期化
	m_textInfo.position = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	// 色の初期化
	m_textInfo.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
	// スケールの初期化
	m_textInfo.scale = 1.0f;

}
/*
*	@brief デストラクタ
*	@details スプライトバッチとスプライトフォントの解放を行う
*	@return なし
*/
UIText::~UIText()
{
}
/*
*	@brief 初期化
*	@details スプライトバッチとスプライトフォントの作成を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void UIText::Initialize(DX::DeviceResources* pDR)
{
	// デバイスコンテキストの取得
	auto context = pDR->GetD3DDeviceContext();
	// デバイスの取得
	auto device = pDR->GetD3DDevice();

	// スプライトバッチの作成
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// スプライトフォントの作成
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/PixelMplus12-Regular.spritefont");
}
/*
*	@brief 更新
*	@details テキストの更新を行う
*	@param deltaTime 前フレームからの経過時間（秒）
*	@return なし
*/
void UIText::Update(float deltaTime)
{
	// 一回だけ点滅更新
	bool needBlink = m_isComplete || m_waitForInput;
	// 完了後の点滅
	if (needBlink)UpdateCursorBlink(deltaTime);
	// ─────────────────────────────
	// ■ 待機中
	// ─────────────────────────────
	if (m_waitForInput)
	{
		if (m_requestNext)
		{
			m_waitForInput = false;
			m_requestNext = false;
			//m_currentLine++;
			m_cursorVisible = false;
			m_cursorTimer = 0.0f;
			// WAIT明け直後は必ず器を作る
			if (m_currentLine < m_lines.size() &&
				m_lines[m_currentLine] != L"__WAIT__")
			{
				m_displayLines.push_back(L"");

				if (m_displayLines.size() > m_maxDisplayLines)
				{
					m_displayLines.erase(m_displayLines.begin());
				}
			}
		}
		return;
	}

	// ─────────────────────────────
	// ■ 通常更新
	// ─────────────────────────────
	if (m_currentLine >= m_lines.size())
		return;

	m_elapsedTime += deltaTime;

	while (m_elapsedTime >= m_charInterval)
	{
		const std::wstring& full = m_lines[m_currentLine];

		// ── 待機ポイント ──
		if (full == L"__WAIT__")
		{
			m_waitForInput = true;
			m_cursorVisible = true;
			m_currentLine++;
			return;
		}

		// ── 文字送り ──
		if (m_currentCharIndex < full.size())
		{
			// 空白の行対策
			if (m_displayLines.empty())m_displayLines.push_back(L"");
			// １文字追加
			m_displayLines.back() += full[m_currentCharIndex];
			// 次の文字へ
			m_currentCharIndex++;
		}
		else
		{
			// ── 1行おわり ──
			m_currentLine++;
			m_currentCharIndex = 0;

			// すべて終了したか
			if (m_currentLine >= m_lines.size())
			{
				m_cursorVisible = true;
				m_cursorTimer = 0.0f;
				m_isComplete = true;
				m_finishedAll = true;
				break;
			}

			// 次がWAITなら器は作らない！
			if (m_lines[m_currentLine] == L"__WAIT__")
			{
				m_waitForInput = true;
				return;
			}
			// 通常行なら器を作る
			m_displayLines.push_back(L"");

			if (m_displayLines.size() > m_maxDisplayLines)
			{
				m_displayLines.erase(m_displayLines.begin());
			}
		}

		m_elapsedTime -= m_charInterval;
	}


}


/*
*	@brief 描画
*	@details スプライトバッチとスプライトフォントを使用してテキストの描画を行う
*	@return なし
*/
void UIText::Render()
{
	using namespace DirectX::SimpleMath;

	if (!m_spriteBatch || !m_spriteFont)return;

	m_spriteBatch->Begin();

	Vector2 pos = m_textInfo.position;
	size_t lineCount = m_displayLines.size();
	// 最大より少ないときは下から積む
	if (lineCount > m_maxDisplayLines)
		pos.y += m_fontHeight * (m_maxDisplayLines - lineCount);

	for (size_t i = 0; i < m_displayLines.size(); i++)
	{
		std::wstring text = m_displayLines[i];

		int lastIndex = (int)m_displayLines.size() - 1;

		bool canShowCursor =
			(m_waitForInput || m_isComplete) &&
			m_cursorVisible;

		if (i == lastIndex && canShowCursor)
		{
			text += L"　▼";
		}

		m_spriteFont->DrawString(
			m_spriteBatch.get(),
			text.c_str(),
			pos,
			m_textInfo.color,
			0.0f,
			Vector2::Zero,
			m_textInfo.scale
		);

		pos.y += m_fontHeight * m_textInfo.scale;
	}



	m_spriteBatch->End();
}

/*
*	@brief 後処理
*	@details スプライトバッチとスプライトフォントの解放を行う
*	@return なし
*/
void UIText::Finalize()
{
	// スプライトバッチの解放
	m_spriteBatch.reset();
	// スプライトフォントの解放
	m_spriteFont.reset();
}
/*
*	@brief アライメントに応じた位置を計算
*	@details テキストアライメントに応じて描画位置を調整する
*	@return 調整後の位置
*/
DirectX::SimpleMath::Vector2 UIText::CalculateAlignedPosition()
{
	using namespace DirectX::SimpleMath;
	// 元の位置を取得
	Vector2 position = m_textInfo.position;
	// フォントがなかったらそのまま
	if (!m_spriteFont)return position;
	// テキストのサイズを取得
	Vector2 textSize = m_spriteFont->MeasureString(m_textInfo.text.c_str());
	// アライメントに応じて位置を調整
	switch (m_alignment)
	{
		case TextAlignment::LEFT:
			// 左揃えの場合はそのまま
			break;
		case TextAlignment::CENTER:
			// 中央揃えの場合は幅の半分を引く
			position.x -= textSize.x * 0.5f * m_textInfo.scale;
			break;
		case TextAlignment::RIGHT:
			// 右揃えの場合は幅全体を引く
			position.x -= textSize.x * m_textInfo.scale;
			break;
		default:
			break;
	}
	// 調整後の位置を返す
	return position;
}
/*
*	@brief カーソルの点滅更新
*	@details カーソルの点滅状態を更新する
*	@param deltaTime 前フレームからの経過時間（秒）
*	@return なし
*/
void UIText::UpdateCursorBlink(float deltaTime)
{
	m_cursorTimer += deltaTime;
	if (m_cursorTimer >= m_cursorBlinkInterval)
	{
		m_cursorVisible = !m_cursorVisible;
		m_cursorTimer -= m_cursorBlinkInterval;
	}
}
