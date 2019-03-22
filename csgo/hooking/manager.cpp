#include "../inc.hpp"


c_hooks g_hooks{ };

void c_hooks::init( ) {

	m_directx.init( g_csgo.m_d3d9_vmt );
	m_clientmode.init( g_csgo.m_client_mode );
	m_client.init( g_csgo.m_client );
	m_surface.init( g_csgo.m_surface );
	m_panel.init( g_csgo.m_panel );
	m_renderview.init( g_csgo.m_render_view );
	m_modelrender.init( g_csgo.m_model_render );
	m_materialsystem.init( g_csgo.m_material_system );
	m_viewrender.init( g_csgo.m_viewrender );
}

void c_hooks::hook( ) {
	m_directx.hook_method( hook::idx::PRESENT, &hook::Present );
	m_directx.hook_method( hook::idx::RESET, &hook::Reset );

	m_clientmode.hook_method( hook::idx::SHOULD_DRAW_FOG, &hook::ShouldDrawFog );
	m_clientmode.hook_method( hook::idx::CREATE_MOVE, &hook::CreateMove );
	m_clientmode.hook_method( hook::idx::OVERRIDE_VIEW, &hook::OverrideView );
	//m_clientmode.hook_method( 35, &hook::GetViewModelFOV );
	m_clientmode.hook_method( 35, &hook::GetViewModelFOV );

	m_client.hook_method( hook::idx::LEVEL_INIT_POST_ENTITY, &hook::LevelInitPostEntity );
	m_client.hook_method( hook::idx::LEVEL_SHUTDOWN, &hook::LevelShutdown );
	m_client.hook_method( hook::idx::FRAME_STAGE_NOTIFY, &hook::FrameStageNotify );

	m_surface.hook_method( hook::idx::LOCK_CURSOR, &hook::LockCursor );

	m_panel.hook_method( hook::idx::PAINT_TRAVERSE, &hook::PaintTraverse );

	m_modelrender.hook_method( hook::idx::DRAW_MODEL_EXECUTE, &hook::DrawModelExecute );

	m_renderview.hook_method( hook::idx::SCENE_END, hook::SceneEnd );

	m_viewrender.hook_method( hook::idx::RENDER_SMOKE_OVERLAY, hook::RenderSmokeOverlay );

	m_materialsystem.hook_method( hook::idx::GET_MATERIAL, hook::GetMaterial );
}


void c_hooks::release( ) {
	m_directx.unhook_all( );
	m_clientmode.unhook_all( );
	m_client.unhook_all( );
	m_surface.unhook_all( );
	m_panel.unhook_all(  );
	m_modelrender.unhook_all( );
	m_renderview.unhook_all( );
	m_viewrender.unhook_all( );
	m_materialsystem.unhook_all( );
};