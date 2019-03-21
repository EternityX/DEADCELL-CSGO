#include "../../inc.hpp"
#include "../../features/misc/misc.h"
#include "../../features/anti-aim/antiaim.h"
#include "../../features/animations/anim.h"
#include "../../features/resolver/resolver.h"
#include "../../features/visuals/visuals.h"

void __fastcall hook::FrameStageNotify( uintptr_t ecx, uintptr_t edx, ClientFrameStage_t curstage ) {

	g_misc.no_smoke( curstage );

	if( g_cl.m_should_update_materials ) {
		g_misc.nightmode( );
		g_misc.transparent_props( );
	}

	switch ( curstage ) {
		case FRAME_RENDER_START: {
			auto in_thirdperson = g_csgo.m_input->m_fCameraInThirdPerson;

			if ( in_thirdperson && g_vars.antiaim.enabled )
				g_csgo.m_prediction->SetLocalViewangles( g_antiaim.m_real );
		}
	}

	g_hooks.m_client.get_old_method< fn::FrameStageNotify_t >( hook::idx::FRAME_STAGE_NOTIFY )( ecx, curstage );

	switch ( curstage ) {
		case FRAME_RENDER_START: {
			for ( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
				auto e = g_csgo.m_entity_list->Get< C_CSPlayer >( i );

				if ( !e )
					continue;

				*( int* )( ( uintptr_t )e + 0xA30 ) = g_csgo.m_global_vars->m_frame_count;
				*( int* )( ( uintptr_t )e + 0xA28 ) = 0;
			}
		}
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START: {
			auto local = C_CSPlayer::get_local( );
			if ( local ) {
				if ( g_vars.visuals.misc.no_flash )
					local->flash_alpha( ) = 0.f;
				else
					local->flash_alpha( ) = 255.f;
			}
		}
	}
}
