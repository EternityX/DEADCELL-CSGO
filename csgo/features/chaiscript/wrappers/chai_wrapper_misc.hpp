#pragma once
#include "../../../inc.h"

class chai_wrapper_misc
{
private:
public:
	bool key_pressed(int key) {
		return g_input.key_pressed(key);
	}
	bool is_visible(vec3_t start, vec3_t end, chai_wrapper_player target) {
		ray_t ray;
		trace_t tr;
		ray.init(start, end);

		c_trace_filter_skip_entity filter(g_cl.m_local);

		g_csgo.m_engine_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

		return (tr.m_hit_entity == target.ent || tr.m_fraction > 0.97f);
	}
};
static chai_wrapper_misc return_misc_wrap() 
{
	chai_wrapper_misc new_wrap;
	return new_wrap;
}
static chai_wrapper_entlist return_entlist_wrap()
{
	chai_wrapper_entlist new_wrap;
	return new_wrap;
}
