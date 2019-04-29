#include "../inc.h"

c_hooks g_hooks{ };

bool c_hooks::init( ) {
	if( !m_directx.init( g_csgo.m_d3d9_vmt ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_directx. This is fatal./n/n%#08X", g_csgo.m_d3d9_vmt );
		return false;
	}

	if( !m_clientmode.init( g_csgo.m_client_mode ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_client_mode. This is fatal./n/n%#08X", g_csgo.m_client_mode );
		return false;
	}

	if( !m_client.init( g_csgo.m_client ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_client. This is fatal./n/n%#08X", g_csgo.m_client );
		return false;
	}

	if( !m_surface.init( g_csgo.m_surface ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_surface. This is fatal./n/n%#08X", g_csgo.m_surface );
		return false;
	}

	if( !m_panel.init( g_csgo.m_panel ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_panel. This is fatal./n/n%#08X", g_csgo.m_panel );
		return false;
	}

	if( !m_renderview.init( g_csgo.m_render_view ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_render_view. This is fatal./n/n%#08X", g_csgo.m_render_view );
		return false;
	}

	if( !m_modelrender.init( g_csgo.m_model_render ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_model_render. This is fatal./n/n%#08X", g_csgo.m_model_render );
		return false;
	}

	if( !m_materialsystem.init( g_csgo.m_material_system ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_material_system. This is fatal./n/n%#08X", g_csgo.m_material_system );
		return false;
	}

	if( !m_viewrender.init( g_csgo.m_viewrender ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_viewrender. This is fatal./n/n%#08X", g_csgo.m_viewrender );
		return false;
	}

	if( !m_engine.init( g_csgo.m_engine ) ) {
		_RPTF1( _CRT_ERROR, "Failed to initialize m_engine. This is fatal./n/n%#08X", g_csgo.m_engine );
		return false;
	}

	return true;
}

bool c_hooks::hook( ) {
	/*if( !m_directx.hook_method( hook::idx::PRESENT, &hook::Present ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook Present. This is fatal." );
		return false;
	}*/

	if( !m_directx.hook_method( hook::idx::RESET, &hook::Reset ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook Reset. This is fatal." );
		return false;
	}

	if (!m_directx.hook_method(hook::idx::RESETEX, &hook::ResetEx)) {
		_RPTF0(_CRT_ERROR, "Failed to hook ResetEx. This is fatal.");
		return false;
	}

	if( !m_directx.hook_method( hook::idx::END_SCENE, &hook::EndScene ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook EndScene. This is fatal." );
		return false;
	}

	if( !m_clientmode.hook_method( hook::idx::SHOULD_DRAW_FOG, &hook::ShouldDrawFog ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook ShouldDrawFog. This is fatal." );
		return false;
	}

	if( !m_clientmode.hook_method( hook::idx::CREATE_MOVE, &hook::CreateMove ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook CreateMove. This is fatal." );
		return false;
	}

	if( !m_clientmode.hook_method( hook::idx::OVERRIDE_VIEW, &hook::OverrideView ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook OverrideView. This is fatal." );
		return false;
	}

	if( !m_clientmode.hook_method( hook::idx::GET_VIEWMODEL_FOV, &hook::GetViewModelFOV ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook GetViewModelFOV. This is fatal." );
		return false;
	}

	if( !m_clientmode.hook_method( hook::idx::DO_POST_SCREEN_SPACE_FX, &hook::DoPostScreenSpaceEffects ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook DoPostScreenSpaceEffects. This is fatal." );
		return false;
	}

	if( !m_client.hook_method( hook::idx::LEVEL_INIT_POST_ENTITY, &hook::LevelInitPostEntity ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook LevelInitPostEntity. This is fatal." );
		return false;
	}

	if( !m_client.hook_method( hook::idx::LEVEL_SHUTDOWN, &hook::LevelShutdown ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook LevelShutdown. This is fatal." );
		return false;
	}

	if( !m_client.hook_method( hook::idx::FRAME_STAGE_NOTIFY, &hook::FrameStageNotify ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook FrameStageNotify. This is fatal." );
		return false;
	}

	if( !m_surface.hook_method( hook::idx::LOCK_CURSOR, &hook::LockCursor ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook LockCursor. This is fatal." );
		return false;
	}

	if ( !m_surface.hook_method( hook::idx::DRAW_SET_COLOR, &hook::DrawSetColor ) )
	{
		_RPTF0( _CRT_ERROR, "Failed to hook DrawSetColor. This is fatal." );
		return false;
	}

	if( !m_panel.hook_method( hook::idx::PAINT_TRAVERSE, &hook::PaintTraverse ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook PaintTraverse. This is fatal." );
		return false;
	}

	if( !m_modelrender.hook_method( hook::idx::DRAW_MODEL_EXECUTE, &hook::DrawModelExecute ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook DrawModelExecute. This is fatal." );
		return false;
	}

	if( !m_renderview.hook_method( hook::idx::SCENE_END, &hook::SceneEnd ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook SceneEnd. This is fatal." );
		return false;
	}

	if( !m_viewrender.hook_method( hook::idx::RENDER_SMOKE_OVERLAY, &hook::RenderSmokeOverlay ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook RenderSmokeOverlay. This is fatal." );
		return false;
	}

	if( !m_materialsystem.hook_method( hook::idx::GET_MATERIAL, &hook::GetMaterial ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook GetMaterial. This is fatal." );
		return false;
	}

	if( !m_engine.hook_method( hook::idx::IS_HLTV, &hook::IsHltv ) ) {
		_RPTF0( _CRT_ERROR, "Failed to hook IsHltv. This is fatal." );
		return false;
	}

	m_hooked_successful = true;

	return true;
}

bool c_hooks::release( ) {
	if( !m_directx.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_directx. This is fatal." );
		return false;
	}

	if( !m_clientmode.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_clientmode. This is fatal." );
		return false;
	}

	if( !m_client.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_client. This is fatal." );
		return false;
	}

	if( !m_surface.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_surface. This is fatal." );
		return false;
	}

	if( !m_panel.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_panel. This is fatal." );
		return false;
	}

	if( !m_modelrender.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_modelrender. This is fatal." );
		return false;
	}

	if( !m_renderview.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_renderview. This is fatal." );
		return false;
	}

	if( !m_viewrender.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_viewrender. This is fatal." );
		return false;
	}

	if( !m_materialsystem.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_materialsystem. This is fatal." );
		return false;
	}

	if( !m_engine.unhook_all( ) ) {
		_RPTF0( _CRT_ERROR, "Failed to unhook all functions from m_engine. This is fatal." );
		return false;
	}

	return true;
};
