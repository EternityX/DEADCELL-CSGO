#include "../../inc.h"
#include "../../features/visuals/visuals.h"

bool __fastcall hook::DoPostScreenSpaceEffects( uintptr_t ecx, uintptr_t edx, const c_view_setup *pSetup ) {
	if( !g_cl.m_local )
		return g_hooks.m_clientmode.get_old_method< fn::DoPostScreenEffects_t >( hook::idx::DO_POST_SCREEN_SPACE_FX )( ecx, pSetup );

	if( g_vars.visuals.glow )
		g_visuals.handle_glow( );

	return g_hooks.m_clientmode.get_old_method< fn::DoPostScreenEffects_t >( hook::idx::DO_POST_SCREEN_SPACE_FX )( ecx, pSetup );
}