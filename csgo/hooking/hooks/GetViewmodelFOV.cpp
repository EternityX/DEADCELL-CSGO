#include "../../inc.hpp"

float __fastcall hook::GetViewModelFOV( uintptr_t ecx, uintptr_t edx ){
	return g_vars.visuals.effects.weapon_fov;
}