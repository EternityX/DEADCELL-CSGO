#pragma once
#include "../../inc.hpp"
#include "../misc/misc.h"
#include "../autowall/autowall.h"

class c_antiaim {
	vec3_t m_input = vec3_t{};

	bool allow( CUserCmd *ucmd );
	void adjust_yaw( CUserCmd *ucmd );
	void adjust_pitch( CUserCmd *ucmd );
	static float at_target( );
public:
	void set_angles( CUserCmd *ucmd );

	vec3_t	m_real = vec3_t( );
	vec3_t	m_fake = vec3_t( );
};

extern c_antiaim g_antiaim;