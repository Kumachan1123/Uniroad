/*
*	@file Fade.cpp
*	@brief フェードクラスの実装ファイル
*/
#include "pch.h"
#include "Fade.h"

// フェード最小値
const float Fade::FADE_MIN = -1.01f;
// フェード最大値
const float Fade::FADE_MAX = 1.01f;
// フェード速度
const float Fade::FADE_SPEED = 1.5f;
// フェード初期値
const float Fade::FADE_INIT = -0.75f;
// フェードの滑らかさ
const float Fade::FADE_SMOOTHNESS = 0.01f;
// 座標を定義
const DirectX::SimpleMath::Vector2 Fade::POSITION(1.0f, 1.0f);
// サイズを定義
const DirectX::SimpleMath::Vector2 Fade::SIZE(2.0f, 2.0f);

/*
*	@brief コンストラクタ
*	@details フェードクラスのコンストラクタ
*	@param なし
*	@return なし
*/
Fade::Fade()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(POSITION) // フェードの位置
	, m_size(SIZE) // フェードのサイズ
	, m_fadeTime{ FADE_INIT }// フェード時間
	, m_fadeState{ FadeState::None }// フェード状態
{
}
/*
*	@brief デストラクタ
*	@details フェードクラスのデストラクタ
*	@param なし
*	@return なし
*/
Fade::~Fade()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details フェードクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Fade::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// 画像を作成
	m_pImage = std::make_unique<Image>();
	// シェーダーパスを渡す
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Fade/VS_Fade.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Fade/PS_Fade.cso");
	// 画像を設定
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Fade"));
	// シェーダーバッファサイズを設定
	m_pImage->SetShaderBufferSize(sizeof(FadeBuffer));
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
	// 矩形を設定
	m_rect.position = POSITION;
	m_rect.size = SIZE;
}
/*
*	@brief 更新
*	@details フェードクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Fade::Update(float elapsedTime)
{
	// フェードイン
	FadeIn(elapsedTime);
	// フェードアウト
	FadeOut(elapsedTime);
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 画像を表示
*	@details フェードクラスの画像を表示する
*	@param なし
*	@return なし
*/
void Fade::Render()
{
	// フェード描画
	m_pImage->DrawQuadWithBuffer(m_rect, m_fadeBuffer);
}
/*
*	@brief 定数バッファを更新
*	@details フェードクラスの定数バッファを更新する
*	@param なし
*	@return なし
*/
void Fade::UpdateConstantBuffer()
{
	// フェードの滑らかさを設定
	m_fadeBuffer.smoothness = FADE_SMOOTHNESS;
	// フェードの進行度を設定
	m_fadeBuffer.fadeAmount = m_fadeTime;
}
/*
*	@brief フェードアウト
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::FadeOut(float elapsedTime)
{
	// フェードアウト中なら
	if (m_fadeState == FadeState::FadeOut)
	{
		// 時間を計算
		m_fadeTime -= elapsedTime * FADE_SPEED;
		// フェードを制限
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// フェードが最小値になったらフェードアウト終了
		if (m_fadeTime <= FADE_MIN)
			m_fadeState = FadeState::FadeOutEnd;
	}
}
/*
*	@brief フェードイン
*	@param elapsedTime	経過時間
*	@return なし
*/
void Fade::FadeIn(float elapsedTime)
{
	// フェードイン中なら
	if (m_fadeState == FadeState::FadeIn)
	{
		// 時間を計算
		m_fadeTime += elapsedTime * FADE_SPEED;
		// フェードを制限
		m_fadeTime = Clamp(m_fadeTime, FADE_MIN, FADE_MAX);
		// フェードが最大値になったらフェードイン終了
		if (m_fadeTime >= FADE_MAX)
			m_fadeState = FadeState::FadeInEnd;
	}
}
