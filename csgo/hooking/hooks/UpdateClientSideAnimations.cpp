#include "../../inc.hpp"
#include "../../features/animations/anim.h"

void __fastcall animations::UpdateClientSideAnimations( uintptr_t ecx, uintptr_t edx ) {
	auto player = reinterpret_cast<C_CSPlayer *>( ecx );
	auto index = player->GetIndex( );
	g_anim.m_track[ index ].m_vmt->get_old_method< fn::UpdateClientSideAnimations_t >( 219 )( ecx );
}