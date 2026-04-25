#include "pch.h"
#include "Player2D.h"

Player2D::Player2D()
	: m_pBillboardSprite(nullptr)

{
	Initialize();
}

void Player2D::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto textureManager = MyResourecs::Get().GetTextureManager();
	// ビルボードスプライトを生成する。
	m_pBillboardSprite = std::make_unique<BillboardSprite>();
	m_pBillboardSprite->SetTexture(textureManager->GetTexture("Player"));
	m_pBillboardSprite->Initialize();
	// ビルボードスプライトの位置とスケール を設定
	m_pBillboardSprite->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	m_pBillboardSprite->SetScale(1.0f);
	// ビルボード機能の有効/無効を設定
	m_pBillboardSprite->SetBillboard(true);
}

void Player2D::Update(float elapsedTime)
{
	m_pBillboardSprite->Update(elapsedTime);
}

void Player2D::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_pBillboardSprite->Render(view, projection);
}
