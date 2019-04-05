#include "resolver.h"

c_resolver g_resolver;

void c_resolver::frame_stage_notify( ) {
	if( !g_vars.rage.enabled )
		return;

	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
		auto e = g_csgo.m_entity_list->get< c_csplayer >( i );

		if( !e || !e->alive( ) || e->is_dormant( ) || e->team( ) == local->team( ) || e == local || e->get_info( ).m_is_fake_player )
			continue;

		resolve_player( e );
	}
}

void c_resolver::resolve_player( c_csplayer* e ) {
	auto& data = m_resolver_data[ e->get_index( ) ];

	if( data.m_resolve_mode != RESOLVE_MODE_NONE )
		e->angles( ).y = data.m_resolved_yaw;
}