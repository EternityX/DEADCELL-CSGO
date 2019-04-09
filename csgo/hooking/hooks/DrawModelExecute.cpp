#include "../../inc.hpp"
#include "../../features/chams/chams.h"

void __fastcall hook::DrawModelExecute( uintptr_t ecx, uintptr_t edx, IMatRenderContext *ctx, void *state, const model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld ){
	static auto original = g_hooks.m_modelrender.get_old_method< fn::DrawModelExecute_t >( hook::idx::DRAW_MODEL_EXECUTE );
	if ( g_chams.on_dme( ctx, state, pInfo, pCustomBoneToWorld ) )
		original( ecx, ctx, state, pInfo, pCustomBoneToWorld );




	g_csgo.m_model_render->forced_material_override( nullptr );
}