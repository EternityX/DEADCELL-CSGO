#pragma once
#include "../../inc.hpp"

struct lag_record_t
{
	lag_record_t( C_CSPlayer *player )
		: m_bones{ } {

		// save all data here.
		m_simtime = player->simtime( );
		m_origin = player->origin( );
		m_lby = player->lby_t( );
		m_flags = player->flags( );
		m_vel = player->velocity( );
		player->SetupBones( m_bones.data( ), 128, 0x100, g_csgo.m_global_vars->m_cur_time );
	}

	bool is_valid( ) const;

	int						m_flags;
	vec3_t					m_origin;
	float					m_lby;
	float					m_simtime;
	vec3_t					m_vel;

	CUtlVector< float >	m_poses;
	 
	std::array< matrix3x4_t, 128 >	m_bones;
};

struct player_log_t
{
	C_CSPlayer *m_player;
	std::deque< lag_record_t > m_records;
};

class c_player_log{
	size_t m_max_records{ 12 };
public:
	std::array< player_log_t, 64 > m_players;
	void log( );
	player_log_t get( int index );
};

extern c_player_log g_player_log;