/*
*	@file		BGMTexture.cpp
*	@brief		「BGM」テクスチャクラス
*/
#include "pch.h"
#include "BGMTexture.h"
/*
*	@brief コンストラクタ
*	@details 「BGM」テクスチャクラスの初期化を行う
*	@param なし
*	@return なし
*/
BGMTexture::BGMTexture()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(DirectX::SimpleMath::Vector2(0.1f, 0.36f)) //位置
	, m_size(DirectX::SimpleMath::Vector2(0.17f, 0.1f)) // サイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
{
}
/*
*	@brief デストラクタ
*	@details 「BGM」テクスチャクラスのデストラクタ
*	@param なし
*	@return なし
*/
BGMTexture::~BGMTexture()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details 「BGM」テクスチャクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void BGMTexture::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// 画像を作成
	m_pImage = std::make_unique<Image>();
	// 頂点シェーダーのパスを渡す
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	// ピクセルシェーダーのパスを渡す
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// 画像を設定
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("BGMText"));
	// シェーダーバッファサイズを設定
	m_pImage->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
	// 矩形を設定
	// 座標を設定
	m_rect.position = m_position;
	// サイズを設定
	m_rect.size = m_size;
}
/*
*	@brief 更新
*	@details 「BGM」テクスチャクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void BGMTexture::Update(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 描画
*	@details 「BGM」テクスチャクラスの描画を行う
*	@param なし
*	@return なし
*/
void BGMTexture::Render()
{
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(m_rect, m_spriteSheetBuffer);
}
/*
*	@brief 定数バッファ更新
*	@details 定数バッファの更新を行う
*	@param なし
*	@return なし
*/
void BGMTexture::UpdateConstantBuffer()
{
	// SimpleMathの名前空間を使用
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
