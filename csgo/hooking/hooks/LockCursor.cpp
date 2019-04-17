#include "../../inc.h"

void __stdcall hook::LockCursor( ) {
	if( g_renderer.get_instance( ) ) {
		if( g_renderer.get_instance( )->IsEnabled( ) ) {
			g_csgo.m_surface->unlock_cursor( );
			return;
		}
	}
	g_hooks.m_surface.get_old_method< fn::LockCursor_t >( hook::idx::LOCK_CURSOR )( g_csgo.m_surface );
}