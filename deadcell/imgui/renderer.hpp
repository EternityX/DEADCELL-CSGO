#pragma once

namespace renderer {
	enum : uint32_t {
		FONT_VERDANA_7PX = 0,
		FONT_VERDANA_BOLD_7PX,
		FONT_04B03_6PX,
		FONT_PIXELTYPE_7PX,
		FONT_LUCIDA_CONSOLE_7PX,
		FONT_MARLETT_45PX,
		FONT_ENHANCE_9PX,
		FONT_MAX
	};

	enum : uint32_t {
		NO_FLAGS = 0,
		CENTERED_X = 1,
		CENTERED_Y = 2,
		OUTLINED = 4,
		DROPSHADOW = 8
	};

	class c_renderer {
	public:
		std::vector< ImFont * > m_fonts;

		c_renderer();
		void init( IDirect3DDevice9Ex *device );

		void start_drawing( );
		void end_drawing( );
		void render( );

		// drawing.
		void rect( const ImU32 &color, float x, float y, float width, float height ) const;
		void filled_rect( const ImU32 &color, float x, float y, float width, float height) const;
		void ansi_text( ImFont *font, const ImU32 &color, const ImU32 &shadow_color, float x, float y, int flags, const std::string str, ...) const;
		void line( const ImU32 &color, float x, float y, float x1, float y1) const;
		void circle( const ImU32 &color, float x, float y, float radius ) const;
		ImFont *get_font( int index );
	};
}
