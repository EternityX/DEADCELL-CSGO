#include "nade_pred.h"

c_grenade_prediction g_nadepred;

void c_grenade_prediction::run(CUserCmd* cmd) {
	if (!g_vars.visuals.grenade_pred) { return; }

	bool attack = cmd->m_buttons & IN_ATTACK,
		 attack2 = cmd->m_buttons & IN_ATTACK2;

	if (g_cl.m_local && g_cl.m_local->health() > 0) {
		m_act = (attack && attack2) ? ACT_LOB :
				(attack2) ? ACT_DROP :
				(attack) ? ACT_THROW :
				ACT_NONE;
	}
}
void c_grenade_prediction::run(CViewSetup* setup) {
	if (!g_vars.visuals.grenade_pred) { return; }

	if (g_cl.m_local && g_cl.m_local->health() > 0) {
		auto weapon = g_cl.m_local->get_active_weapon();

		if (weapon && weapon->is_grenade() && m_act != ACT_NONE) {
			m_type = weapon->item_index();
			simulate(setup);
		}
		else {
			m_type = 0;
		}
	}
}
void c_grenade_prediction::run() {
	if (!g_vars.visuals.grenade_pred) { return; }
	if (g_cl.m_local && g_cl.m_local->health() > 0 && ((m_type) && m_path.size() > 1)) {
		vec3_t nade_start, nade_end, collision_end, prev = m_path[0];

		for (auto it = m_path.begin(), end = m_path.end(); it != end; ++it) {
			if (g_visuals.world_to_screen(prev, nade_start) && g_visuals.world_to_screen(*it, nade_end)) {
				g_renderer.line(OSHColor::FromRGB(0, 125, 255), (int)nade_start.x, (int)nade_start.y, (int)nade_end.x, (int)nade_end.y);
			} prev = *it;
		}

		for (auto it = m_others.begin(), end = m_others.end(); it != end; ++it) {
			if (g_visuals.world_to_screen(it->first, collision_end)) {
				g_renderer.rect(OSHColor::FromRGB(255, 255, 255), (int)collision_end.x - 2, (int)collision_end.y - 2, 5, 5);
			}
		}

		if (g_visuals.world_to_screen(prev, nade_end)) {
			g_renderer.rect(OSHColor::FromRGB(255, 0, 0), (int)nade_end.x - 2, (int)nade_end.y - 2, 5, 5);
			this->draw_3d_dotted_circle(prev, 100, 150);
		}
	}
}

