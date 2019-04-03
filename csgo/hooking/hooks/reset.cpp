#include "../../inc.hpp"

HRESULT __stdcall hook::Reset( IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *pPresentationParameters ) {
	g_renderer.get_renderer( ).PreD3DReset( );

	const long ret = g_hooks.m_directx.get_old_method< fn::Reset_t >( hook::idx::RESET )( device, pPresentationParameters );

	g_renderer.get_renderer( ).PostD3DReset( );

	return ret;
}
