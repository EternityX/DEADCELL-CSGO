#include "../inc.hpp"
#include "renderer.hpp"

#include <misc/freetype/imgui_freetype.h>

bool get_system_font_path( const std::string &font_name, std::string &font_path );

using namespace renderer;

c_renderer::c_renderer( ) { }

void c_renderer::init( IDirect3DDevice9Ex *device ) {
	if ( !ImGui::CreateContext( ) ) {
		return;
	}

	if ( !ImGui_ImplWin32_Init( g_input.get_window_handle( ) ) ) {
		return;
	}

	if ( !ImGui_ImplDX9_Init( device ) ) {
		return;
	}

	ImGui::StyleColorsDark( );

	ImGuiIO &io = ImGui::GetIO( );
	//io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	{
		m_fonts.resize( FONT_MAX );

		std::string tahoma, verdana, verdana_bold, fontawesome;
		
		if( !get_system_font_path( "Tahoma", tahoma ) ) {
			return;
		}

		if( !get_system_font_path( "Verdana", verdana ) ) {
			return;
		}

		if( !get_system_font_path( "Verdana Bold", verdana_bold ) ) {
			return;
		}

		io.Fonts->AddFontFromFileTTF( tahoma.data(), 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault() );
		io.Fonts->AddFontAwesome( 56.0f );
		
		// misc fonts
		m_fonts[ FONT_VERDANA_7PX ] = io.Fonts->AddFontFromFileTTF( verdana.data( ), 12.0f, nullptr, io.Fonts->GetGlyphRangesDefault( ) );
		m_fonts[ FONT_VERDANA_BOLD_7PX ] = io.Fonts->AddFontFromFileTTF( verdana_bold.data( ), 12.0f, nullptr, io.Fonts->GetGlyphRangesDefault( ) );
		m_fonts[ FONT_04B03_6PX ] = m_fonts[ FONT_VERDANA_7PX ];
		m_fonts[ FONT_LUCIDA_CONSOLE_7PX ] = m_fonts[ FONT_VERDANA_7PX ];
		m_fonts[ FONT_MARLETT_45PX ] = m_fonts[ FONT_VERDANA_7PX ];
		m_fonts[ FONT_ENHANCE_9PX ] = m_fonts[ FONT_VERDANA_7PX ];

		/*m_fonts[ FONT_VERDANA_7PX ] = OSHGui::Drawing::FontManager::LoadFont( "Verdana", 7.f, true );
		m_fonts[ FONT_VERDANA_BOLD_7PX ] = OSHGui::Drawing::FontManager::LoadFont( "Verdana Bold", 7.f, true );
		m_fonts[ FONT_04B03_6PX ] = OSHGui::Drawing::FontManager::LoadFontFromMemory( OSHGui::Misc::RawDataContainer( pixel_type_font ), 6.f, false );
		m_fonts[ FONT_LUCIDA_CONSOLE_7PX ] = OSHGui::Drawing::FontManager::LoadFont( "Lucida Console", 7.5f, false );
		m_fonts[ FONT_MARLETT_45PX ] = OSHGui::Drawing::FontManager::LoadFontFromMemory( OSHGui::Misc::RawDataContainer( marlett_font ), 45.f, true );
		m_fonts[ FONT_ENHANCE_9PX ] = OSHGui::Drawing::FontManager::LoadFontFromMemory( OSHGui::Misc::RawDataContainer( enhance_font ), 9.f, true );*/

		ImGuiFreeType::BuildFontAtlas( io.Fonts, 0 );
	}
}

void c_renderer::start_drawing( ) {
	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	ImGui::SetNextWindowPos( ImVec2( 0.f, 0.f ), ImGuiSetCond_Always );
	ImGui::SetNextWindowSize( ImVec2( ImGui::GetIO( ).DisplaySize.x, ImGui::GetIO( ).DisplaySize.y ), ImGuiSetCond_Always );

	ImGui::Begin( 
		"##back_buffer",
		nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus 
	);
}

void c_renderer::end_drawing( ) {
	ImGui::GetWindowDrawList( )->PushClipRectFullScreen( );
	ImGui::End( );
}

