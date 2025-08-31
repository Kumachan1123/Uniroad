/*
*	@file		SettingPanel.cpp
*	@brief		設定画面のパネルクラス
*/
#include "pch.h"
#include "SettingPanel.h"
/*
*	@brief コンストラクタ
*	@details 設定画面のパネルクラスのコンストラクタ
*	@param なし
*	@return なし
*/
SettingPanel::SettingPanel()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.45f)) // ロゴの位置
	, m_size(DirectX::SimpleMath::Vector2(1.0f, 0.3725f)) // ロゴのサイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
{
}
/*
*	@brief デストラクタ
*	@details 設定画面のパネルクラスのデストラクタ
*	@param なし
*	@return なし
*/
SettingPanel::~SettingPanel()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details 設定画面のパネルクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void SettingPanel::Initialize(CommonResources* resources, int width, int height)
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
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("SettingPanel"));
	// シェーダーバッファサイズを設定
	m_pImage->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
	// 矩形を設定
	// 位置を設定
	m_rect.position = m_position;
	// サイズを設定
	m_rect.size = m_size;
}
/*
*	@brief 更新
*	@details 設定画面のパネルクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void SettingPanel::Update(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 描画
*	@details 設定画面のパネルクラスの描画を行う
*	@param なし
*	@return なし
*/
void SettingPanel::Render()
{
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(m_rect, m_spriteSheetBuffer);
}
/*
*	@brief 定数バッファの更新
*	@details 定数バッファの更新を行う
*	@param なし
*	@return なし
*/
void SettingPanel::UpdateConstantBuffer()
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
