#include "../../inc.hpp"
#include "../../features/animations/anim.h"

bool __fastcall animations::TestHitboxes( uintptr_t ecx, uintptr_t edx, const Ray_t &ray, unsigned int fContentsMask, trace_t &tr ) {
	const auto player = reinterpret_cast< C_CSPlayer * >( ecx );
	const auto index = player->GetIndex( );
	return g_anim.m_track[ index ].m_vmt->get_old_method< fn::TestHitboxes_t >( hook::idx::TEST_HITBOXES )( ecx, ray, fContentsMask, tr );
}
