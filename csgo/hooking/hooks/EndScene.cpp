#include "../../inc.h"
#include "../../features/chaiscript/chai_wrapper.hpp"
#include "../../features/visuals/visuals.h"
#include "../../features/nade_pred/nade_pred.h"

long __stdcall hook::EndScene( IDirect3DDevice9Ex *device ) {
	static auto wanted_ret_address = _ReturnAddress( );

	if( _ReturnAddress( ) == wanted_ret_address ) {
		static bool once { false };
		if( !once ) {
			g_renderer.init( device );
			g_menu.init( );
			g_input.init( "Valve001", g_renderer.get_instance( ) );
			once = true;
		}

		g_renderer.start_drawing( );

		g_visuals.run( );

		if (g_vars.misc.chaiscript_enabled) {
			for (auto & session : chai_manager::sessions) {
				if (session.chai_defined_endscene && session.chai_finished_init)
					session.chai_hook_endscene(&g_renderer);
			}
		}

		g_nadepred.draw( );

		g_notify.draw( );

		if ( g_vars.visuals.extra.misc_flags && g_csgo.m_engine->is_connected( ) ) {
			int flag_count = 0;

			if ( g_cl.m_under_tickrate ) {
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_ENHANCE_9PX ), OSHColor::FromARGB( 230, 230, 43, 10 ), OSHColor::FromARGB( 200, 0, 0, 0 ), 15, ( g_renderer.get_renderer( ).GetDisplaySize( ).Height / 2 ) - flag_count++ * 21, DROPSHADOW, "FPS" );
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_7PX ), OSHColor::FromARGB( 230, 230, 43, 10 ), OSHColor::FromARGB( 200, 0, 0, 0 ), 15, ( g_renderer.get_renderer( ).GetDisplaySize( ).Height / 2 ) - 1 * 11, DROPSHADOW, "%i", g_cl.m_client_framerate );
			}
		}

		g_visuals.watermark( );

		g_renderer.end_drawing( );
	}

	return g_hooks.m_directx.get_old_method< fn::EndScene_t >( hook::idx::END_SCENE )( device );
}
