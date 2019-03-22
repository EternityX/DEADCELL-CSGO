#include "../../inc.hpp"

HRESULT __stdcall hook::Reset( IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *pPresentationParameters ) {
	g_renderer.m_instance->GetRenderer( ).PreD3DReset( );

	auto ret = g_hooks.m_directx.get_old_method< fn::Reset_t >( hook::idx::RESET )( device, pPresentationParameters );

	g_renderer.m_instance->GetRenderer( ).PostD3DReset( );

	return ret;
}
