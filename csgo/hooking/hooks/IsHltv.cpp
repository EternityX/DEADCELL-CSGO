#include "../../inc.h"


// credits to L3D451R7, thanks to him for showing me what this actually does
// Skip the AccumulateLayers call inside StandardBlendingRules

bool __fastcall hook::IsHltv( uintptr_t ecx, uintptr_t edx ){
	static const auto accumulate_layers_call = pattern::find< void * >( g_csgo.m_client_dll, "84 C0 75 0D F6 87" );
	static const auto setupvelocity_call = pattern::find< void * >( g_csgo.m_client_dll, "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" );

	auto ret = g_hooks.m_engine.get_old_method< fn::IsHltv_t >( 93 )( ecx );

	if( !g_csgo.m_engine->is_in_game( ) || !g_cl.m_local )
		return ret;

	if( _ReturnAddress( ) == accumulate_layers_call || _ReturnAddress( ) == setupvelocity_call )
		return true;

	return ret;
}