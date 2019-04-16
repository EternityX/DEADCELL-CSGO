#include "../../inc.h"
#include "../../features/misc/misc.h"
#include "../../features/ragebot/ragebot.h"

void __fastcall hook::LevelInitPostEntity( uintptr_t ecx, uintptr_t edx ) {
	g_hooks.m_client.get_old_method< fn::LevelInitPostEntity_t >( hook::idx::LEVEL_INIT_POST_ENTITY )( ecx );
	
	g_cl.m_local = c_csplayer::get_local( );
	g_cl.m_should_update_materials = true;
	g_cl.m_server_update_rate = g_csgo.m_convar->find_var( "sv_maxupdaterate" )->get_int( );
	g_backtrack.reset( );
	g_ragebot.m_last_target = nullptr;
}
