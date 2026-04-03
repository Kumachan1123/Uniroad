#include "pch.h"
#include "Puyo.h"

// 指定色でぷよを構築する。
// ここでは主に論理状態の初期値を設定し、GPUリソースはInitializeで確保する。
Puyo::Puyo(PuyoColor color)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(POSITION) // 画面上の初期表示位置
	, m_size(SIZE) // 画面上の初期表示サイズ
	, m_color(color) // ぷよの色
	, m_frameRows(1) // スプライトシートの行数（現状1行）
	, m_frameCols(5) // スプライトシートの列数（色数に対応）
	, m_row(0) // 盤面上の行番号
	, m_col(0) // 盤面上の列番号
	, m_time(0.0f) // 落下タイマー
	, m_isFalling(false) // 自由落下フラグ
{}

Puyo::~Puyo()
{
	// 外部所有の参照を破棄。
	m_pCommonResources = nullptr;
}

void Puyo::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースを保存。
	m_pCommonResources = resources;

	// Imageを作り直して初期化する。
	// 色変更や再生成時にも安全に再初期化できるよう、毎回新しいインスタンスを使用する。
	m_pImage = std::make_unique<Image>();

	// 描画シェーダを設定。
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");

	// ぷよ用テクスチャを設定（スプライトシート）。
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("Puyo"));

	// シェーダへ送るバッファサイズを登録。
	m_pImage->SetShaderBufferSize(sizeof(SpriteSheetBuffer));

	// GPU側リソースを初期化。
	m_pImage->Initialize(m_pCommonResources, width, height);

	// 描画矩形を現在の位置・サイズで同期。
	m_rect.position = m_position;
	m_rect.size = m_size;
}

void Puyo::Update(float elapsedTime)
{
	// 毎フレーム、描画用の定数バッファを現在状態に更新する。
	UpdateConstantBuffer();

	// 簡易自由落下モード。
	// RehabiliScene側の盤面制御と独立して動くため、必要時のみ有効化される。
	if (m_isFalling)
	{
		m_time += elapsedTime;
		// 1秒ごとに1マス下へ落下。
		if (m_time >= 1.0f)
		{
			m_time = 0.0f;
			m_row++;
			m_position.y += SIZE.y * 2;
			m_rect.position = m_position;
		}

		// 盤面最下段に達したら落下終了。
		if (m_row >= 11)
		{
			m_isFalling = false;
		}
	}
}

void Puyo::Render()
{
	// Noneは空マスを表すため描画しない。
	if (m_color == Puyo::PuyoColor::None)return;

	// 現在の矩形情報と定数バッファで描画。
	m_pImage->DrawQuadWithBuffer(m_rect, m_spriteSheetBuffer);
}

void Puyo::Finalize()
{}

void Puyo::UpdateConstantBuffer()
{
	using namespace DirectX::SimpleMath;

	// 2D描画のため、行列は単位行列のまま使用。
	m_spriteSheetBuffer.matWorld = Matrix::Identity;
	m_spriteSheetBuffer.matView = Matrix::Identity;
	m_spriteSheetBuffer.matProj = Matrix::Identity;

	// 色番号をスプライトシートの列として渡す。
	m_spriteSheetBuffer.count = Vector4((float)(m_color));
	// シート分割情報（行・列）を渡す。
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}
