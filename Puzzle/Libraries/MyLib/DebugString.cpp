/*
	@file	DebugString.cpp
	@brief	デバッグ文字列の描画クラス
*/
#include "pch.h"
#include "DebugString.h"
#include <cstdarg>
#include <cassert>

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
mylib::DebugString::DebugString(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* fontFilePath
)
	:
	m_strings{}
{
	assert(device);
	assert(context);

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, fontFilePath);

	m_fontHeight = m_spriteFont->GetLineSpacing();
	m_color = DirectX::Colors::Black;
}

//---------------------------------------------------------
// 描画する文字列を登録する
//---------------------------------------------------------
void mylib::DebugString::AddString(const char* format, ...)
{
	// 可変長引数を扱うためのデータ型：va_list
	va_list args;

	// 登録する文字列の長さを求める
	va_start(args, format);
	int length = vsnprintf(nullptr, 0, format, args);
	va_end(args);

	// 登録する文字列の大きさを調整する
	std::string appendString;
	appendString.resize(length + 1);	// +1 for the '\0'

	// 登録する文字列を作成する
	va_start(args, format);
	vsnprintf(&appendString[0], length + 1, format, args);
	va_end(args);

	// 作成した文字列を文字列配列に登録する
	m_strings.push_back(appendString);
}

//---------------------------------------------------------
// デバッグ文字列を描画する
//---------------------------------------------------------
void mylib::DebugString::Render(DirectX::CommonStates* states)
{
	UNREFERENCED_PARAMETER(states);	// Beginのパラメータ設定用

	m_spriteBatch->Begin();

	// 文字列の描画位置
	DirectX::SimpleMath::Vector2 pos{ DirectX::SimpleMath::Vector2::Zero };

	for (unsigned int i = 0; i < m_strings.size(); i++)
	{
		// 表示する行の高さを計算する
		pos.y = m_fontHeight * i;

		// 描画する
		m_spriteFont->DrawString(
			m_spriteBatch.get(),
			m_strings[i].c_str(),
			pos,
			m_color
		);
	}

	m_spriteBatch->End();

	// 描画後、文字列配列をクリアする
	m_strings.clear();
}
