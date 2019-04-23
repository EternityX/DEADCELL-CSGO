#include "../inc.hpp"
#include "renderer.hpp"
#include "font.hpp"
#include "../../oshgui/Drawing/FreeTypeFont.hpp"

using namespace renderer;

c_renderer::c_renderer( ) { }

void c_renderer::init( IDirect3DDevice9Ex *device ) {
	// initialize oshgui with our renderer.
	OSHGui::Application::Initialize( std::make_unique< OSHGui::Drawing::Direct3D9ExRenderer >( device ) );

	m_fonts.resize( FONT_MAX );

	m_fonts[ FONT_VERDANA_7PX ] = OSHGui::Drawing::FontManager::LoadFont( "Verdana", 7.f, true );
	m_fonts[ FONT_VERDANA_BOLD_7PX ] = OSHGui::Drawing::FontManager::LoadFont( "Verdana Bold", 7.f, true );
	m_fonts[ FONT_04B03_6PX ] = OSHGui::Drawing::FontManager::LoadFontFromMemory( OSHGui::Misc::RawDataContainer( pixel_type_font ), 6.f, false );
	m_fonts[ FONT_LUCIDA_CONSOLE_7PX ] = OSHGui::Drawing::FontManager::LoadFont( "Lucida Console", 7.5f, false );
	m_fonts[ FONT_MARLETT_45PX ] = OSHGui::Drawing::FontManager::LoadFontFromMemory( OSHGui::Misc::RawDataContainer( marlett_font ), 45.f, true );
	m_fonts[ FONT_ENHANCE_9PX ] = OSHGui::Drawing::FontManager::LoadFontFromMemory( OSHGui::Misc::RawDataContainer( enhance_font ), 9.f, true );

	get_instance( )->SetDefaultFont( m_fonts[ FONT_VERDANA_7PX ] );
}

void c_renderer::start_drawing( ) {
	m_render_target = get_renderer( ).GetDefaultRenderTarget( );
	if ( !m_render_target )
		return;

	m_geometry = get_renderer( ).CreateGeometryBuffer( );
	if( !m_geometry )
		return;

	get_renderer( ).BeginRendering( );
}

void c_renderer::end_drawing( ) const {
	if( !m_render_target || !m_geometry )
		return;

	m_render_target->Activate( );

	m_render_target->Draw( *m_geometry );

	m_render_target->Deactivate( );

	get_instance( )->Render( );

	get_instance( )->GetRenderer( ).EndRendering( );
}

void c_renderer::rect( const OSHGui::Drawing::Color &color, int x, int y, int width, int height ) const {
	OSHGui::Drawing::Graphics g( *m_geometry );

	g.DrawRectangle( color, OSHGui::Drawing::PointI( x, y ), OSHGui::Drawing::SizeI( width, height ) );
}

void c_renderer::filled_rect( const OSHGui::Drawing::Color &color, int x, int y, int width, int height ) const {
	OSHGui::Drawing::Graphics g( *m_geometry );

	g.FillRectangle( color, OSHGui::Drawing::PointI( x, y ), OSHGui::Drawing::SizeI( width, height ) );
}

void c_renderer::ansi_text( const OSHGui::Drawing::FontPtr &font, const OSHGui::Drawing::Color &color, const OSHGui::Drawing::Color &shadow_color, int x, int y, int flags, const std::string str, ... ) const {
	va_list va;
	std::string buf;

	if( str.empty( ) )
		return;

	va_start( va, str );

	int str_len = std::vsnprintf( nullptr, 0, str.c_str( ), va );
	if( str_len < 0 ) {
		va_end( va );
		return;
	}

	buf.resize( str_len );

	std::vsnprintf( &buf[ 0 ], str_len + 1, str.c_str( ), va );

	OSHGui::Drawing::Graphics g( *m_geometry );

	if( flags & CENTERED_X || flags & CENTERED_Y ) {
		OSHGui::Misc::TextHelper text_helper( font );
		text_helper.SetText( str );

		if( flags & CENTERED_X )
			x -= text_helper.GetSize( ).Width / 2.f;

		if( flags & CENTERED_Y )
			y -= text_helper.GetSize( ).Height / 2.f;
	}

	if( flags & DROPSHADOW )
		g.DrawString( buf, font, shadow_color, x + 1, y + 1 );

	if( flags & OUTLINED ) {
		g.DrawString( buf, font, shadow_color, x, y + 1 );
		g.DrawString( buf, font, shadow_color, x, y - 1 );
		g.DrawString( buf, font, shadow_color, x + 1, y );
		g.DrawString( buf, font, shadow_color, x - 1, y );
	}

	g.DrawString( buf, font, color, x, y );
}

void c_renderer::filled_rect_gradient( const OSHGui::Drawing::ColorRectangle &colors, int x, int y, int width, int height ) const {
	OSHGui::Drawing::Graphics g( *m_geometry );

	g.FillRectangleGradient( colors, OSHGui::Drawing::PointI( x, y ), OSHGui::Drawing::SizeI( width, height ) );
}

void c_renderer::line( const OSHGui::Drawing::Color &color, float x, float y, float x1, float y1 ) const {
	OSHGui::Drawing::Graphics g( *m_geometry );

	g.DrawLine( color, OSHGui::Drawing::PointF( x, y ), OSHGui::Drawing::PointF( x1, y1 ) );
}

void c_renderer::circle( const OSHGui::Drawing::Color &color, int x, int y, int radius ) const {
	OSHGui::Drawing::Graphics g( *m_geometry );

	g.FillCircle( color, OSHGui::Drawing::PointI( x, y ), radius );
}

OSHGui::Drawing::FontPtr c_renderer::get_font( int index ) {
	return m_fonts[ index ];
}

OSHGui::Application *c_renderer::get_instance( ) const {
	return OSHGui::Application::InstancePtr( );
}

OSHGui::Drawing::Renderer &c_renderer::get_renderer( ) const {
	return get_instance( )->GetRenderer( );
}
