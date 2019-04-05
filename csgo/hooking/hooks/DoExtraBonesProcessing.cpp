#include "../../inc.hpp"
#include "../../features/animations/anim.h"

void __fastcall animations::DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, c_studio_hdr *hdr, vec3_t *pos, quaternion *q, matrix3x4_t *matrix, c_bone_bit_list &bone_list, c_ik_context *context ) {
	auto e = reinterpret_cast< c_csplayer * >( ecx );

	bool backup = false;
	const auto animstate = e->animstate( );

	if( animstate ) {
		backup = animstate->on_ground;
		animstate->on_ground = false;
	}

	g_anim.m_track[ e->get_index( ) ].m_vmt->get_old_method< fn::DoExtraBonesProcessing_t >( hook::idx::DO_EXTRA_BONE_PROC )( ecx, hdr, pos, q, matrix, bone_list, context );

	if( animstate )
		animstate->on_ground = backup;
}
