#include "../../inc.h"
#include "../../features/visuals/visuals.h"
#include "../../features/nade_pred/nade_pred.h"

IDirect3DVertexDeclaration9* vert_dec; 
IDirect3DVertexShader9* vert_shader;
DWORD old_colorwrite;

void save( IDirect3DDevice9Ex* device ) {
	device->GetRenderState( D3DRS_COLORWRITEENABLE, &old_colorwrite );
	device->GetVertexDeclaration( &vert_dec );
	device->GetVertexShader( &vert_shader );
	device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
	device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	device->SetSamplerState( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	device->SetSamplerState( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	device->SetSamplerState( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
	device->SetSamplerState( NULL, D3DSAMP_SRGBTEXTURE, NULL );
}

void restore( IDirect3DDevice9Ex *device )
{
	device->SetRenderState( D3DRS_COLORWRITEENABLE, old_colorwrite );
	device->SetRenderState( D3DRS_SRGBWRITEENABLE, true );
	device->SetVertexDeclaration( vert_dec );
	device->SetVertexShader( vert_shader );
}

long __stdcall hook::EndScene( IDirect3DDevice9Ex *device ) {
	static auto wanted_ret_address = _ReturnAddress( );
	if ( _ReturnAddress( ) == wanted_ret_address ) {
		save( device ); {
			static bool once { false };
			if ( !once ) {
				g_input.init( "Valve001" );
				g_renderer.init( device );
				g_menu.init( device );
				once = true;
			}
			else {
				ImGuiIO &io = ImGui::GetIO( );
				io.MouseDrawCursor = g_menu.is_active();

				g_renderer.start_drawing( );
				{
					g_visuals.run( );

					g_nadepred.draw( );

					g_notify.draw( );

					if ( g_vars.visuals.extra.misc_flags ) {
						int flag_count = 0;

						if ( g_cl.m_under_tickrate ) {
							g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_ENHANCE_9PX ), ImGui::GetColorU32( { 0.90f, 0.90f, 0.17f, 0.04f } ), ImGui::GetColorU32( { 0.78f, 0.0f, 0.0f, 0.0f } ), 15, ( io.DisplaySize.x / 2 ) - flag_count++ * 21, DROPSHADOW, "FPS" );
							g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_7PX ), ImGui::GetColorU32( { 0.90f, 0.90f, 0.17f, 0.04f } ), ImGui::GetColorU32( { 0.78f, 0.0f, 0.0f, 0.0f } ), 15, ( io.DisplaySize.y / 2 ) - 1 * 11, DROPSHADOW, "%i", g_cl.m_client_framerate );
						}
					}

					g_visuals.watermark( );
				}
				g_renderer.end_drawing( );

				g_menu.render();

				g_renderer.render( );
			}
		} restore( device );
	}

	return g_hooks.m_directx.get_old_method< fn::EndScene_t >( hook::idx::END_SCENE )( device );
}
