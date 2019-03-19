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
	m_directx.hook_method( 17, &hook::Present );
	m_directx.hook_method( 16, &hook::Reset );

	m_clientmode.hook_method( 17, &hook::ShouldDrawFog );
	m_clientmode.hook_method( 24, &hook::CreateMove );
	m_clientmode.hook_method( 18, &hook::OverrideView );
	//m_clientmode.hook_method( 35, &hook::GetViewModelFOV );

	m_client.hook_method( 7, &hook::LevelInitPostEntity );
	m_client.hook_method( 8, &hook::LevelShutdown );
	m_client.hook_method( 37, &hook::FrameStageNotify );

	m_surface.hook_method( 67, &hook::LockCursor );

	m_panel.hook_method( 41, &hook::PaintTraverse );

	m_modelrender.hook_method( 21, &hook::DrawModelExecute );

	m_renderview.hook_method( 9, hook::SceneEnd );

	m_viewrender.hook_method( 41, hook::RenderSmokeOverlay );

	m_materialsystem.hook_method( 84, hook::GetMaterial );
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