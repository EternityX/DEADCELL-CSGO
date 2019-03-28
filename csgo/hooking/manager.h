#pragma once

class CBoneBitList;
class CIKContext;

namespace hook {
	namespace fn {
		using Present_t = HRESULT (__stdcall *)( IDirect3DDevice9 *, const RECT *, const RECT *, HWND, const RGNDATA * );
		using Reset_t = HRESULT (__stdcall *)( IDirect3DDevice9 *, D3DPRESENT_PARAMETERS * );
		using PaintTraverse_t = void (__thiscall *)( uintptr_t, int, bool, bool );
		using LevelInitPostEntity_t = void (__thiscall *)( uintptr_t );
		using LevelShutdown_t = void (__thiscall *)( uintptr_t );
		using CreateMove_t = bool (__thiscall *)( uintptr_t, float, CUserCmd * );
		using ShouldDrawFog_t = bool (__thiscall *)( uintptr_t );
		using OverrideView_t = void (__thiscall *)( uintptr_t, CViewSetup * );
		using GetViewModelFOV_t = float(__thiscall *)( uintptr_t );
		using OverrideConfig_t = bool( __thiscall *)( uintptr_t, MaterialSystem_Config_t *, bool );
		using BeginFrame_t = void (__thiscall *)( uintptr_t, float );
		using SceneEnd_t = void (__thiscall *)( uintptr_t );
		using DrawModelExecute_t = void (__thiscall *)( uintptr_t, IMatRenderContext *, void *, const ModelRenderInfo_t &,
		                                                matrix3x4_t * );
		using FrameStageNotify_t = void (__thiscall *)( uintptr_t, ClientFrameStage_t );
		using DrawModel_t = void (__thiscall *)( uintptr_t, uintptr_t, DrawModelInfo_t &, matrix3x4_t *, float *, float *,
		                                         vec3_t &, int );
		using DoPostScreenEffects_t = bool (__thiscall *)( uintptr_t, const CViewSetup * );
		using DispatchUserMessage_t = bool (__thiscall *)( uintptr_t, unsigned int, unsigned int, unsigned int, const void * );
		using LockCursor_t = void (__thiscall *)( ISurface * );
		using GetMaterial_t = IMaterial *( __thiscall*)( uintptr_t, const char *, const char *, bool, const char * );
		using RenderSmokeOverlay_t = bool (__thiscall *)( uintptr_t, bool );
		using RunCommand_t = void( __thiscall *)( uintptr_t, C_BasePlayer *, CUserCmd *, IMoveHelper * );

	};

	// enum for indexes for easier updating
	enum idx : int {
		// directx
		RESET =					16,
		PRESENT =				17,
		
		// clientmode
		SHOULD_DRAW_FOG =		17,
		OVERRIDE_VIEW =			18,
		CREATE_MOVE =			24,
		GET_VIEWMODEL_FOV =		35,

		// client
		LEVEL_INIT_POST_ENTITY = 7,
		LEVEL_SHUTDOWN =		 8,
		FRAME_STAGE_NOTIFY =	 37,

		// surface
		LOCK_CURSOR =			 67,

		// panel
		PAINT_TRAVERSE =		 41,

		// modelrender
		DRAW_MODEL_EXECUTE =	 21,

		// renderview
		SCENE_END =				 9,

		// viewrender
		RENDER_SMOKE_OVERLAY =	 41,

		// materialsystem
		GET_MATERIAL =			 84,

		// ccsplayer
		TEST_HITBOXES =			 52,
		DO_EXTRA_BONE_PROC =	 193,
		UPDATE_CLIENTSIDE_ANIM = 219,
	};

	// our funcs.
	HRESULT __stdcall Present( IDirect3DDevice9 *device, const RECT *pSourceRect, const RECT *pDestRect,
	                           HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
	HRESULT __stdcall Reset( IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *pPresentationParameters );
	void __stdcall LockCursor( );
	void __fastcall PaintTraverse( uintptr_t ecx, uintptr_t edx, int vguiPanel, bool forceRepaint, bool allowForce );
	void __fastcall LevelInitPostEntity( uintptr_t ecx, uintptr_t edx );
	void __fastcall LevelShutdown( uintptr_t ecx, uintptr_t edx );
	bool __fastcall CreateMove( uintptr_t ecx, uintptr_t edx, float flInputSampleTime, CUserCmd *cmd );
	bool __fastcall ShouldDrawFog( uintptr_t ecx, uintptr_t edx );
	void __fastcall OverrideView( uintptr_t ecx, uintptr_t edx, CViewSetup *pSetup );
	float __fastcall GetViewModelFOV( uintptr_t ecx, uintptr_t edx );
	IMaterial * __fastcall GetMaterial( uintptr_t ecx, uintptr_t edx, const char *material_name, const char *texture_group_name, bool complain, const char *complain_prefix );
	void __fastcall SceneEnd( uintptr_t ecx, uintptr_t edx );
	void __fastcall DrawModelExecute( uintptr_t ecx, uintptr_t edx, IMatRenderContext *ctx, void *state,
	                                  const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld );
	void __fastcall FrameStageNotify( uintptr_t ecx, uintptr_t edx, ClientFrameStage_t curstage );
	bool __fastcall DoPostScreenSpaceEffects( uintptr_t ecx, uintptr_t edx, const CViewSetup *pSetup );
	void __fastcall RenderSmokeOverlay( uintptr_t ecx, uintptr_t edx, bool a1 );
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

	bool init( );
	bool hook( );
	bool release( );
};

extern c_hooks g_hooks;
