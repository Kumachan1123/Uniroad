#include "pch.h"
#include "Puyo.h"

Puyo::Puyo(PuyoColor color)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(POSITION) // ロゴの位置
	, m_size(SIZE) // ロゴのサイズ
	, m_color(color) // ぷよの色
	, m_frameRows(1) // 画像の行数
	, m_frameCols(5) // 画像の列数
{}

Puyo::~Puyo()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}

void Puyo::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// アニメーションクラスを作成
	// 画像を作成
	m_pImage = std::make_unique<Image>();
	// 頂点シェーダーのパスを渡す
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	// ピクセルシェーダーのパスを渡す
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// 画像を設定
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Puyo"));
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

void Puyo::Update(float elapsedTime)
{
	// 定数バッファを更新
	UpdateConstantBuffer();
}

void Puyo::Render()
{
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(m_rect, m_spriteSheetBuffer);

}

void Puyo::Finalize()
{}

void Puyo::UpdateConstantBuffer()
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
	m_spriteSheetBuffer.count = Vector4((float)(m_color));
	// 高さを設定
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// 幅を設定
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}
