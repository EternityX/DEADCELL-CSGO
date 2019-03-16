#include "../../inc.hpp"

void __fastcall hook::RenderSmokeOverlay( uintptr_t ecx, uintptr_t edx, bool a1 ){
	if( !g_vars.visuals.misc.remove_smoke ) {
		g_hooks.m_viewrender.get_old_method< fn::RenderSmokeOverlay_t >( 41 )( ecx, a1 );
	}
}