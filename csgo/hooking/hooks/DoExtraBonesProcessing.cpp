#include "../../inc.h"
#include "../../features/animations/anim.h"

void __fastcall animations::DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, c_studio_hdr *hdr, vec3_t *pos, quaternion *q, matrix3x4_t *matrix, c_bone_bit_list &bone_list, c_ik_context *context ) {
	auto e = reinterpret_cast< c_csplayer * >( ecx );

	auto original = g_anim.m_track[ e->get_index( ) ].m_vmt->get_old_method< fn::DoExtraBonesProcessing_t >( hook::idx::DO_EXTRA_BONE_PROC );

	const auto state = e->animstate( );

	if( !state || !state->entity )
		return original( ecx, hdr, pos, q, matrix, bone_list, context );

	// prevent call to do_procedural_foot_plant
	const auto backup_onground = state->on_ground;
	state->on_ground = false;
	original( ecx, hdr, pos, q, matrix, bone_list, context );
	state->on_ground = backup_onground;
}
