#pragma once
#include "../../inc.h"

struct autowall_info_t {
	autowall_info_t( const vec3_t start, const vec3_t end, c_csplayer *to, c_csplayer *from ){
		m_start = start;
		m_end = end;
		m_to = to;
		m_from = from;
	}

	vec3_t m_start;
	vec3_t m_end;
	c_csplayer* m_to;
	c_csplayer* m_from;
};

class c_autowall {
private:
	static void clip_trace( const vec3_t &src, vec3_t &end, trace_t *tr, c_base_entity *target );
	bool is_breakable( c_base_entity *entity ) const;
	bool trace_to_exit( vec3_t &end, const vec3_t &start, const vec3_t &dir, trace_t *enter_trace, trace_t *exit_trace ) const;
	bool handle_bullet_pen( surface_data_t *enter_surface, trace_t *enter_trace, const vec3_t &direction, vec3_t *origin, float penetration, int &penetration_count, float &current_damage, float min_dmg );

public:
	std::string m_enter_material_name;
	std::string m_exit_material_name;

	float m_dmg_mod{};
	float m_avg_pen{};
	float m_pen_len{};
	float m_lost_dmg{};
	float m_autowall_dmg{};

	static float hitgroup_dmg( int hitgroup );
	static void scale_damage( int hitgroup, c_csplayer *ent, float weapon_armor_ratio, float &current_damage );
	bool think( const vec3_t &position, c_csplayer *entity, int mindmg, bool run_bullet_pen );
};

extern c_autowall g_autowall;