#pragma once
#pragma once
#include "../../../inc.h"

class chai_wrapper_player
{
public:
	c_csplayer* ent;
	chai_wrapper_player(c_csplayer* ent_var)
	{
		ent = ent_var;
	}
	int get_health()
	{
		return ent->health();
	}
	int get_team()
	{
		return ent->team();
	}
	vec3_t get_origin()
	{
		return ent->origin();
	}
	bool is_alive()  
	{
		return ent->alive();
	}
	vec3_t get_eye_pos() {
		return ent->eye_pos();
	}
	bool is_valid(bool check_protected, bool check_dormant) {
		return ent->is_valid_player(check_protected, check_dormant);
	}
	std::string get_name() {
		return ent->get_info().m_player_name;
	}
	std::string get_id() {
		return ent->get_info().m_steam_id;
	}
	bool is_same_as(chai_wrapper_player player) {
		if (player.ent == ent) {
			return true;
		}
		return false;
	}
	bool player_check(bool team_check = true, bool local_check = true, bool dormant_check = false, bool protected_check = false) {
		bool ret = true;
		c_csplayer* local = g_cl.m_local;

		if (dormant_check && ent->is_dormant())
			ret = false;

		if (protected_check && ent->immune())
			ret = false;

		if (team_check && ent->team() == local->team())
			ret = false;

		if (local_check && ent == local)
			ret = false;

		return ret;
	}
	int get_index() {
		return ent->get_index();
	}
};