void c_grenade_prediction::setup(vec3_t& vecSrc, vec3_t& vecThrow, vec3_t viewangles) {
	vec3_t ang_throw = viewangles;
	float pitch = ang_throw.x;

	if (pitch <= 90.0f) {
		if (pitch < -90.0f) {
			pitch += 360.0f;
		}
	} else {
		pitch -= 360.0f;
	}

	ang_throw.x = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;

	float velocity = 750.0f * 0.9f;
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[m_act];
	b = b * 0.7f;
	b = b + 0.3f;
	velocity *= b;

	vec3_t vForward, vRight, vUp;
	math::angle_to_vectors(ang_throw, &vForward, &vRight, &vUp);

	vecSrc = g_cl.m_local->eye_pos();
	float off = (power[m_act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	trace_t tr;
	vec3_t vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	trace_hull(vecSrc, vecDest, tr);

	vec3_t vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	vecThrow = g_cl.m_local->velocity();
	vecThrow *= 1.25f;
	vecThrow += vForward * velocity;
}

void c_grenade_prediction::simulate(CViewSetup* setup_2) {
	vec3_t vecSrc, vecThrow;
	vec3_t angles = g_cl.m_cmd->m_viewangles;
	setup(vecSrc, vecThrow, angles);

	float interval = g_csgo.m_global_vars->m_interval_per_tick;
	int logstep = (int)(0.05f / interval);
	int logtimer = 0;

	m_path.clear(); m_others.clear();
	for (unsigned int i = 0; i < m_path.max_size() - 1; ++i) {
		if (!logtimer) { m_path.push_back(vecSrc); }

		int s = step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) { break; }
		if ((s & 2) || logtimer >= logstep) { logtimer = 0; }
		else { ++logtimer; }
	} m_path.push_back(vecSrc);
}

int c_grenade_prediction::step(vec3_t& vecSrc, vec3_t& vecThrow, int tick, float interval)
{
	vec3_t move; add_gravity_move(move, vecThrow, interval, false);
	trace_t tr; push_entity(vecSrc, move, tr);

	int result = 0;
	if (check_detonate(vecThrow, tr, tick, interval)) {
		result |= 1;
	}

	if (tr.fraction != 1.0f) {
		result |= 2;
		resolve_fly_collision_custom(tr, vecThrow, interval);

		QAngle angles;
		math::vector_angle((tr.endpos - tr.startpos).Normalized(), angles);
		m_others.push_back(std::make_pair(tr.endpos, angles));
	}
	
	vecSrc = tr.endpos;

	return result;
}

bool c_grenade_prediction::check_detonate(const vec3_t& vecThrow, const trace_t& tr, int tick, float interval) {
	switch (m_type) {
		case WEAPON_SMOKEGRENADE:
		case WEAPON_DECOY:
			if (vecThrow.Length2D() < 0.1f) {
				int det_tick_mod = (int)(0.2f / interval);
				return !(tick % det_tick_mod);
			}
			return false;

		case WEAPON_MOLOTOV:
		case WEAPON_INCGRENADE:
			if (tr.fraction != 1.0f && tr.plane.m_normal.z > 0.7f) {
				return true;
			}
		case WEAPON_FLASHBANG:
		case WEAPON_HEGRENADE:
			return (float)tick * interval > 1.5f && !(tick % (int)(0.2f / interval));

		default:
			assert(false);
			return false;
	}
}

void c_grenade_prediction::trace_hull(vec3_t& src, vec3_t& end, trace_t& tr) {
	Ray_t ray; CTraceFilterWorldOnly filter;
	ray.init(src, end, vec3_t(-2.0f, -2.0f, -2.0f), vec3_t(2.0f, 2.0f, 2.0f));
	g_csgo.m_engine_trace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void c_grenade_prediction::add_gravity_move(vec3_t& move, vec3_t& vel, float frametime, bool onground) {
	vec3_t basevel(0.0f, 0.0f, 0.0f);
	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground) {
		move.z = (vel.z + basevel.z) * frametime;
	} else {
		float gravity = 800.0f * 0.4f;
		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;
		vel.z = newZ;
	}
}

void c_grenade_prediction::push_entity(vec3_t& src, const vec3_t& move, trace_t& tr) {
	vec3_t vecAbsEnd = src;
	vecAbsEnd += move;
	trace_hull(src, vecAbsEnd, tr);
}

void c_grenade_prediction::resolve_fly_collision_custom(trace_t& tr, vec3_t& vecVelocity, float interval) {
	float flSurfaceElasticity = 1.0, flGrenadeElasticity = 0.45f;
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) { flTotalElasticity = 0.9f; }
	if (flTotalElasticity < 0.0f) { flTotalElasticity = 0.0f; }

	vec3_t vecAbsVelocity;
	physics_clip_velocity(vecVelocity, tr.plane.m_normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f;

	if (flSpeedSqr < flMinSpeedSqr) {
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	if (tr.plane.m_normal.z > 0.7f) {
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval);
		push_entity(tr.endpos, vecAbsVelocity, tr);
	} else {
		vecVelocity = vecAbsVelocity;
	}
}

int c_grenade_prediction::physics_clip_velocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce) {
	static const float STOP_EPSILON = 0.1f;

	float change, angle;
	int   i, blocked;

	blocked = 0;
	angle = normal[2];

	if (angle > 0) { blocked |= 1; }
	if (!angle) { blocked |= 2; }

	for (i = 0; i < 3; i++) {
		change = normal[i] * in.Dot(normal) * overbounce;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON) {
			out[i] = 0;
		}
	} return blocked;
}