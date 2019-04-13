#pragma once
#include "../../inc.hpp"
#include "../misc/misc.h"
#include "../autowall/autowall.h"

class c_antiaim {
	vec3_t m_input = vec3_t{};
	vec3_t m_stored_input = vec3_t{};

	c_animstate* m_server_animstate;

	bool m_desync_next_tick;

	float m_next_lby_update;
	bool m_break_lby;

	void update_animstate( c_animstate* state, vec3_t angle ); // hack, since gay mango doesnt let me do asm in lambdas
	void handle_animstate( );

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