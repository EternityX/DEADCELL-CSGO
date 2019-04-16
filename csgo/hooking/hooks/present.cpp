#include "../../inc.h"
#include "../../features/visuals/visuals.h"
#include "../../features/nade_pred/nade_pred.h"
#include "../../features/anti-aim/antiaim.h"

HRESULT __stdcall hook::Present( IDirect3DDevice9Ex *device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion ) {
	return g_hooks.m_directx.get_old_method< fn::Present_t >( hook::idx::PRESENT )( device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}
