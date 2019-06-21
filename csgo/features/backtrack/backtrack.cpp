#include "backtrack.h"
#include "../entity listener/ent_listener.h"

c_backtrack g_backtrack;

float get_lerp_time( ) {
	static auto cl_ud_rate = g_csgo.m_convar->find_var( "cl_updaterate" );
	static auto min_ud_rate = g_csgo.m_convar->find_var( "sv_minupdaterate" );
	static auto max_ud_rate = g_csgo.m_convar->find_var( "sv_maxupdaterate" );

	int ud_rate = 64;
	if ( cl_ud_rate )
		ud_rate = cl_ud_rate->get_int( );

	if ( min_ud_rate && max_ud_rate )
		ud_rate = max_ud_rate->get_int( );

	float ratio = 1.f;
	static auto cl_interp_ratio = g_csgo.m_convar->find_var( "cl_interp_ratio" );
	if ( cl_interp_ratio )
		ratio = cl_interp_ratio->get_float( );

	static auto cl_interp = g_csgo.m_convar->find_var( "cl_interp" );
	static auto c_min_ratio = g_csgo.m_convar->find_var( "sv_client_min_interp_ratio" );
	static auto c_max_ratio = g_csgo.m_convar->find_var( "sv_client_max_interp_ratio" );

	float lerp = g_csgo.m_global_vars->m_interval_per_tick;
	if ( cl_interp )
		lerp = cl_interp->get_float( );

	if ( c_min_ratio && c_max_ratio && c_min_ratio->get_float( ) != 1 )
		ratio = util::misc::clamp( ratio, c_min_ratio->get_float( ), c_max_ratio->get_float( ) );

	return math::max( lerp, ratio / ud_rate );
}

bool lag_record_t::is_valid( ) const {
	i_net_channel_info *channel_info = g_csgo.m_engine->get_net_channel_info();
	if( !channel_info )
		return false;

	float max_unlag = 0.2f;
	static auto sv_maxunlag = g_csgo.m_convar->find_var( "sv_maxunlag" );
	if ( sv_maxunlag )
		max_unlag = sv_maxunlag->get_float( );

	// use normal curtime here, we filter using predicted curtime in ragebot
	float curtime = g_csgo.m_global_vars->m_cur_time;

	// correct for latency.
	float correct = channel_info->get_average_latency( FLOW_OUTGOING ) + channel_info->get_average_latency( FLOW_INCOMING );
	math::clamp( correct, 0.f, max_unlag );

	// get difference between tick sent by player and the latency tick.
	return std::abs( correct - ( curtime - m_simtime ) ) <= 0.2f;
}

void c_backtrack::log( ){
	for ( auto &entry : g_listener.m_players ) {
		int idx = entry.m_idx;
		c_csplayer* e = entry.m_player;
		if( !e || !g_cl.m_local )
			continue;

		if( e == g_cl.m_local || e->team( ) == g_cl.m_local->team( ) || !e->is_valid_player( true, true ) )
			continue;

		// get player record entry.
		player_log_t* entry = &m_players.at( idx - 1 );

		entry->m_player = e;

		// we have no records or we received a player update from the server, make a new entry.
		if( entry->m_records.empty( ) || e->simtime( ) != entry->m_records.front( ).m_simtime ) {

			update_animation_data( e );

			auto lag_record = lag_record_t( e, entry->m_records );

			// only push valid records.
			if ( lag_record.is_valid( ) ) {
				entry->m_records.push_front( lag_record );
			}

			size_t size = entry->m_records.size( );

			// too many records...
			if( size > m_max_records )
				entry->m_records.resize( m_max_records );
		}
	}
}

void c_backtrack::reset( ) {

	for ( auto &player : m_players ) {
		player.m_records.clear( );
	}
}

bool c_backtrack::restore( c_csplayer *e, lag_record_t &record ) {
	if ( !e )
		return false;

	e->angles( ) = record.m_angles;
	e->origin( ) = record.m_origin;
	e->set_abs_origin( record.m_origin );
	e->flags( ) = record.m_flags;
	e->get_collideable( )->mins( ) = record.m_mins;
	e->get_collideable( )->maxs( ) = record.m_maxs;

	std::memcpy( e->bone_cache( ).base( ), record.m_matrix, record.m_bonecount * sizeof( matrix3x4_t )  );

	e->get_bone_count( ) = record.m_bonecount;

	return true;
}

void c_backtrack::update_animation_data( c_csplayer *e ){
	c_animstate *animstate = e->animstate( );
	if( !animstate )
		return;

	float backup_curtime = g_csgo.m_global_vars->m_cur_time;
	float backup_frametime = g_csgo.m_global_vars->m_frametime;

	g_csgo.m_global_vars->m_cur_time = e->simtime( );
	g_csgo.m_global_vars->m_frametime = g_csgo.m_global_vars->m_interval_per_tick;

	int backup_flags = e->flags( );
	int backup_eflags = e->eflags( );

	e->eflags( ) &= ~0x1000;

	e->abs_velocity( ) = e->velocity( );


	int last_update = animstate->last_client_side_animation_update_framecount;
	if ( last_update == g_csgo.m_global_vars->m_frame_count )
		animstate->last_client_side_animation_update_framecount = last_update - 1;

	e->client_side_anims( ) = true; {
		e->update_anims( );

	} e->client_side_anims( ) = false;

	e->flags( ) = backup_flags;
	e->eflags( ) = backup_eflags;

	g_csgo.m_global_vars->m_cur_time = backup_curtime;
	g_csgo.m_global_vars->m_frametime = backup_frametime;

	e->invalidate_bone_cache( );

	e->last_setupbones_frame( ) = 0;

	e->setup_bones( nullptr, -1, 0x7FF00, g_csgo.m_global_vars->m_cur_time );
}

void c_backtrack::process_cmd( c_user_cmd *cmd, c_csplayer* e, lag_record_t &record ) {
	if ( !record.is_valid( ) ) {
		cmd->m_tick_count = TIME_TO_TICKS( e->simtime( ) );
	}
	else {
		std::string name = e->get_info( ).m_player_name;
		std::transform( name.begin( ) , name.end( ), name.begin( ), ::tolower );

		g_notify.add( true, OSHColor::FromARGB( 220, 249, 44, 69 ), "fired shot at backtrack record ( player %s, %i ticks).",
			name.c_str( ), cmd->m_tick_count - TIME_TO_TICKS( record.m_simtime ) + 2 );

		cmd->m_tick_count = TIME_TO_TICKS( record.m_simtime );
	}
}

player_log_t *c_backtrack::get( int index ){
	try {
		return &m_players.at( index - 1 );
	}
	catch ( std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_WARN, "Failed to get player backtrack records", ex.what( ) );
		return nullptr;
	}
}
