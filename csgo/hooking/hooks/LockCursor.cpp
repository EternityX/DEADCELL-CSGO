#include "../../inc.h"

void __stdcall hook::LockCursor( ) {
	if( g_menu.is_active() ) {
		g_csgo.m_surface->unlock_cursor( );
		return;
	}
	
	g_hooks.m_surface.get_old_method< fn::LockCursor_t >( hook::idx::LOCK_CURSOR )( g_csgo.m_surface );
}