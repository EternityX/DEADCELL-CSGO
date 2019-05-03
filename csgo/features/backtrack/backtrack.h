#pragma once
#include "../../inc.h"

struct lag_record_t {
	lag_record_t( ) : m_bonecount{ 0 }, m_simtime{ 0.f }, m_lby{ 0.f }{ }

	lag_record_t( c_csplayer* player, std::deque< lag_record_t > records ) : m_priority{ 0 } {

		if ( !records.empty( ) ) {
			m_prev_record = &records.front( );
		}
		else {
			m_prev_record = nullptr;
		}

		std::memcpy( m_matrix, player->bone_cache( ).base( ), player->get_bone_count( ) * sizeof( matrix3x4_t ) );

		m_bonecount = player->get_bone_count( );

		m_tickcount = g_csgo.m_global_vars->m_tickcount;

		// save all data here.
		m_simtime = player->simtime( );
		m_origin = player->origin( );
		m_angles = player->angles( );
		m_lby = player->lby_t( );
		m_flags = player->flags( );
		m_vel = player->velocity( );

		m_mins = player->get_collideable( )->mins( );
		m_maxs = player->get_collideable( )->maxs( );
	}

	bool is_valid( ) const;

	int m_priority;

	int m_tickcount;
	int m_flags;
	int m_bonecount;

	float m_lby;
	float m_simtime;

	vec3_t m_origin;
	vec3_t m_angles;
	vec3_t m_mins;
	vec3_t m_maxs;
	vec3_t m_vel;

	matrix3x4_t m_matrix[ 128 ];

	lag_record_t* m_prev_record;
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
	void reset( );
	bool restore( c_csplayer *e, lag_record_t &record );
	void update_animation_data( c_csplayer *e );
	void process_cmd( c_user_cmd* cmd, c_csplayer* e, lag_record_t &record );
	player_log_t *get( int index );
};

extern c_backtrack g_backtrack;