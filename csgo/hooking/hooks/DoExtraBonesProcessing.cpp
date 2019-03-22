#include "../../inc.hpp"
#include "../../features/animations/anim.h"

void __fastcall animations::DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, CStudioHdr *hdr, vec3_t *pos, Quaternion *q, matrix3x4_t *matrix, CBoneBitList &bone_list, CIKContext *context ) {
	auto e = reinterpret_cast<C_CSPlayer *>( ecx );

	bool backup;
	auto animstate = e->animstate( );
	if( animstate ){
		backup = animstate->on_ground;
		animstate->on_ground = false;
	}
	g_anim.m_track[ e->GetIndex( ) ].m_vmt->get_old_method< fn::DoExtraBonesProcessing_t >( hook::idx::DO_EXTRA_BONE_PROC )( ecx, hdr, pos, q, matrix, bone_list, context );
	if( animstate ){
		animstate->on_ground = backup;
	}
}