#pragma once
#include "../../inc.hpp"
#include "../misc/misc.h"
#include "../autowall/autowall.h"

class c_antiaim {
	vec3_t m_input = vec3_t{};
	vec3_t m_stored_input = vec3_t{};

	bool m_desync_next_tick;

	bool allow( c_user_cmd *ucmd );
	void adjust_yaw( c_user_cmd *ucmd );
	void adjust_pitch( c_user_cmd *ucmd );
	static float at_target( );
public:
	void set_angles( c_user_cmd *ucmd );

	vec3_t	m_real = vec3_t( );
	vec3_t	m_fake = vec3_t( );
};

extern c_antiaim g_antiaim;