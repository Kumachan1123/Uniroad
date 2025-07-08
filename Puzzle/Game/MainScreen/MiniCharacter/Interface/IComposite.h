#pragma once
#include "Game/MainScreen/MiniCharacter/Interface/IComponent.h"

class IComposite : public IComponent
{
public:
	// •”•i‚ğ’Ç‰Á‚·‚é
	virtual void Attach(std::unique_ptr<IComponent> turretParts) = 0;
	// •”•i‚ğíœ‚·‚é
	virtual void Detach(std::unique_ptr<IComponent> turretPart) = 0;
};