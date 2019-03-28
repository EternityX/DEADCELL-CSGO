#include "../../inc.hpp"
#include "../../features/misc/misc.h"
#include "../../features/nade_pred/nade_pred.h"

void __fastcall hook::OverrideView( uintptr_t ecx, uintptr_t edx, CViewSetup *pSetup ) {

	if( pSetup ) {
		g_misc.thirdperson( pSetup );

		if( g_csgo.m_engine->IsInGame( ) ){
			auto local = C_CSPlayer::get_local( );
			if( local && local->alive( ) && !local->is_scoped( ) ){
				pSetup->m_fov = g_vars.visuals.effects.camera_fov;
			}
		}

		g_nadepred.run(pSetup);
	}

	g_misc.thirdperson( );

	g_hooks.m_clientmode.get_old_method< fn::OverrideView_t >( hook::idx::OVERRIDE_VIEW )( ecx, pSetup );
}