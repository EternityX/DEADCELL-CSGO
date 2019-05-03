#include "../../inc.h"
#include "../../features/visuals/visuals.h"

void __fastcall hook::PaintTraverse( uintptr_t ecx, uintptr_t edx, int vguiPanel, bool forceRepaint, bool allowForce ) {
	static int hud_zoom_panel = 0;
	if( !hud_zoom_panel ) {
		if( util::hash::fnv1a_32( g_csgo.m_panel->get_name( vguiPanel ) ) == CT_HASH32( "HudZoom" ) )
			hud_zoom_panel = vguiPanel;
	}
	else {
		if( g_vars.visuals.misc.remove_scope && g_vars.visuals.misc.remove_scope == 1 ) {
			if( vguiPanel == hud_zoom_panel )
				return;
		}
	}

	g_hooks.m_panel.get_old_method< fn::PaintTraverse_t >( idx::PAINT_TRAVERSE )( ecx, vguiPanel, forceRepaint, allowForce );

	static int mat_system_panel = 0;
	if( !mat_system_panel ) {
		if( util::hash::fnv1a_32( g_csgo.m_panel->get_name( vguiPanel ) ) == CT_HASH32( "MatSystemTopPanel" ) )
			mat_system_panel = vguiPanel;
	}

	if( vguiPanel != mat_system_panel )
		return;

	g_csgo.m_surface->draw_set_color( Color( 0, 0, 0, 255 ) );
	g_csgo.m_surface->draw_filled_rect( 1, 1, 1, 1 );
}
