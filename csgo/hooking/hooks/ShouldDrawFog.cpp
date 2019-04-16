#include "../../inc.h"

bool __fastcall hook::ShouldDrawFog( uintptr_t ecx, uintptr_t edx ) {
	return !g_vars.visuals.misc.fog;
}