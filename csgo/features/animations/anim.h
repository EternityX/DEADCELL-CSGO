#pragma once
#include "../../inc.hpp"

namespace animations {
	void __fastcall UpdateClientSideAnimations( uintptr_t ecx, uintptr_t edx );
	void __fastcall DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, c_studio_hdr *hdr, vec3_t *pos, Quaternion *q, matrix3x4_t *matrix, c_bone_bit_list &bone_list, c_ik_context *context );
	bool __fastcall TestHitboxes( uintptr_t ecx, uintptr_t edx, const ray_t &ray, unsigned int fContentsMask, trace_t& tr );
	namespace fn {
		using DoExtraBonesProcessing_t = void (__thiscall *)( uintptr_t, c_studio_hdr *, vec3_t *, Quaternion *, matrix3x4_t *, c_bone_bit_list &, c_ik_context * );
		using UpdateClientSideAnimations_t = void (__thiscall *)( uintptr_t );
		using TestHitboxes_t = bool ( __thiscall*)( uintptr_t, const ray_t&, unsigned int, trace_t& );
	}
}

struct container_t {
	std::unique_ptr< c_vmt > m_vmt;
	int m_index;
	bool m_hooked;
};

class c_animations : public i_client_entity_listener {
public:
	void on_entity_created( c_base_entity *ent ) override;
	void on_entity_deleted( c_base_entity *ent ) override;

	bool init( );
	bool remove( );

	std::array< container_t, 64 > m_track;
};

extern c_animations g_anim;