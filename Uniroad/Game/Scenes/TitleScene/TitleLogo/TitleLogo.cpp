/*
*	@file TitleLogo.cpp
*	@brief タイトルロゴクラス
*/
#include "pch.h"
#include "TitleLogo.h"

// 座標を定義
const DirectX::SimpleMath::Vector2 TitleLogo::POSITION(0.125f, 0.15f);
// サイズを定義
const DirectX::SimpleMath::Vector2 TitleLogo::SIZE(0.55f / 2.5, 0.35f / 2.5);
/*
*	@brief コンストラクタ
*	@details タイトルロゴクラスのコンストラクタ
*	@param なし
*	@return なし
*/
TitleLogo::TitleLogo()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // ロゴの位置
	, m_size(DirectX::SimpleMath::Vector2(0.55f, 0.35f)) // ロゴのサイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
{
}
/*
*	@brief デストラクタ
*	@details タイトルロゴクラスのデストラクタ
*	@param なし
*	@return なし
*/
TitleLogo::~TitleLogo()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details タイトルロゴクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void TitleLogo::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// アニメーションクラスを作成
	m_pAnimation = std::make_unique<Animation>();
	// 画像を作成
	m_pImage = std::make_unique<Image>();
	// シェーダーパスを渡す
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// 画像を設定
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Title"));
	// シェーダーバッファサイズを設定
	m_pImage->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
	// 矩形を設定
	m_logoRect.position = POSITION;
	m_logoRect.size = SIZE;
	// アニメーションシーケンスを作成
	CreateAnimationSequence();
}
/*
*	@brief 更新
*	@details タイトルロゴクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleLogo::Update(float elapsedTime)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// アニメーションを更新
	m_pAnimation->Update(elapsedTime);
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 画像を表示
*	@details タイトルロゴの画像を表示する
*	@param なし
*	@return なし
*/
void TitleLogo::Render()
{
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(m_logoRect, m_spriteSheetBuffer);
}
/*
*	@brief アニメーションシーケンスを作成
*	@details タイトルロゴのアニメーションシーケンスを作成する
*	@param なし
*	@return なし
*/
void TitleLogo::CreateAnimationSequence()
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;

	// フェーズ0: 拡大
	m_pAnimation->CreateAnimationSequence({
			0.5f,// 拡大にかける秒数
			[this](float t) {
			// 進行度を計算
			float easing = Easing::EaseOutBack(t);
			// 中央固定、サイズだけイージング補間
			m_logoRect.position = Vector2(0.5f, 0.5f);
			// サイズをイージング補間
			m_logoRect.size = Vector2::Lerp(Vector2(0.0f, 0.0f), Vector2(0.55f, 0.35f), easing);
	} });
	// フェーズ1: 待機
	m_pAnimation->CreateAnimationSequence({
			2.0f,// 待機時間
			[this](float) {
			// 0で動かした場所とサイズで固定
			m_logoRect.position = Vector2(0.5f, 0.5f);
			m_logoRect.size = Vector2(0.55f, 0.35f);
	} });
	// フェーズ2: 移動縮小
	m_pAnimation->CreateAnimationSequence({
			0.5f, // 移動・縮小にかける秒数
			[this](float t) {
			// 進行度を計算
			float easing = Easing::EaseInOutCubic(t);
			// 左上に移動しつつ縮小
			m_logoRect.position = Vector2::Lerp(Vector2(0.5f, 0.5f), POSITION, easing);
			// サイズをイージング補間
			m_logoRect.size = Vector2::Lerp(Vector2(0.55f, 0.35f), SIZE, easing);
	} });
	// 最終静止フェーズ用の番兵
	m_pAnimation->CreateAnimationSequence({
			0.0f, // 無限
			[this](float) {
			// 最終静止位置とサイズに設定
			m_logoRect.position = POSITION;
			m_logoRect.size = SIZE;
	} });

}
/*
*	@brief 定数バッファを更新
*	@details タイトルロゴの定数バッファを更新する
*	@param なし
*	@return なし
*/
void TitleLogo::UpdateConstantBuffer()
{
	// 名前空間を使用
	using namespace DirectX::SimpleMath;
	// 定数バッファを更新
	// ワールド行列を単位行列に設定
	m_spriteSheetBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_spriteSheetBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_spriteSheetBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_spriteSheetBuffer.count = Vector4(0.0f);
	// 高さを設定
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// 幅を設定
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}
