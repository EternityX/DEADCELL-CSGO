#pragma once

#include "i_client_unknown.h"
#include "i_client_renderable.h"
#include "i_client_networkable.h"

class i_client_entity : public i_client_unknown, public i_client_renderable, public i_client_networkable {
public:
	virtual void release() = 0;
};
