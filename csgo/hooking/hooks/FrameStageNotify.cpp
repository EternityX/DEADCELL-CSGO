#include "../../inc.hpp"
#include "../../features/misc/misc.h"
#include "../../features/anti-aim/antiaim.h"
#include "../../features/animations/anim.h"
#include "../../features/resolver/resolver.h"
#include "../../features/visuals/visuals.h"

void __fastcall hook::FrameStageNotify( uintptr_t ecx, uintptr_t edx, ClientFrameStage_t curstage ) {
	g_misc.no_smoke( curstage );

	if( g_cl.m_should_update_materials )
		g_misc.transparent_props( );

	const auto in_thirdperson = g_csgo.m_input->m_camera_in_thirdperson;
	if( in_thirdperson && g_vars.antiaim.enabled && curstage == FRAME_RENDER_START )
		g_csgo.m_prediction->set_local_viewangles( g_antiaim.m_real );

	if( curstage == FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		g_resolver.frame_stage_notify( );

	g_hooks.m_client.get_old_method< fn::FrameStageNotify_t >( 37 )( ecx, curstage );

	if( curstage == FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
		auto local = c_csplayer::get_local( );
		if( local )
			local->flash_alpha( ) = g_vars.visuals.misc.no_flash ? 0.f : 255.f;
	}
}