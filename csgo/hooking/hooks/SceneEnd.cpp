#include "../../inc.hpp"
#include "../../features/chams/chams.h"

void __fastcall hook::SceneEnd( uintptr_t ecx, uintptr_t edx ){
	g_hooks.m_renderview.get_old_method< fn::SceneEnd_t >( 9 )( ecx );

	g_chams.on_sceneend( );
}