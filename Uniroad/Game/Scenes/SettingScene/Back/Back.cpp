/*
*	@file		Back.cpp
*	@brief		背景テクスチャクラス
*/
#include "pch.h"
#include "Back.h"
/*
*	@brief コンストラクタ
*	@details 背景テクスチャクラスの初期化を行う
*	@param なし
*	@return なし
*/
Back::Back()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // 位置
	, m_size(DirectX::SimpleMath::Vector2(1.0f, .6f)) // サイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_time(0.0f) // 経過時間
{
}
/*
*	@brief デストラクタ
*	@details 背景テクスチャクラスのデストラクタ
*	@param なし
*	@return なし
*/
Back::~Back()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}

/*
*	@brief 初期化
*	@details 背景テクスチャクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Back::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// 画像を作成
	m_pImage = std::make_unique<Image>();
	// シェーダーパスを渡す
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/UVScroll/VS_UVScroll.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/UVScroll/PS_UVScroll.cso");
	// 画像を設定
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Sky"));
	// シェーダーバッファサイズを設定
	m_pImage->SetShaderBufferSize(sizeof(UVScrollBuffer));
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
	// 矩形を設定
	m_rect.position = m_position;
	m_rect.size = m_size;
}
/*
*	@brief 更新
*	@details 背景テクスチャクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Back::Update(float elapsedTime)
{
	// 時間を進める
	m_time += elapsedTime;
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 描画
*	@details 背景テクスチャクラスの描画を行う
*	@param なし
*	@return なし
*/
void Back::Render()
{
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(m_rect, m_uvScrollBuffer);
}
/*
*	@brief 定数バッファを更新
*	@details 背景テクスチャクラスの定数バッファを更新する
*	@param なし
*	@return なし
*/
void Back::UpdateConstantBuffer()
{
	// 名前空間を使用
	using namespace DirectX::SimpleMath;
	// 定数バッファを更新
	// ワールド行列を単位行列に設定
	m_uvScrollBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_uvScrollBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_uvScrollBuffer.matProj = Matrix::Identity;
	// 時間を設定
	m_uvScrollBuffer.time = Vector4(m_time);
}
