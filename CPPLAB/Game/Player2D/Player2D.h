#pragma once
#ifndef Player2D_h
#define Player2D_h
#include <KumachiLib\BillboardSprite\BillboardSprite.h>
#include <Game\MyResources\MyResources.h>
class Player2D
{
public:
	Player2D();
	~Player2D() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

private:
	std::unique_ptr<BillboardSprite> m_pBillboardSprite;// ビルボードスプライト

};
#endif // !Player2D_h
