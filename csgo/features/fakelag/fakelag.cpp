#include "fakelag.h"

void c_fakelag::think( c_user_cmd* cmd ) {
	g_cl.m_sendpacket = true;

	static int choke = 0;
	auto local = c_csplayer::get_local( );
	if( !g_vars.misc.fakelag.enabled && g_vars.antiaim.enabled ){
		choke = 1;
	}

	if( g_vars.misc.fakelag.enabled ) {
		switch( g_vars.misc.fakelag.type ) {
			case 0:{ // Maximum
				choke = std::min< int >( g_vars.misc.fakelag.amount, 14 );
				break;
			}
			case 1: { // Adaptive
				choke = std::min< int >( static_cast< int >( std::ceilf( 64 / ( g_cl.m_local->velocity( ).length( ) * g_csgo.m_global_vars->m_interval_per_tick ) ) ), g_vars.misc.fakelag.amount );
				break;
			}
		}
	}

	if( choke > static_cast< int >( g_csgo.m_clientstate->m_choked_commands ) )
		g_cl.m_sendpacket = false;
}

c_fakelag g_fakelag;
