#pragma once

class c_csgo : protected c_interface_mgr {
private:
	using CommandLine_t = ICommandLine *( __stdcall * )( );

public:
	bool m_interfaces_successful = false;

	// modules.
	pe::Module m_client_dll;
	pe::Module m_engine_dll;

	// misc ptrs;
	uintptr_t m_d3d9_vmt = 0;

	// interfaces.
	CHLClient *m_client                    = nullptr;
	IEngineClient *m_engine                = nullptr;
	IClientModeShared *m_client_mode       = nullptr;
	IClientEntityList *m_entity_list       = nullptr;
	CInput *m_input                        = nullptr;
	CGlobalVars *m_global_vars             = nullptr;
	ICVar *m_convar                        = nullptr;
	IVModelInfo *m_model_info              = nullptr;
	IMaterialSystem *m_material_system     = nullptr;
	IVRenderView *m_render_view            = nullptr;
	IVModelRender *m_model_render          = nullptr;
	ISurface *m_surface                    = nullptr;
	IVDebugOverlay *m_debug_overlay        = nullptr;
	IPanel *m_panel                        = nullptr;
	IStudioRender *m_studio_render         = nullptr;
	IEngineTrace *m_engine_trace           = nullptr;
	IGameMovement *m_game_movement         = nullptr;
	IPrediction *m_prediction              = nullptr;
	IMoveHelper *m_movehelper              = nullptr;
	IGameEventManager2 *m_game_event       = nullptr;
	IViewRenderBeams *m_render_beams       = nullptr;
	IWeaponSystem *m_weapon_system         = nullptr;
	IVEfx *m_effects                       = nullptr;
	ClientState *m_clientstate             = nullptr;
	CPhysicsSurface *m_physics_surface     = nullptr;
	ILocalize *m_localize                  = nullptr;
	IMemAlloc *m_memalloc                  = nullptr;
	IMDLCache *m_modelcache                = nullptr;
	IViewRender *m_viewrender              = nullptr;
	CGlowObjectManager *m_glow_obj_manager = nullptr;
	CommandLine_t m_command_line           = nullptr;

	c_csgo() { }

	bool init( ) {
		// modules.
		m_client_dll = pe::get_module( CT_HASH32( "client_panorama.dll" ) );
		m_engine_dll = pe::get_module( CT_HASH32( "engine.dll" ) );
		if( !m_client_dll || !m_engine_dll )
			return false;

		// misc ptrs.
		m_d3d9_vmt = pattern::find( CT_HASH32( "shaderapidx9.dll" ), "A1 ? ? ? ? 50 8B 08 FF 51 0C" );
		m_d3d9_vmt = **reinterpret_cast< uintptr_t ** >( m_d3d9_vmt + 1 );
		if( !m_d3d9_vmt )
			return false;

		m_memalloc = *pe::get_export< IMemAlloc ** >( pe::get_module( "tier0.dll" ), "g_pMemAlloc" );
		if( !m_memalloc )
			return false;

		m_command_line = pe::get_export< CommandLine_t >( pe::get_module( "tier0.dll" ), "CommandLine" );
		if( !m_command_line )
			return false;

		// interfaces.
		m_client = get_interface< CHLClient >( CT_HASH32( "VClient" ) );
		if( !m_client )
			return false;

		m_engine = get_interface< IEngineClient >( CT_HASH32( "VEngineClient" ) );
		if( !m_engine )
			return false;

		m_client_mode = **c_vmt::get_method( m_client, 10 ).add< IClientModeShared *** >( 5 );
		if( !m_client_mode )
			return false;

		m_input = c_vmt::get_method( m_client, 20 ).at< CInput * >( 4 );
		if( !m_input )
			return false;

		m_entity_list = get_interface< IClientEntityList >( CT_HASH32( "VClientEntityList" ) );
		if( !m_entity_list )
			return false;

		m_global_vars = **c_vmt::get_method( m_client, 0 ).add< CGlobalVars *** >( 27 );
		if( !m_global_vars )
			return false;

		m_convar = get_interface< ICVar >( CT_HASH32( "VEngineCvar" ) );
		if( !m_convar )
			return false;

		m_model_info = get_interface< IVModelInfo >( CT_HASH32( "VModelInfoClient" ) );
		if( !m_model_info )
			return false;

		m_material_system = get_interface< IMaterialSystem >( CT_HASH32( "VMaterialSystem" ) );
		if( !m_material_system )
			return false;

		m_render_view = get_interface< IVRenderView >( CT_HASH32( "VEngineRenderView" ) );
		if( !m_render_view )
			return false;
		
		m_model_render = get_interface< IVModelRender >( CT_HASH32( "VEngineModel" ) );
		if( !m_model_render )
			return false;

		m_debug_overlay = get_interface< IVDebugOverlay >( CT_HASH32( "VDebugOverlay" ) );
		if( !m_debug_overlay )
			return false;

		m_panel = get_interface< IPanel >( CT_HASH32( "VGUI_Panel" ) );
		if( !m_panel )
			return false;

		m_surface = get_interface< ISurface >( CT_HASH32( "VGUI_Surface" ) );
		if( !m_surface )
			return false;

		m_studio_render = get_interface< IStudioRender >( CT_HASH32( "VStudioRender" ) );
		if( !m_studio_render )
			return false;

		m_engine_trace = get_interface< IEngineTrace >( CT_HASH32( "EngineTraceClient" ) );
		if( !m_engine_trace )
			return false;

		m_game_movement = get_interface< IGameMovement >( CT_HASH32( "GameMovement" ) );
		if( !m_game_movement )
			return false;

		m_prediction = get_interface< IPrediction >( CT_HASH32( "VClientPrediction" ) );
		if( !m_prediction )
			return false;

		m_viewrender = *c_vmt::get_method( m_client, 28 ).at< IViewRender ** >( 8 );
		if( !m_viewrender )
			return false;

		m_movehelper = **reinterpret_cast< IMoveHelper *** >( pattern::find( m_client_dll, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 2 );
		if( !m_movehelper )
			return false;

		m_game_event = get_interface< IGameEventManager2 >( CT_HASH32( "GAMEEVENTSMANAGER002" ), 0, false );
		if( !m_game_event )
			return false;

		m_render_beams = *pattern::find< IViewRenderBeams ** >( m_client_dll, "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9", 1 );
		if( !m_render_beams )
			return false;

		m_weapon_system = *pattern::find< IWeaponSystem ** >( m_client_dll, "8B 35 ? ? ? ? FF 10 0F B7 C0", 2 );
		if( !m_weapon_system )
			return false;

		m_effects = get_interface< IVEfx >( CT_HASH32( "VEngineEffects" ) );
		if( !m_effects )
			return false;

		m_clientstate = **c_vmt::get_method( m_engine, 12 ).add< ClientState*** >( 16 );
		if( !m_clientstate )
			return false;

		m_physics_surface = get_interface< CPhysicsSurface >( CT_HASH32( "VPhysicsSurfaceProps" ) );
		if( !m_physics_surface )
			return false;

		m_localize = get_interface< ILocalize >( CT_HASH32( "Localize_001" ), 0, false );
		if( !m_localize )
			return false;

		m_modelcache = get_interface< IMDLCache >( CT_HASH32( "MDLCache" ) );
		if( !m_modelcache )
			return false;

		m_glow_obj_manager = *pattern::find< CGlowObjectManager ** >( m_client_dll, "0F 11 05 ? ? ? ? 83 C8 01", 3 );
		if( !m_glow_obj_manager )
			return false;

		m_interfaces_successful = true;

		return true;
	}
};

extern c_csgo g_csgo;
