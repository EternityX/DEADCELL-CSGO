#include "../../inc.hpp"
#include "../../features/visuals/visuals.h"
#include "../../features/nade_pred/nade_pred.h"
#include "../../features/anti-aim/antiaim.h"

HRESULT __stdcall hook::Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion ) {
	static bool once{ false };
	if( !once ) {
		g_renderer.init( device );
		g_menu.init( );
		g_input.init( "Valve001", g_renderer.m_instance );
		once = true;
	}

	int screen_width, screen_height;

	if( g_csgo.m_engine )
		g_csgo.m_engine->GetScreenSize( screen_width, screen_height );

	g_renderer.start_drawing( device );

	try {
		g_visuals.run( );
	}
	catch(...) {
		console::print( "exception caught inside visuals" );
	}

	g_nadepred.draw( );

	g_notify.draw( );

	if( g_vars.visuals.extra.antiaim_direction && g_vars.antiaim.enabled ) {
		auto direction_color = OSHColor::FromARGB( g_vars.visuals.extra.antiaim_direction_color, 170 );
		switch( g_antiaim.manual_direction ) {
			case LEFT:
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_MARLETT_45PX ), direction_color, OSHColor::FromARGB( 125, 0, 0, 0 ), screen_width / 2 - 100, screen_height / 2 - 29, DROPSHADOW, "1" ); break;
			case RIGHT:
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_MARLETT_45PX ), direction_color, OSHColor::FromARGB( 125, 0, 0, 0 ), screen_width / 2 + 68, screen_height / 2 - 29, DROPSHADOW, "3" ); break;
			case BACKWARDS:
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_MARLETT_45PX ), direction_color, OSHColor::FromARGB( 125, 0, 0, 0 ), screen_width / 2 - 14, screen_height / 2 + 60, DROPSHADOW, "2" ); break;
			default: break;
		}
	}
		
	if( g_vars.visuals.extra.misc_flags ) {
		int flag_count = 0;

		if( g_antiaim.manual_direction && g_vars.antiaim.enabled )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_20PX ), OSHColor::FromARGB( 255, 133, 188, 22 ), OSHColor::FromARGB( 200, 0, 0, 0 ), 15, ( screen_height / 2 ) - flag_count++ * 21, DROPSHADOW, "MANUAL" );

		if( g_cl.m_under_server_tick_rate )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_20PX ), OSHColor::FromARGB( 230, 230, 43, 10 ), OSHColor::FromARGB( 200, 0, 0, 0 ), 15, ( screen_height / 2 ) - flag_count++ * 21, DROPSHADOW, "FPS" );
	}

	g_visuals.watermark( );

	g_renderer.end_drawing( device );

	return g_hooks.m_directx.get_old_method< fn::Present_t >( 17 )( device, pSourceRect, pDestRect, hDestWindowOverride,
	                                                           pDirtyRegion );
}
