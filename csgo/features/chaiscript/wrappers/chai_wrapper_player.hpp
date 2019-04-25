#pragma once
#pragma once
#include "../../../inc.h"

class chai_wrapper_player
{
private:
	c_csplayer* ent;
public:
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
};