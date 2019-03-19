#include "../../inc.hpp"
#include "../../features/chams/chams.h"
#include "../../features/visuals/visuals.h"

void __fastcall hook::SceneEnd( uintptr_t ecx, uintptr_t edx ){
	g_hooks.m_renderview.get_old_method< fn::SceneEnd_t >( 9 )( ecx );

	if( g_vars.visuals.glow )
		g_visuals.handle_glow( );

	g_chams.on_sceneend( );
}