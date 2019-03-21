#include "../../inc.hpp"
#include "../../features/visuals/visuals.h"

bool __fastcall hook::DoPostScreenSpaceEffects( uintptr_t ecx, uintptr_t edx, const CViewSetup *pSetup ) {
	if( !g_cl.m_local )
		return g_hooks.m_clientmode.get_old_method< fn::DoPostScreenEffects_t >( 44 )( ecx, pSetup );

	if( g_vars.visuals.glow )
		g_visuals.handle_glow( );

	return g_hooks.m_clientmode.get_old_method< fn::DoPostScreenEffects_t >( 44 )( ecx, pSetup );
}