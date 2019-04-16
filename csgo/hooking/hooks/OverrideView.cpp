#include "../../inc.h"
#include "../../features/misc/misc.h"

void __fastcall hook::OverrideView( uintptr_t ecx, uintptr_t edx, c_view_setup *pSetup ) {

	if( pSetup ) {
		g_misc.thirdperson( pSetup );

		if( g_csgo.m_engine->is_in_game( ) ){
			auto local = c_csplayer::get_local( );
			if( local && local->alive( ) && ( !local->is_scoped( ) || g_vars.visuals.misc.remove_scope_zoom ) ) {
				pSetup->m_fov = g_vars.visuals.effects.camera_fov;
			}
		}
	}

	c_misc::thirdperson( );

	g_hooks.m_clientmode.get_old_method< fn::OverrideView_t >( hook::idx::OVERRIDE_VIEW )( ecx, pSetup );
}