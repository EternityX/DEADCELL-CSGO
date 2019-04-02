#include "../../inc.hpp"
#include "../../features/animations/anim.h"

void __fastcall animations::UpdateClientSideAnimations( uintptr_t ecx, uintptr_t edx ) {
	const auto player = reinterpret_cast< C_CSPlayer * >( ecx );
	const auto index = player->GetIndex( );
	g_anim.m_track[ index ].m_vmt->get_old_method< fn::UpdateClientSideAnimations_t >( hook::idx::UPDATE_CLIENTSIDE_ANIM )( ecx );
}
