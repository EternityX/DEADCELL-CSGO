#include "../../inc.h"
#include "../../features/chams/chams.h"

void __fastcall hook::DrawModelExecute( uintptr_t ecx, uintptr_t edx, IMatRenderContext *ctx, void *state, model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld ){
	static auto original = g_hooks.m_modelrender.get_old_method< fn::DrawModelExecute_t >( hook::idx::DRAW_MODEL_EXECUTE );

	g_chams.on_dme( ecx, ctx, state, pInfo, pCustomBoneToWorld, original );
	original( ecx, ctx, state, pInfo, pCustomBoneToWorld );
}