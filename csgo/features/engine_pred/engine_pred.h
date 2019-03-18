#pragma once
#include "../../inc.hpp"

class c_engine_pred {
private:
	struct globals_t {
		float frametime;
		float curtime;
		int tickcount;
	} m_old_globals;

	struct player_t {
		int flags;
		vec3_t velocity;
	} m_old_player;

	void* m_movedata;

	int m_prediction_player;
	int m_prediction_seed;

	int post_think( C_BasePlayer *player ) const;
public:
	static void pre_start( );
	void start( CUserCmd * ucmd );
	void end( ) const;
};

extern c_engine_pred g_engine_pred;