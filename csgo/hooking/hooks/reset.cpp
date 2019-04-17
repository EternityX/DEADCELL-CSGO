#include "../../inc.h"

HRESULT __stdcall hook::Reset( IDirect3DDevice9Ex *device, D3DPRESENT_PARAMETERS *pPresentationParameters ) {
	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const long ret = g_hooks.m_directx.get_old_method< fn::Reset_t >( hook::idx::RESET )( device, pPresentationParameters );

	ImGui_ImplDX9_CreateDeviceObjects( );

	return ret;
}
