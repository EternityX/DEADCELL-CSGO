#include "tab_visuals.h"
#include "../../inc.h"

void ui::tab::visuals()
{
	if ( ImGui::BeginChild( "visuals", { -1, -1 }, true ) )
	{
		ui::checkbox( "Checkbox check", &g_vars.misc.air_strafe );
		ui::checkbox( "Checkbox check1", &g_vars.misc.bhop );
		ui::checkbox( "Checkbox check2", &g_vars.misc.dangerzone_menu);
		ui::checkbox( "Checkbox check3", &g_vars.misc.bullet_impacts );
		ImGui::SliderInt( "ff", &g_vars.misc.bullet_impacts_duration, 0, 100 );
		ImGui::Combo( "Combooo", &g_vars.misc.client_hitboxes_duration, "cool\0cool\0cooooool\0\0" );

		ImGui::EndChild();
	}
}
