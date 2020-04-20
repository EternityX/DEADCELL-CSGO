#pragma once

class c_bone_bit_list;
class c_ik_context;

namespace hook {
	namespace fn {
		using Present_t = HRESULT (__stdcall *)( IDirect3DDevice9Ex *, const RECT *, const RECT *, HWND, const RGNDATA * );
		using Reset_t = HRESULT (__stdcall *)( IDirect3DDevice9Ex *, D3DPRESENT_PARAMETERS * );
		using ResetEx_t = HRESULT(__stdcall *)( IDirect3DDevice9Ex *, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX * );
		using EndScene_t = HRESULT (__stdcall *)( IDirect3DDevice9Ex * );
		using PaintTraverse_t = void (__thiscall *)( uintptr_t, int, bool, bool );
		using LevelInitPostEntity_t = void (__thiscall *)( uintptr_t );
		using LevelShutdown_t = void (__thiscall *)( uintptr_t );
		using CreateMove_t = bool (__thiscall *)( uintptr_t, float, c_user_cmd * );
		using ShouldDrawFog_t = bool (__thiscall *)( uintptr_t );
		using OverrideView_t = void (__thiscall *)( uintptr_t, c_view_setup * );
		using GetViewModelFOV_t = float(__thiscall *)( uintptr_t );
		using OverrideConfig_t = bool( __thiscall *)( uintptr_t, MaterialSystem_Config_t *, bool );
		using BeginFrame_t = void (__thiscall *)( uintptr_t, float );
		using SceneEnd_t = void (__thiscall *)( uintptr_t );
		using DrawModelExecute_t = void (__thiscall *)( uintptr_t, IMatRenderContext *, void *, const model_render_info_t &, matrix3x4_t * );
		using FrameStageNotify_t = void (__thiscall *)( uintptr_t, client_frame_stage_t );
		using DrawModel_t = void (__thiscall *)( uintptr_t, uintptr_t, DrawModelInfo_t &, matrix3x4_t *, float *, float *, vec3_t &, int );
		using DoPostScreenEffects_t = bool (__thiscall *)( uintptr_t, const c_view_setup * );
		using DispatchUserMessage_t = bool (__thiscall *)( uintptr_t, unsigned int, unsigned int, unsigned int, const void * );
		using LockCursor_t = void (__thiscall *)( i_surface * );
		using GetMaterial_t = i_material *( __thiscall*)( uintptr_t, const char *, const char *, bool, const char * );
		using RenderSmokeOverlay_t = bool (__thiscall *)( uintptr_t, bool );
		using RunCommand_t = void( __thiscall *)( uintptr_t, c_base_player *, c_user_cmd *, i_move_helper * );
		using IsHltv_t = bool( __thiscall * )( uintptr_t );
		using DrawSetColor_t = void( __thiscall * )( i_surface*, int, int, int, int );
	};

	// enum for indexes for easier updating
	enum idx : int {
		// directx
		RESET =						16,
		PRESENT =					17,
		END_SCENE =					42,
		RESETEX =					132,

		// clientmode
		SHOULD_DRAW_FOG =			17,
		OVERRIDE_VIEW =				18,
		CREATE_MOVE =				24,
		GET_VIEWMODEL_FOV =			35,
		DO_POST_SCREEN_SPACE_FX =   44,

		//engine
		IS_HLTV = 93,

		// client
		LEVEL_INIT_POST_ENTITY =	 6,
		LEVEL_SHUTDOWN =			 7,
		FRAME_STAGE_NOTIFY =		 37,

		// surface
		LOCK_CURSOR =				 67,
		DRAW_SET_COLOR =			 15,

		// panel
		PAINT_TRAVERSE =			 41,

		// modelrender
		DRAW_MODEL_EXECUTE =		 21,

		// renderview
		SCENE_END =					 9,

		// viewrender
		RENDER_SMOKE_OVERLAY =		 41,

		// materialsystem
		GET_MATERIAL =				 84,

		// ccsplayer

		DO_EXTRA_BONE_PROC =		 197,

		// cbaseanimating
		SETUP_BONES =                13
	};

	// our funcs.
	HRESULT __stdcall Present( IDirect3DDevice9Ex *device, const RECT *pSourceRect, const RECT *pDestRect,
	                           HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
	HRESULT __stdcall Reset( IDirect3DDevice9Ex *device, D3DPRESENT_PARAMETERS *pPresentationParameters );
	HRESULT __stdcall ResetEx(IDirect3DDevice9Ex *device, D3DPRESENT_PARAMETERS *pPresentationParameters, D3DDISPLAYMODEEX *pFullscreenDisplayMode );
	HRESULT __stdcall EndScene( IDirect3DDevice9Ex *device );
	void __stdcall LockCursor( );
	void __fastcall PaintTraverse( uintptr_t ecx, uintptr_t edx, int vguiPanel, bool forceRepaint, bool allowForce );
	void __fastcall LevelInitPostEntity( uintptr_t ecx, uintptr_t edx );
	void __fastcall LevelShutdown( uintptr_t ecx, uintptr_t edx );
	bool __fastcall CreateMove( uintptr_t ecx, uintptr_t edx, float flInputSampleTime, c_user_cmd *cmd );
	bool __fastcall ShouldDrawFog( uintptr_t ecx, uintptr_t edx );
	void __fastcall OverrideView( uintptr_t ecx, uintptr_t edx, c_view_setup *pSetup );
	float __fastcall GetViewModelFOV( uintptr_t ecx, uintptr_t edx );
	i_material * __fastcall GetMaterial( uintptr_t ecx, uintptr_t edx, const char *material_name, const char *texture_group_name, bool complain, const char *complain_prefix );
	void __fastcall SceneEnd( uintptr_t ecx, uintptr_t edx );
	void __fastcall DrawModelExecute( uintptr_t ecx, uintptr_t edx, IMatRenderContext *ctx, void *state, model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld );
	void __fastcall FrameStageNotify( uintptr_t ecx, uintptr_t edx, client_frame_stage_t curstage );
	bool __fastcall DoPostScreenSpaceEffects( uintptr_t ecx, uintptr_t edx, const c_view_setup *pSetup );
	void __fastcall RenderSmokeOverlay( uintptr_t ecx, uintptr_t edx, bool a1 );
	bool __fastcall IsHltv( uintptr_t ecx, uintptr_t edx );
	void __fastcall DrawSetColor( i_surface* ecx, uintptr_t edx, int r, int g, int b, int a );
};

class c_hooks {
public:
	bool m_hooked_successful = false;

	c_vmt m_directx;
	c_vmt m_clientmode;
	c_vmt m_client;
	c_vmt m_surface;
	c_vmt m_panel;
	c_vmt m_renderview;
	c_vmt m_modelrender;
	c_vmt m_materialsystem;
	c_vmt m_viewrender;
	c_vmt m_engine;

	bool init( );
	bool hook( );
	bool release( );
};

extern c_hooks g_hooks;
