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
{
	// 文字の初期化
	m_textInfo.text = "";
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
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}


/*
*	@brief 描画
*	@details スプライトバッチとスプライトフォントを使用してテキストの描画を行う
*	@return なし
*/
void UIText::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// バッチとフォントがなかったら処理なし
	if (!m_spriteBatch || !m_spriteFont)return;

	// スプライトバッチの開始
	m_spriteBatch->Begin();
	// アライメントに応じた位置を計算
	Vector2 alignedPosition = CalculateAlignedPosition();
	// テキストの描画
	m_spriteFont->DrawString(
		m_spriteBatch.get(),
		m_textInfo.text.c_str(),
		alignedPosition,
		m_textInfo.color,
		0.0f,
		Vector2::Zero,
		m_textInfo.scale
	);
	// スプライトバッチの終了
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
