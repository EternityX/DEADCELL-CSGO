#include "../inc.h"

#include "tabs/tab_visuals.h"

c_menu::c_menu( )
	: m_is_active{ true } { }

void c_menu::init( IDirect3DDevice9 * device )
{
	m_device = device;

	/// No need for this here because it's already done in renderer
	/*ImGui::CreateContext();
	ImGui_ImplWin32_Init( g_input.get_window_handle() );
	ImGui_ImplDX9_Init( m_device );*/

	ui::setup_style();
}

void c_menu::render()
{
	if ( !m_is_active )
		return;

	if ( ImGui::Begin( "looooooooooooooooool", &is_active(), ImVec2( 997.0f, 600.0f ), 1.0f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar ) )
	{
		ui::window_bar();
		ui::tabs();

		switch ( m_current_tab )
		{
			case TAB_AIMBOT:
				break;
			case TAB_VISUALS:
				ui::tab::visuals();
				break;
			case TAB_MISC:
				break;
			case TAB_SKINS:
				break;
			case TAB_DEBUG:
				break;
		}

		ImGui::End();
	}

}

bool& c_menu::is_active()
{
	return m_is_active;
}

int & c_menu::get_active_tab()
{
	return m_current_tab;
}
