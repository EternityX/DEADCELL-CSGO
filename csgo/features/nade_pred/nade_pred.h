#pragma once
#include "../../inc.h"
#include "../visuals/visuals.h"

class c_nadepoint {
public:
	c_nadepoint( ) {
		m_valid = false;
	}

	c_nadepoint( vec3_t start, vec3_t end, bool plane, bool valid, vec3_t normal, bool detonate ) {
		m_start = start;
		m_end = end;
		m_plane = plane;
		m_valid = valid;
		m_normal = normal;
		m_detonate = detonate;
	}

	vec3_t m_start, m_end, m_normal;
	bool m_valid, m_plane, m_detonate;
};

enum nade_throw_act {
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

class c_nade_prediction {
	std::array< c_nadepoint, 500 >	_points { };
	bool		 _predicted = false;

	void predict( c_user_cmd *ucmd );
	bool detonated( c_base_combat_weapon*weapon, float time, trace_t &trace );
public:
	void trace( c_user_cmd *ucmd );
	void draw( );
};

extern c_nade_prediction g_nadepred;