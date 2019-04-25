#pragma once
#include "../../../inc.h"

class chai_wrapper_misc
{
private:
public:
	bool key_pressed(int key) {
		return g_input.key_pressed(key);
	}
};
static chai_wrapper_misc return_misc_wrap() 
{
	chai_wrapper_misc new_wrap;
	return new_wrap;
}
static chai_wrapper_entlist return_entlist_wrap()
{
	chai_wrapper_entlist new_wrap;
	return new_wrap;
}
