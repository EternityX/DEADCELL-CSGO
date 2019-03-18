#pragma once
#include "../../inc.hpp"

namespace animations {
	void __fastcall UpdateClientSideAnimations( uintptr_t ecx, uintptr_t edx );
	void __fastcall DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, CStudioHdr *hdr, vec3_t *pos, Quaternion *q, matrix3x4_t *matrix, CBoneBitList &bone_list, CIKContext *context );
	bool __fastcall TestHitboxes( uintptr_t ecx, uintptr_t edx, const Ray_t &ray, unsigned int fContentsMask, trace_t& tr );
	namespace fn {
		using DoExtraBonesProcessing_t = void (__thiscall *)( uintptr_t, CStudioHdr *, vec3_t *, Quaternion *, matrix3x4_t *, CBoneBitList &, CIKContext * );
		using UpdateClientSideAnimations_t = void (__thiscall *)( uintptr_t );
		using TestHitboxes_t = bool ( __thiscall*)( uintptr_t, const Ray_t&, unsigned int, trace_t& );
	}
}

struct container_t {
	std::unique_ptr< c_vmt > m_vmt;
	int m_index;
	bool m_hooked;
};

class c_animations : public IClientEntityListener {
public:
	void OnEntityCreated( C_BaseEntity *ent ) override;
	void OnEntityDeleted( C_BaseEntity *ent ) override;

	void init( );
	void remove( );

	std::array< container_t, 64 > m_track;
};

extern c_animations g_anim;