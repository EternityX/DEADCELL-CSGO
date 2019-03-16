#include "../../inc.hpp"
#include "../../features/animations/anim.h"

void __fastcall hook::LevelShutdown( uintptr_t ecx, uintptr_t edx ) {
	g_hooks.m_client.get_old_method< fn::LevelShutdown_t >( 8 )( ecx );
	g_cl.m_local = nullptr;
}