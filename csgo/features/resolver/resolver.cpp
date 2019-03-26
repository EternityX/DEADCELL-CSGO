#include "resolver.h"

c_resolver g_resolver;

void c_resolver::frame_stage_notify( ) {
	if( !g_vars.rage.enabled )
		return;

	auto local = C_CSPlayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
		auto e = g_csgo.m_entity_list->Get< C_CSPlayer >( i );

		if( !e || !e->alive( ) || e->IsDormant( ) || e->team( ) == local->team( ) || e == local || e->get_info( ).m_bIsFakePlayer )
			continue;

		resolve_player( e );
	}
}

void c_resolver::resolve_player( C_CSPlayer* e ) {
	auto& data = m_resolver_data[ e->GetIndex( ) ];

	if( data.m_resolve_mode != RESOLVE_MODE_NONE )
		e->angles( ).y = data.m_resolved_yaw;
}