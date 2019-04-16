#include "../../inc.h"
#include "../../features/chams/chams.h"
#include "../../features/visuals/visuals.h"
#include "../../features/nade_pred/nade_pred.h"

void __fastcall hook::SceneEnd( uintptr_t ecx, uintptr_t edx ){
	g_hooks.m_renderview.get_old_method< fn::SceneEnd_t >( hook::idx::SCENE_END )( ecx );

	g_chams.on_sceneend( );
}
