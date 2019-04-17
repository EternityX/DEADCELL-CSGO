#include "../../inc.h"
#include "../../features/animations/anim.h"

// credits: good friend of mine.. (how to do it is already on uc though but code still can exist and it's just the pvs and nothing more)
bool __fastcall animations::SetupBones(uintptr_t ecx, uintptr_t edx, matrix3x4_t *bone_to_world_out, int max_bones, int bone_bask, float current_time) {
	auto e = reinterpret_cast <c_csplayer *>(ecx);

	static auto osb = g_anim.m_track[e->get_index()].m_renderable_vmt->get_old_method< fn::SetupBones_t >(hook::idx::SETUP_BONES);

	if (e && e->is_player()) {
		int restore_magic = 0;
		float restore_frametime = 0.f;

		int &last_framecount = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(e) + 0xA64);
		int &magic = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(e) + 0x64);

		restore_frametime = g_csgo.m_global_vars->m_frametime;
		restore_magic = magic;

		if (!(magic % 3)) {
			magic = 9;
		}

		if (g_csgo.m_global_vars->m_frametime > 0.0033333334) {
			g_csgo.m_global_vars->m_frametime = 0.003333333;
		}
		last_framecount = g_csgo.m_global_vars->m_frame_count;

		int &occlusion_flag = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(e) + 0xA24);
		int &prev_bone_mask = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(e) + 0x269C);

		static auto offs_bone_mask = g_netvars.get_offset("DT_BaseAnimating", "m_nForceBone") + 0x20;
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(e) + offs_bone_mask) = 0;
		prev_bone_mask = 0;

		occlusion_flag = 0xA;

		e->invalidate_bone_cache();

		bool result = osb(ecx, bone_to_world_out, max_bones, bone_bask, current_time);

		magic = restore_magic;
		g_csgo.m_global_vars->m_frametime = restore_frametime;

		return result;
	}

	return osb(ecx, bone_to_world_out, max_bones, bone_bask, current_time);
}