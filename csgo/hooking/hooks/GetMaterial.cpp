#include "../../inc.h"

i_material * __fastcall hook::GetMaterial( uintptr_t ecx, uintptr_t edx, const char *material_name, const char *texture_group_name, bool complain,
	const char *complain_prefix ) {

	static auto original = g_hooks.m_materialsystem.get_old_method< fn::GetMaterial_t >( hook::idx::GET_MATERIAL );

	if( strcmp( material_name, "dev/scope_bluroverlay" ) == 0 && g_vars.visuals.misc.remove_scope ){
		static auto clear = original( ecx, "dev/clearalpha", nullptr, complain, complain_prefix );
		return clear;
	}
	
	return original( ecx, material_name, texture_group_name, complain, complain_prefix );
}