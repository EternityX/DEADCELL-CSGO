#pragma once
#include "../../inc.hpp"
#include "../misc/misc.h"
#include "../autowall/autowall.h"

enum ManualDirection {
	NONE,
	BACKWARDS,
	LEFT,
	RIGHT,
};

class c_antiaim {
	void ensure_sanity( CUserCmd *ucmd );

	void modify_packets( CUserCmd *ucmd );

	float best_head_yaw( );

	vec3_t	_input { };
	bool	_allow { };

public:
	float calculate_max_desync_angle( C_CSPlayer *local, CCSGOPlayerAnimState *anim_state );
	void adjust_yaw( CUserCmd *ucmd );
	void adjust_pitch( CUserCmd *ucmd );
	void	set_angles( CUserCmd *ucmd );
	void manual( CUserCmd *ucmd );

	vec3_t	_real = vec3_t( );
	vec3_t	_fake = vec3_t( );
	int manual_direction;
};

extern c_antiaim g_antiaim;