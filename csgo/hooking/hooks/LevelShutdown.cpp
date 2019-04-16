#include "../../inc.h"
#include "../../features/animations/anim.h"

void __fastcall hook::LevelShutdown( uintptr_t ecx, uintptr_t edx ) {
	g_hooks.m_client.get_old_method< fn::LevelShutdown_t >( hook::idx::LEVEL_SHUTDOWN )( ecx );
	g_cl.m_local = nullptr;
}