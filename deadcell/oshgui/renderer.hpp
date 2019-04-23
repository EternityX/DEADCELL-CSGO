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
		OSHGui::Drawing::RenderTargetPtr m_render_target;
		OSHGui::Drawing::GeometryBufferPtr m_geometry;
		std::vector< OSHGui::Drawing::FontPtr > m_fonts;

		c_renderer();
		void init( IDirect3DDevice9Ex *device );

		void start_drawing( );
		void end_drawing( ) const;

		// drawing.
		void rect( const OSHGui::Drawing::Color &color, int x, int y, int width, int height ) const;
		void filled_rect(const OSHGui::Drawing::Color & color, int x, int y, int width, int height) const;
		void ansi_text( const OSHGui::Drawing::FontPtr &font, const OSHGui::Drawing::Color &color, const OSHGui::Drawing::Color &shadow_color, int x, int y, int flags, const std::string str, ...) const;
		void filled_rect_gradient( const OSHGui::Drawing::ColorRectangle &colors, int x, int y, int width, int height ) const;
		void line(const OSHGui::Drawing::Color & color, float x, float y, float x1, float y1) const;
		void circle( const OSHGui::Drawing::Color &color, int x, int y, int radius ) const;
		OSHGui::Drawing::FontPtr get_font(int index);
		OSHGui::Application * get_instance() const;
		OSHGui::Drawing::Renderer & get_renderer() const;
	};
}
