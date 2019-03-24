#pragma once
#include "../../inc.hpp"
#include "../misc/misc.h"
#include "../autowall/autowall.h"

class c_antiaim {
	float best_head_yaw( );

public:
	void adjust_yaw( CUserCmd *ucmd );
	void adjust_pitch( CUserCmd *ucmd );
	void set_angles( CUserCmd *ucmd );

	vec3_t	m_real = vec3_t( );
	vec3_t	m_fake = vec3_t( );
};

extern c_antiaim g_antiaim;