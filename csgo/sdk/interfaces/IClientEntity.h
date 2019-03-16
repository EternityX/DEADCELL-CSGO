#pragma once

#include "IClientUnknown.h"
#include "IClientRenderable.h"
#include "IClientNetworkable.h"

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable {
public:
	virtual void Release() = 0;
};
