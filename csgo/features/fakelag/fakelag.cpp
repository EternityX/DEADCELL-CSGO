#include "fakelag.h"

void c_fakelag::think( CUserCmd* cmd ) {
	g_cl.m_sendpacket = true;

	if ( !g_vars.misc.fakelag.enabled ) {
		m_choked = 0;

		if ( g_vars.antiaim.enabled ) {
			static auto should_send_packet = true;
			g_cl.m_sendpacket = should_send_packet;
			should_send_packet = !should_send_packet;
		}
		else {
			return;
		}
	}

	if ( !g_cl.m_local || !g_cl.m_local->alive( ) )
		return;
	
	switch ( g_vars.misc.fakelag.type ) {
		case MAXIMUM: {
			if ( m_choked < g_vars.misc.fakelag.amount ) {
				g_cl.m_sendpacket = false;
			}
			else {
				m_choked = 0;
			}
			break;
		}
		case ADAPTIVE: {
			int ideal_choke = int( ceil( 64.f / ( g_cl.m_local->velocity( ).Length( ) * g_csgo.m_global_vars->m_interval_per_tick ) ) );
			math::clamp< int >( ideal_choke, 1, 15 );
			if ( m_choked < ideal_choke ) {
				g_cl.m_sendpacket = false;
			}
			else {
				m_choked = 0;
			}
			break;
		}
	}

	if ( !g_cl.m_sendpacket )
		++m_choked;
}

c_fakelag g_fakelag;