#include "../../inc.hpp"
#include "../../features/misc/misc.h"

void __fastcall hook::OverrideView( uintptr_t ecx, uintptr_t edx, CViewSetup *pSetup ) {

	if( pSetup ) {
		g_misc.thirdperson( pSetup );

		if( g_csgo.m_engine->IsInGame( ) ){
			auto local = C_CSPlayer::get_local( );
			if( local && local->alive( ) && !local->is_scoped( ) ){
				pSetup->m_fov = g_vars.visuals.effects.camera_fov;
			}
			else if (local && local->alive() && g_vars.visuals.misc.remove_scope)
				pSetup->m_fov = g_vars.visuals.effects.camera_fov;
		}
	}

	g_misc.thirdperson( );

	g_hooks.m_clientmode.get_old_method< fn::OverrideView_t >( 18 )( ecx, pSetup );
}
