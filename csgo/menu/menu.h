#pragma once
#include "../inc.h"

class c_menu {
private:
	// x axis pos.
	int m_control_x_pos;
	// y axis pos.
	int m_control_y_pos;

	OSHGui::Drawing::Color m_primary_colour;
public:
	std::shared_ptr< OSHGui::Form > m_form;
	std::shared_ptr< OSHGui::Form > m_dangerzone_form;
	std::shared_ptr< OSHGui::Form > m_chaiscript_form;
	std::string m_selected_sound_text;

	c_menu( );
	void init( );

	void set_x_pos( int x );
	void set_y_pos( int y );

	int get_x_pos( ) const;
	int get_y_pos( ) const;

	void push_x_pos( int x );
	void push_y_pos( int y );
};

extern c_menu g_menu;

namespace controls {
	using namespace OSHGui::Drawing;
	using namespace OSHGui::Misc;

	class c_groupbox : public OSHGui::GroupBox {
	private:
	public:
		c_groupbox( const AnsiString &text, int x, int y, int w, int h );
	};

	class c_checkbox : public OSHGui::CheckBox {
	private:
		void init( const AnsiString &text, int x, int y, Control *parent, bool *cvar );
	public:
		c_checkbox( const AnsiString &text, int x, int y, Control *parent, bool *cvar );
		c_checkbox( const AnsiString &text, Control *parent, bool *cvar );
	};


	class c_text_box : public OSHGui::TextBox {
	private:
		void init( const AnsiString &text, int x, int y, Control *parent ) {
			SetFont( g_renderer.m_fonts.at( 0 ) );
			SetLocation( x, y );
			//SetSize( 220, 30 );
			// SetText( text );

			parent->AddControl( this );
		}
	public:
		c_text_box( const AnsiString &text, int x, int y, Control *parent ) {
			init( text, x, y, parent );
		}

		// automatic positioning.
		c_text_box( const AnsiString &text, Control *parent ) {
			// set size here.
			Control::SetSize( 160, 20 );

			init( text, parent->GetWidth() / 2 - Control::GetWidth() / 2 - 3, g_menu.get_y_pos( ), parent );
			g_menu.push_y_pos( 28 );
		}
	};

	class c_label : public OSHGui::Label {
	private:
		void init( const AnsiString &text, int x, int y, Control *parent, const FontPtr &font, int style, OSHColor colour );
	public:
		c_label( const AnsiString &text, int x, int y, Control *parent, const FontPtr &font, int style, OSHColor colour );
		c_label( const AnsiString &text, int x, int y, Control *parent );
		c_label( const AnsiString &text, Control *parent, const FontPtr &font, int style, OSHColor colour );
		c_label( const AnsiString &text, Control *parent );
	};

	class c_combo : public OSHGui::ComboBox {
	private:
		void init( const AnsiString &text, std::vector< AnsiString > items, int x, int y, Control *parent, int max_items, int *cvar );
	public:
		c_combo( const AnsiString &text, const std::vector< AnsiString > &items, int x, int y, Control *parent, int max_items, int *cvar );
		c_combo( const AnsiString &text, const std::vector< AnsiString > &items, Control *parent, int max_items, int *cvar, int parent_width );
	};

	class c_sound_combo : public OSHGui::ComboBox {
	private:
		void init( const AnsiString &text, std::vector<AnsiString> items, int x, int y, Control *parent, int max_items, int *cvar );
	public:
		c_sound_combo( const AnsiString &text, const std::vector<AnsiString> &items, int x, int y, Control *parent, int max_items, int *cvar );
		c_sound_combo( const AnsiString &text, const std::vector<AnsiString> &items, Control *parent, int max_items, int *cvar, int parent_width );
	};

	class c_slider : public OSHGui::TrackBar {
	private:
		void init( const AnsiString &text, int x, int y, Control *parent, float min, float max, float *value, int precision, float default_value, std::string append_text );
		void init( const AnsiString &text, int x, int y, Control *parent, float min, float max, int *value, int default_value, std::string append_text );
	public:
		c_slider( const AnsiString &text, int x, int y, Control *parent, float min, float max, int *value, int default_value, std::string append_text = "" );
		c_slider( const AnsiString &text, int x, int y, Control *parent, float min, float max, float *value, int precision, float default_value, std::string append_text = "" );
		c_slider( const AnsiString &text, Control *parent, float min, float max, int *value, int default_value, std::string append_text = "" );
		c_slider( const AnsiString &text, Control *parent, float min, float max, float *value, int precision, float default_value, std::string append_text = "" );
	};

	class c_colorpicker : public OSHGui::ColorButton {
	private:
		void init( int x, int y, Control *parent, float *col );
	public:
		c_colorpicker( Control *parent, Control *control, float *col );
	};

	class c_button : public OSHGui::Button {
	private:
		void init( const AnsiString &text, int x, int y, Control *parent ) {
			SetForeColor( OSHColor::FromARGB( 255, 201, 201, 201 ) );
			SetFont( g_renderer.m_fonts.at( 0 ) );
			SetLocation( x, y );
			//SetSize( 220, 30 );
			SetText( text );

			// I'm not sure how to pass a lambda as a function parameter.
			// so we'll just have to call AddControl manually.
			// parent->AddControl( this );
		}
	public:
		c_button( const AnsiString &text, int x, int y, Control *parent ) {
			init( text, x, y, parent );
		}

		// automatic positioning.
		c_button( const AnsiString &text, Control *parent ) {
			// set size here.
			Control::SetSize( 160, 20 );

			init( text, parent->GetWidth() / 2 - Control::GetWidth() / 2 - 3, g_menu.get_y_pos( ), parent );
			g_menu.push_y_pos( 28 );
		}
	};

	class c_hotkey : public OSHGui::HotkeyControl {
	private:
		void init( const AnsiString &text, int x, int y, Control *parent, int *cvar );
	public:
		c_hotkey( const AnsiString &text, Control *parent, int *cvar, int parent_width );
	};
}

class c_main_form : public OSHGui::Form {
private:
	std::vector< std::shared_ptr< OSHGui::TabPage > > m_pages;
	OSHGui::TabControl *m_tab_control{ };

	// tab pages.
	enum Pages {
		PAGE_RAGEBOT,
		PAGE_VISUALS,
		PAGE_MISC,
		PAGE_CONFIG,
		PAGE_PLAYERLIST,
		PAGE_MAX
	};

	// initialize all controls.
	void init_component( );
	void init_tabs( );

	void playerlist_tab( );
	void ragebot_tab( );
	void misc_tab( );
	void visuals_tab( );
	void config_tab();
public:
	c_main_form( ) {
		init_component( );
	}
};

class c_dangerzone_form : public OSHGui::Form {
private:
	std::vector< std::shared_ptr< OSHGui::TabPage > > m_pages;
	OSHGui::TabControl *m_tab_control{ };

	// tab pages.
	enum Pages {
		PAGE_VISUALS,
		PAGE_MISC,
		PAGE_MAX
	};

	// initialize all controls.
	void init_component( );
	void init_tabs( );

	void misc_tab( );
	void visuals_tab( );
public:
	c_dangerzone_form( ) {
		init_component( );
	}
};

class c_chaiscript_form : public OSHGui::Form {
private:
	void update_label();
	// initialize all controls.
	void init_component();
	void init_controls();
	void submit_code();
public:
	OSHGui::Label* label_console_pos = new OSHGui::Label();
	OSHGui::TextBox* text_input = new OSHGui::TextBox();
	c_chaiscript_form() {
		init_component();
	}
};