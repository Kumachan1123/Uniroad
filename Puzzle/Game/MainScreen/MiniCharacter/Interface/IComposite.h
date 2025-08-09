#pragma once
#include "Game/MainScreen/MiniCharacter/Interface/IComponent.h"

class IComposite : public IComponent
{
public:
	// ���i��ǉ�����
	virtual void Attach(std::unique_ptr<IComponent> turretParts) = 0;
	// ���i���폜����
	virtual void Detach(std::unique_ptr<IComponent> turretPart) = 0;
};