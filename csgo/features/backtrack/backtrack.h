#pragma once
#include "../../inc.hpp"

struct lag_record_t {
	lag_record_t( ) : m_prev_record{ } {
		m_tickcount = 0;
		m_curtime = 0;
	}

	lag_record_t( c_csplayer *player, std::deque< lag_record_t > &records )
		: m_prev_record{} {

		if ( records.size( ) ) {
			m_prev_record = &records.front( );
		}
		else {
			m_prev_record = nullptr;
		}

		m_tickcount = g_csgo.m_global_vars->m_tickcount;
		m_curtime = g_csgo.m_global_vars->m_cur_time;

		// save all data here.
		m_simtime = player->simtime( );
		m_origin = player->origin( );
		m_abs_origin = player->abs_origin( );
		m_angles = player->angles( );
		m_lby = player->lby_t( );
		m_flags = player->flags( );
		m_vel = player->velocity( );

		m_animstate = player->animstate( );
		m_poses = player->poses( );
		m_animoverlays = player->animoverlays( );

		m_mins = player->get_collideable( )->mins( );
		m_maxs = player->get_collideable( )->maxs( );
		//player->setup_bones( m_bones.data( ), 128, 0x100, g_csgo.m_global_vars->m_cur_time );
	}

	bool is_valid( ) const;

	int								  m_tickcount;
	int								  m_curtime;

	int								  m_flags;
	vec3_t							  m_origin;
	vec3_t							  m_abs_origin;
	vec3_t							  m_angles;
	vec3_t							  m_abs_angles;
	float							  m_lby;
	float							  m_simtime;
	vec3_t							  m_vel;

	c_animstate*					  m_animstate;
	c_utl_vector< float >			  m_poses;
	c_utl_vector< animation_layer_t > m_animoverlays;

	vec3_t							  m_mins;
	vec3_t							  m_maxs;

	lag_record_t*					  m_prev_record;

	//std::array< matrix3x4_t, 128 >	  m_bones;
};

struct player_log_t {
	c_csplayer *m_player;
	std::deque< lag_record_t > m_records;
};

class c_backtrack{
	size_t m_max_records{ 12 };
public:
	std::array< player_log_t, 64 > m_players;
	void log( );
	bool restore( c_csplayer *e, lag_record_t &record );
	void process_cmd( c_user_cmd* cmd, c_csplayer* e, lag_record_t &record );
	player_log_t get( int index );
};

extern c_backtrack g_backtrack;