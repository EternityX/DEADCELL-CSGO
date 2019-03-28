#pragma once
#include "../../inc.hpp"
#include "../visuals/visuals.h"

enum nade_throw_act {
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

class c_grenade_prediction {
private:
	std::vector<vec3_t> m_path;
	std::vector<std::pair<vec3_t, QAngle>> m_others;

	int m_type = 0;
	int m_act = 0;

public:
	void	setup(vec3_t& vecSrc, vec3_t& vecThrow, vec3_t viewangles);
	void	simulate(CViewSetup* setup);

	int		step(vec3_t& vecSrc, vec3_t& vecThrow, int tick, float interval);
	bool	check_detonate(const vec3_t& vecThrow, const trace_t& tr, int tick, float interval);

	void	trace_hull(vec3_t& src, vec3_t&, trace_t& tr);
	void	add_gravity_move(vec3_t& move, vec3_t& vel, float frametime, bool onground);
	void	push_entity(vec3_t& src, const vec3_t& move, trace_t& tr);
	void	resolve_fly_collision_custom(trace_t& tr, vec3_t& vecVelocity, float interval);
	int		physics_clip_velocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce);

	void	run(CUserCmd* cmd);
	void	run(CViewSetup* setup);
	void	run();
};

extern c_grenade_prediction g_nadepred;