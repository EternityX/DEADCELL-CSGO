#pragma once
#include "../../../inc.h"
#include "../visuals/visuals.h"
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
{;
	return chai_wrapper_entlist(g_csgo.m_entity_list);
}
static OSHGui::Drawing::Color return_color()
{
	OSHGui::Drawing::Color new_wrap;
	return new_wrap;
}
static vec3_t chai_world_to_screen(vec3_t point) {
	vec3_t screen;
	c_visuals::world_to_screen(point, screen);
	return screen;
}
static uint8_t as_uint8(int i)
{
	return i;
}