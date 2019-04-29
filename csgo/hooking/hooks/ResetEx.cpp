#include "../../inc.h"

HRESULT __stdcall hook::ResetEx( IDirect3DDevice9Ex *device, D3DPRESENT_PARAMETERS *pPresentationParameters, D3DDISPLAYMODEEX *pFullscreenDisplayMode ) {
	g_renderer.get_renderer( ).PreD3DReset( );

	const long ret = g_hooks.m_directx.get_old_method< fn::ResetEx_t >( hook::idx::RESETEX )( device, pPresentationParameters, pFullscreenDisplayMode);

	g_renderer.get_renderer( ).PostD3DReset( );

	return ret;
}
