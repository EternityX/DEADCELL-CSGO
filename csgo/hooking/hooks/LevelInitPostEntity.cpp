#include "../../inc.hpp"
#include "../../features/misc/misc.h"
#include "../../features/ragebot/ragebot.h"

void __fastcall hook::LevelInitPostEntity( uintptr_t ecx, uintptr_t edx ) {
	g_hooks.m_client.get_old_method< fn::LevelInitPostEntity_t >( 7 )( ecx );
	
	g_cl.m_local  = C_CSPlayer::get_local( );
	g_cl.m_should_update_materials = true;
	g_cl.m_server_update_rate = g_csgo.m_convar->FindVar( "sv_maxupdaterate" )->GetInt( );
	g_ragebot.m_last_target = nullptr;
}
