#include "../../inc.h"

void __fastcall hook::DrawSetColor( i_surface* ecx, uintptr_t edx, int r, int g, int b, int a )
{
	static auto original = g_hooks.m_surface.get_old_method< fn::DrawSetColor_t >( hook::idx::DRAW_SET_COLOR );

	const auto return_address = uintptr_t( _ReturnAddress() );

	static auto return_to_scope_arc = pattern::find( g_csgo.m_client_dll, "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?" ) + 5;
	static auto return_to_scope_lens = pattern::find( g_csgo.m_client_dll, "FF 50 3C 8B 4C 24 20" ) + 3;

	if ( ( return_address == return_to_scope_arc
		 || return_address == return_to_scope_lens )
		 && g_vars.visuals.misc.remove_scope == 2 )
	{
		// We don't want this to draw, so we set the alpha to 0
		return original( ecx, r, g, b, 0 );
	}

	return original( ecx, r, g, b, a );
}