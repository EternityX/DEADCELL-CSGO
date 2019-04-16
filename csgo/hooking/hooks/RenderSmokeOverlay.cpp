#include "../../inc.h"

void __fastcall hook::RenderSmokeOverlay( uintptr_t ecx, uintptr_t edx, bool a1 ){
	if( !g_vars.visuals.misc.remove_smoke )
		g_hooks.m_viewrender.get_old_method< fn::RenderSmokeOverlay_t >( hook::idx::RENDER_SMOKE_OVERLAY )( ecx, a1 );
}