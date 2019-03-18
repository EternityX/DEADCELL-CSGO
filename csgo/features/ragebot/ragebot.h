#pragma once
#include "../../inc.hpp"
#include "../autowall/autowall.h"
#include "../misc/misc.h"
#include "../player_log/player_log.h"

class c_ragebot {
	struct rage_t {
		rage_t( C_CSPlayer *player, int i, int damage, vec3_t point, float dist, std::array< matrix3x4_t, 128 > matrix ){
			m_player = player;
			index = i;
			m_damage = damage;
			m_bestpoint = point;
			distance = dist;
			mat = matrix;
		}
		vec3_t m_bestpoint;
		C_CSPlayer* m_player;
		int index;
		int m_damage;
		float distance;
		std::array< matrix3x4_t, 128 > mat;
	};
	std::vector< rage_t > m_players;
	void choose_angles( );
	void select_target( );
	bool get_points_from_hitbox( C_CSPlayer * e, std::vector< int > hitboxes, std::array< matrix3x4_t, 128 > matrix, std::vector<vec3_t>& points, float scale );
	void quickstop( C_BaseCombatWeapon *local_weapon );
	bool is_valid( C_CSPlayer * player );
	bool hitchance( vec3_t &angle, C_CSPlayer *ent );
	static std::vector< lag_record_t > get_best_records( std::deque< lag_record_t > records );
	CUserCmd * m_cmd;
public:
	int shots_fired;
	C_CSPlayer *m_last_target = nullptr;
	void work( CUserCmd * cmd );
};

extern c_ragebot g_ragebot;