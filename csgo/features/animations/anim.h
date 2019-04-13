#pragma once
#include "../../inc.hpp"

namespace animations {
	void __fastcall DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, c_studio_hdr *hdr, vec3_t *pos, quaternion *q, matrix3x4_t *matrix, c_bone_bit_list &bone_list, c_ik_context *context );
	namespace fn {
		using DoExtraBonesProcessing_t = void (__thiscall *)( uintptr_t, c_studio_hdr *, vec3_t *, quaternion *, matrix3x4_t *, c_bone_bit_list &, c_ik_context * );
	}
}

struct container_t {
	container_t( ) { }
	container_t( c_base_entity* ent ) {
		m_idx = ent->get_index( );
		m_vmt = std::make_unique< c_vmt >( ent );
		m_vmt->hook_method( hook::idx::DO_EXTRA_BONE_PROC, animations::DoExtraBonesProcessing );
		m_hooked = true;
	}

	std::unique_ptr< c_vmt > m_vmt;
	int m_idx;
	bool m_hooked = false;
};

class c_animations : public i_client_entity_listener {
public:
	void on_entity_created( c_base_entity *ent ) override;
	void on_entity_deleted( c_base_entity *ent ) override;

	void init( );
	void remove( );

	std::array< container_t, 64 > m_track;
};

extern c_animations g_anim;