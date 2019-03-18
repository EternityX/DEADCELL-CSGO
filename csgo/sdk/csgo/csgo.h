#pragma once

class c_csgo : protected c_interface_mgr {
private:
	// todo - dex; func prototypes go here.

public:
	// modules.
	pe::Module m_client_dll;
	pe::Module m_engine_dll;

	// misc ptrs;
	uintptr_t m_d3d9_vmt;

	// interfaces.
	CHLClient *m_client;
	IEngineClient *m_engine;
	IClientModeShared *m_client_mode;
	IClientEntityList *m_entity_list;
	CInput *m_input;
	CGlobalVars *m_global_vars;
	ICVar *m_convar;
	IVModelInfo *m_model_info;
	IMaterialSystem *m_material_system;
	IVRenderView *m_render_view;
	IVModelRender *m_model_render;
	ISurface *m_surface;
	IVDebugOverlay *m_debug_overlay;
	IPanel *m_panel;
	IStudioRender *m_studio_render;
	IEngineTrace *m_engine_trace;
	IGameMovement *m_game_movement;
	IPrediction *m_prediction;
	IMoveHelper *m_movehelper;
	IGameEventManager2 *m_game_event;
	IViewRenderBeams *m_render_beams;
	IWeaponSystem *m_weapon_system;
	IVEfx *m_effects;
	ClientState *m_clientstate;
	CPhysicsSurface *m_physics_surface;
	ILocalize *m_localize;
	IMemAlloc *m_memalloc;
	IMDLCache *m_modelcache;
	IViewRender *m_viewrender;
	// functions.

	c_csgo() : m_d3d9_vmt{ } { }

	bool init( ) {
		// modules.
		m_client_dll = pe::get_module( CT_HASH32( "client_panorama.dll" ) );
		m_engine_dll = pe::get_module( CT_HASH32( "engine.dll" ) );
		if( !m_client_dll || !m_engine_dll )
			return false;

		// misc ptrs.
		m_d3d9_vmt = pattern::find( CT_HASH32( "shaderapidx9.dll" ), "A1 ? ? ? ? 50 8B 08 FF 51 0C" );
		m_d3d9_vmt = **reinterpret_cast< uintptr_t ** >( m_d3d9_vmt + 1 );

		m_memalloc = *pe::get_export< IMemAlloc** >( pe::get_module( "tier0.dll" ), "g_pMemAlloc" );

		// interfaces.
		m_client = get_interface< CHLClient >( CT_HASH32( "VClient" ) );

		m_engine = get_interface< IEngineClient >( CT_HASH32( "VEngineClient" ) );

		m_client_mode = **c_vmt::get_method( m_client, 10 ).add< IClientModeShared *** >( 5 );

		m_input = c_vmt::get_method( m_client, 20 ).at< CInput * >( 4 );

		m_entity_list = get_interface< IClientEntityList >( CT_HASH32( "VClientEntityList" ) );

		m_global_vars = **c_vmt::get_method( m_client, 0 ).add< CGlobalVars *** >( 27 );

		m_convar = get_interface< ICVar >( CT_HASH32( "VEngineCvar" ) );

		m_model_info = get_interface< IVModelInfo >( CT_HASH32( "VModelInfoClient" ) );

		m_material_system = get_interface< IMaterialSystem >( CT_HASH32( "VMaterialSystem" ) );

		m_render_view = get_interface< IVRenderView >( CT_HASH32( "VEngineRenderView" ) );

		m_model_render = get_interface< IVModelRender >( CT_HASH32( "VEngineModel" ) );

		m_debug_overlay = get_interface< IVDebugOverlay >( CT_HASH32( "VDebugOverlay" ) );

		m_panel = get_interface< IPanel >( CT_HASH32( "VGUI_Panel" ) );

		m_surface = get_interface< ISurface >( CT_HASH32( "VGUI_Surface" ) );

		m_studio_render = get_interface< IStudioRender >( CT_HASH32( "VStudioRender" ) );

		m_engine_trace = get_interface< IEngineTrace >( CT_HASH32( "EngineTraceClient" ) );

		m_game_movement = get_interface< IGameMovement >( CT_HASH32( "GameMovement" ) );

		m_prediction = get_interface< IPrediction >( CT_HASH32( "VClientPrediction" ) );

		m_viewrender = *c_vmt::get_method( m_client, 28 ).at< IViewRender ** >( 8 );

		m_movehelper = **reinterpret_cast< IMoveHelper *** >( pattern::find( m_client_dll, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 2 );

		m_game_event = get_interface< IGameEventManager2 >( CT_HASH32( "GAMEEVENTSMANAGER002" ), 0, false );

		m_render_beams = *pattern::find< IViewRenderBeams **>( m_client_dll, "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9", 1 );

		m_weapon_system = *pattern::find< IWeaponSystem **>( m_client_dll, "8B 35 ? ? ? ? FF 10 0F B7 C0", 2 );

		m_effects = get_interface< IVEfx >( CT_HASH32( "VEngineEffects" ) );

		m_clientstate = **c_vmt::get_method( m_engine, 12 ).add< ClientState*** >( 16 );

		m_physics_surface = get_interface< CPhysicsSurface >( CT_HASH32( "VPhysicsSurfaceProps" ) );

		m_localize = get_interface< ILocalize >( CT_HASH32( "Localize_001" ), 0, false );

		m_modelcache = get_interface< IMDLCache >( CT_HASH32( "MDLCache" ) );

		return true;
	}
};

extern c_csgo g_csgo;