void c_renderer::render( ) {
	ImGui::Render( );
	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
}

void c_renderer::rect( const ImU32 &color, float x, float y, float width, float height ) const {
	ImGui::GetWindowDrawList( )->AddRect( { x, y }, { x + width, y + height }, color );
}

void c_renderer::filled_rect( const ImU32 &color, float x, float y, float width, float height ) const {
	ImGui::GetWindowDrawList( )->AddRectFilled( { x, y }, { x + width, y + height }, color );
}

void c_renderer::ansi_text( ImFont *font, const ImU32 &color, const ImU32 &shadow_color, float x, float y, int flags, const std::string str, ... ) const {
	if( !font ) {
		return;
	}
	
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

	if( flags & CENTERED_X || flags & CENTERED_Y ) {
		const ImVec2 text_size = font->CalcTextSizeA( font->FontSize, FLT_MAX, 0.f, buf.c_str( ) );

		if( flags & CENTERED_X )
			x -= static_cast< int >( text_size.x / 2.f );

		if( flags & CENTERED_Y )
			y -= static_cast< int >( text_size.y / 2.f );
	}

	if( flags & DROPSHADOW )
		ImGui::GetWindowDrawList( )->AddText( font, font->FontSize, { x + 1.f, y + 1.f }, shadow_color, buf.c_str( ) );

	if( flags & OUTLINED ) {
		ImGui::GetWindowDrawList( )->AddText( font, font->FontSize, { x, y + 1.f }, shadow_color, buf.c_str( ) );
		ImGui::GetWindowDrawList( )->AddText( font, font->FontSize, { x, y - 1.f }, shadow_color, buf.c_str( ) );
		ImGui::GetWindowDrawList( )->AddText( font, font->FontSize, { x + 1.f, y }, shadow_color, buf.c_str( ) );
		ImGui::GetWindowDrawList( )->AddText( font, font->FontSize, { x - 1.f, y }, shadow_color, buf.c_str( ) );
	}

	ImGui::GetWindowDrawList( )->AddText( font, font->FontSize, { x, y }, color, buf.c_str( ) );
}

void c_renderer::line( const ImU32 &color, float x, float y, float x1, float y1 ) const {
	ImGui::GetWindowDrawList( )->AddLine( { x, y }, { x1, y1 }, color );
}

void c_renderer::circle( const ImU32 &color, float x, float y, float radius ) const {
	ImGui::GetWindowDrawList( )->AddCircle( { x, y }, radius, color );
}

ImFont *c_renderer::get_font( int index ) {
	return m_fonts[ index ];
}

bool get_system_font_path( const std::string &font_name, std::string &font_path ) {
	char buffer [ MAX_PATH ];

	if( !GetWindowsDirectoryA( buffer, MAX_PATH ) ) {
		return false;
	}

	std::string fonts_directory( buffer );
	fonts_directory.append( "\\Fonts\\" );

	HKEY registry_key;

	if( RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &registry_key ) ) {
		return false;
	}

	uint32_t index = 0;

	char value_name [ MAX_PATH ];
	uint8_t value_data [ MAX_PATH ];

	do {
		uint32_t value_name_size = MAX_PATH;
		uint32_t value_data_size = MAX_PATH;
		uint32_t value_type;

		const LSTATUS error = RegEnumValueA(
			registry_key,
			index,
			value_name,
			reinterpret_cast< DWORD * >( &value_name_size ),
			nullptr,
			reinterpret_cast< DWORD * >( &value_type ),
			value_data,
			reinterpret_cast< DWORD * >( &value_data_size )
		);

		index++;

		if( error == ERROR_NO_MORE_ITEMS ) {
			RegCloseKey( registry_key );
			return false;
		}

		if( error || value_type != REG_SZ ) {
			continue;
		}

		if( !_strnicmp( font_name.data( ), value_name, font_name.size( ) ) ) {
			font_path = fonts_directory + std::string( reinterpret_cast< char * >( value_data ), value_data_size );
			RegCloseKey( registry_key );
			return true;
		}
	} while ( true );
}