#pragma once

class IGameObject
{
public:
	virtual ~IGameObject() = default;
	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) = 0;
	virtual void Finalize() = 0;
};