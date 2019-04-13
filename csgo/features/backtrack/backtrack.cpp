#include "backtrack.h"
#include "../entity listener/ent_listener.h"

c_backtrack g_backtrack;

float get_lerp_time( ) {
	int ud_rate = g_csgo.m_convar->find_var( "cl_updaterate" )->get_int( );

	cvar *min_ud_rate = g_csgo.m_convar->find_var( "sv_minupdaterate" );
	cvar *max_ud_rate = g_csgo.m_convar->find_var( "sv_maxupdaterate" );

	if ( min_ud_rate && max_ud_rate )
		ud_rate = max_ud_rate->get_int( );

	float ratio = g_csgo.m_convar->find_var( "cl_interp_ratio" )->get_float( );
	if ( !ratio )
		ratio = 1.f;

	float lerp = g_csgo.m_convar->find_var( "cl_interp" )->get_float( );
	cvar *c_min_ratio = g_csgo.m_convar->find_var( "sv_client_min_interp_ratio" );
	cvar *c_max_ratio = g_csgo.m_convar->find_var( "sv_client_max_interp_ratio" );

	if ( c_min_ratio && c_max_ratio && c_min_ratio->get_float( ) != 1 )
		ratio = util::misc::clamp( ratio, c_min_ratio->get_float( ), c_max_ratio->get_float( ) );

	return std::max( lerp, ratio / ud_rate );
}

bool lag_record_t::is_valid( ) const{
	i_net_channel_info *channel_info = g_csgo.m_engine->get_net_channel_info();
	if( !channel_info )
		return false;

	// predict cur time.
	float curtime = TICKS_TO_TIME( g_cl.m_local->tickbase( ) );

	// correct for latency and lerp time.
	float correct = channel_info->get_average_latency( FLOW_OUTGOING ) + channel_info->get_average_latency( FLOW_INCOMING ) + get_lerp_time( );
	math::clamp( correct, 0.f, 0.2f /*sv_maxunlag*/ );

	// get difference between tick sent by player and the latency tick.
	return std::abs( correct - ( curtime - m_simtime ) ) < 0.19f;
}

void c_backtrack::log( ){
	for ( auto &entry : g_listener.m_players ) {
		int idx = entry.m_idx;
		c_csplayer* e = entry.m_player;
		if( !e || !g_cl.m_local || e == g_cl.m_local || e->team( ) == g_cl.m_local->team( ) )
			continue;

		if( !e->is_valid_player( true, true ) )
			continue;

		// get player record entry.
		player_log_t* entry = &m_players.at( idx - 1 );

		entry->m_player = e;

		// we have no records or we received a player update from the server, make a new entry.
		if( entry->m_records.empty( ) || e->simtime( ) > entry->m_records.front( ).m_simtime ) {
			update_animation_data( e );
			auto lag_record = lag_record_t( e );

			// only push valid records.
			if ( lag_record.is_valid( ) ) {
				entry->m_records.push_front( lag_record );
			}

			size_t size = entry->m_records.size( );

			// too many records...
			if ( size > m_max_records ) {
				for ( size_t s = 0; s < size - m_max_records; s++ )
					entry->m_records.pop_back( );
			}
		}
	}
}

void c_backtrack::reset( ) {
	for ( auto player :  m_players ) {
		player.m_records.clear( );
	}
}

bool c_backtrack::restore( c_csplayer *e, lag_record_t &record ) {
	if ( !e || !e->alive( ) )
		return false;

	if ( !record.is_valid( ) )
		return false;

	e->angles( ) = record.m_angles;
	e->origin( ) = record.m_origin;
	e->abs_origin( ) = record.m_abs_origin;
	e->velocity( ) = record.m_vel;
	e->flags( ) = record.m_flags;
	e->get_collideable( )->mins( ) = record.m_mins;
	e->get_collideable( )->maxs( ) = record.m_maxs;

	std::memcpy( e->bone_cache( ).base( ), &record.m_matrix, sizeof( matrix3x4_t ) * e->bone_cache( ).count( ) );

	// TO-DO : restore bonecount

	return true;
}

void c_backtrack::update_animation_data( c_csplayer *e ){
	e->client_side_anims( ) = true; {
		e->update_anims( );

		e->invalidate_bone_cache( );

		int backup_flags = e->flags( );
		e->flags( ) &= ~FL_ONGROUND;
		e->setup_bones( nullptr, -1, 0x7FF00, g_csgo.m_global_vars->m_cur_time );
		e->flags( ) = backup_flags;

	} e->client_side_anims( ) = false;
}

void c_backtrack::process_cmd( c_user_cmd *cmd, c_csplayer* e, lag_record_t &record ) {
	if ( !record.is_valid( ) ) {
		cmd->m_tick_count = TIME_TO_TICKS( e->simtime( ) + get_lerp_time( ) );
	}
	else {
		cmd->m_tick_count = TIME_TO_TICKS( record.m_simtime + get_lerp_time( ) );
	}
}

player_log_t c_backtrack::get( int index ){
	try {
		return m_players.at( index - 1 );
	}
	catch ( std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_WARN, "Failed to get player backtrack records", ex.what( ) );
		return player_log_t{};
	}
}