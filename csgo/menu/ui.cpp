#include "../inc.h"

ImColor ui::accent_primary = ImColor( 255, 95, 33, 255 );
ImColor ui::grey24 = ImColor( 24, 24, 24, 255 );
ImColor ui::grey28 = ImColor( 28, 28, 28, 255 );
ImColor ui::grey32 = ImColor( 32, 32., 32, 255 );

void ui::setup_style()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ ImGuiCol_Text ] = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
	style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( 1.0f, 1.0f, 1.0f, 0.6f );
	style.Colors[ ImGuiCol_Border ] = grey32;
	style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 18.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f );
	style.Colors[ ImGuiCol_Button ] = grey24;
	style.Colors[ ImGuiCol_ButtonHovered ] = grey28;
	style.Colors[ ImGuiCol_ButtonActive ] = grey32;
	style.Colors[ ImGuiCol_SliderGrab ] = accent_primary;
	style.Colors[ ImGuiCol_SliderGrabActive ] = accent_primary;
	style.Colors[ ImGuiCol_FrameBg ] = grey24;
	style.Colors[ ImGuiCol_FrameBgHovered ] = grey28;
	style.Colors[ ImGuiCol_FrameBgActive ] = grey32;
	style.Colors[ ImGuiCol_ColumnActive ] = accent_primary;
	style.Colors[ ImGuiCol_Header ] = accent_primary;
	style.Colors[ ImGuiCol_HeaderActive ] = grey28;
	style.Colors[ ImGuiCol_HeaderHovered ] = grey32;
	style.Colors[ ImGuiCol_PopupBg ] = grey28;
	style.Colors[ ImGuiCol_MenuBarBg ] = grey24;
	style.Colors[ ImGuiCol_ScrollbarBg ] = grey24;
	style.Colors[ ImGuiCol_ScrollbarGrab ] = accent_primary;
	style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = grey32;
	style.Colors[ ImGuiCol_ScrollbarGrabActive ] = accent_primary;

	style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2( 5.0f, 2.0f );
	style.ScrollbarSize = 7.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 1.0f;
	style.ItemInnerSpacing.x = 5.0f;
	style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );
	style.WindowPadding = ImVec2( 8.f, 12.f );
}

void ui::tabs()
{
	static std::vector<c_menu::tab_t> tabs
	{
		{ "Aimbot", ICON_FA_ANGRY, c_menu::TAB_AIMBOT },
		{ "Visuals", ICON_FA_EYE, c_menu::TAB_VISUALS },
		{ "Miscellaneous", ICON_FA_GRIN_TEARS, c_menu::TAB_MISC },
		{ "Skins", ICON_FA_GEM, c_menu::TAB_SKINS },
		{ "Debug", ICON_FA_SAVE, c_menu::TAB_DEBUG }
	};

	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 3.f, 3.f } );
	{
		const auto window_padding = ImGui::GetStyle().WindowPadding.x;
		const auto item_spacing = ImGui::GetStyle().ItemSpacing.x;

		const auto tab_size =
			ImVec2( ( ImGui::GetWindowSize().x - window_padding * 2 - item_spacing * ( tabs.size() - 1 ) ) / tabs.size(),
					40 );

		for ( auto tab : tabs )
		{
			const bool is_selected = g_menu.get_active_tab() == tab.m_index;

			if ( ui::tab_button( tab.m_name, tab.m_icon, is_selected, tab_size ) )
				g_menu.get_active_tab() = tab.m_index;

			ImGui::SameLine();
		}

		/// Reset line
		ImGui::NewLine();
	} ImGui::PopStyleVar();

	ImGui::Spacing();
	ImGui::Spacing();
}

void ui::window_bar()
{
	const auto window = ImGui::GetCurrentWindow();
	ImGui::PopClipRect();
	{
		g_renderer.filled_rect( ui::accent_primary, window->Pos.x, window->Pos.y, window->Size.x, 2.f );
	} ImGui::PushClipRect( { window->Pos.x, window->Pos.y },
						   { window->Pos.x + window->Size.x, window->Pos.y + window->Size.y }, false );
}

bool ui::tab_button( const char * label, const char * icon, bool is_selected, const ImVec2 & size_arg, int flags )
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if ( window->SkipItems )
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID( label );
	const ImVec2 label_size = ImGui::CalcTextSize( label, NULL, true );

	ImVec2 pos = window->DC.CursorPos;
	if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

	const ImRect bb( pos, pos + size );
	ImGui::ItemSize( size, style.FramePadding.y );
	if ( !ImGui::ItemAdd( bb, id ) )
		return false;

	if ( window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat )
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior( bb, id, &hovered, &held, flags );
	if ( pressed )
		ImGui::MarkItemEdited( id );

	// Render
	const ImU32 col = ImGui::GetColorU32( ( held && hovered ) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button );
	ImGui::RenderNavHighlight( bb, id );
	ImGui::RenderFrame( bb.Min, bb.Max, col, true, style.FrameRounding );

	if ( is_selected )
	{
		window->DrawList->AddRectFilled( ImVec2( bb.Min.x, bb.Min.y ), ImVec2( bb.Min.x + 5, bb.Min.y + size_arg.y ), ImColor( accent_primary.Value.x, accent_primary.Value.y, accent_primary.Value.z, pulse( 1.20f ) + 0.15f ), 0, -1 );
	}

	/// icon
	ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.2f, 0.2f, 0.2f, 0.2f ) );
	ImGui::RenderTextClipped( ImVec2( bb.Min.x + style.FramePadding.x + 5, bb.Min.y - 1 ), bb.Max - style.FramePadding - ImVec2( 20, -25 ), icon, nullptr, &label_size, { 0.5f, 0.53f }, &bb );
	ImGui::PopStyleColor();
	
	/// label
	ImGui::RenderTextClipped( bb.Min, bb.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb );

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	IMGUI_TEST_ENGINE_ITEM_INFO( id, label, window->DC.LastItemStatusFlags );
	return pressed;
}

bool ui::checkbox( const char * label, bool * v )
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if ( window->SkipItems )
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID( label );
	const ImVec2 label_size = ImGui::CalcTextSize( label, NULL, true );

	const float square_sz = ImGui::GetFrameHeight();
	const ImVec2 pos = window->DC.CursorPos;
	const ImRect total_bb( pos, pos + ImVec2( square_sz + ( label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f ), label_size.y + style.FramePadding.y * 2.0f ) );
	ImGui::ItemSize( total_bb, style.FramePadding.y );
	if ( !ImGui::ItemAdd( total_bb, id ) )
		return false;

	/// TODO: fix piece of shit below

	static float timeBegin = -1.f;
	static ImGuiID timeID = 0;
	static const float timeActionLength = 0.40f;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior( total_bb, id, &hovered, &held );
	if ( pressed )
	{
		*v = !( *v );
		if ( timeID == id )
		{
			// Fine tuning for the case when user clicks on the same checkbox twice quickly
			float elapsedTime = g_csgo.m_global_vars->m_cur_time - timeBegin;
			if ( elapsedTime > timeActionLength ) timeBegin = g_csgo.m_global_vars->m_cur_time;   // restart
			else
			{
				// We must invert the time, tweaking timeBegin
				const float newElapsedTime = timeActionLength - elapsedTime;
				timeBegin = g_csgo.m_global_vars->m_cur_time - newElapsedTime;
			}
		}
		else
		{
			timeID = id;
			timeBegin = g_csgo.m_global_vars->m_cur_time;
		}

		ImGui::MarkItemEdited( id );
	}

	float t = 0.f;    // In [0,1] 0 = OFF 1 = ON
	bool animationActive = false;
	if ( timeID == id )
	{
		float elapsedTime = g_csgo.m_global_vars->m_cur_time - timeBegin;
		if ( elapsedTime > timeActionLength ) { timeBegin = -1; timeID = 0; }
		else
		{
			t = 1 - elapsedTime / timeActionLength;
			animationActive = t > 0;
		}
	}
	//if (*v) t = 1.f-t;
	if ( t < 0 ) t = 0;
	else if ( t > 1 ) t = 1;

	float radius = 5.0f / t;
	if ( radius > 10.0f )
		radius = 10.0f;

	const ImRect check_bb( pos, pos + ImVec2( square_sz, square_sz ) );
	ImGui::RenderNavHighlight( total_bb, id );

	if ( *v )
		window->DrawList->AddCircleFilled( { check_bb.GetCenter().x - 3, check_bb.GetCenter().y - 1 }, radius, ImColor( accent_primary.Value.x, accent_primary.Value.y, accent_primary.Value.z, t ), 40 );

	if ( hovered )
	{
		if ( !*v )
			window->DrawList->AddCircleFilled( { check_bb.GetCenter( ).x - 3, check_bb.GetCenter( ).y - 1 }, 10.0f, ImColor( 1.0f, 1.0f, 1.0f, 0.10f ), 40 );
			else
				window->DrawList->AddCircleFilled( { check_bb.GetCenter( ).x - 3, check_bb.GetCenter( ).y - 1 }, 10.0f, ImColor( accent_primary.Value.x, accent_primary.Value.y, accent_primary.Value.z, 0.10f ), 40 );
	}
	if ( held && !pressed )
	{
		if ( !*v )
			window->DrawList->AddCircleFilled( { check_bb.GetCenter( ).x - 3, check_bb.GetCenter( ).y - 1 }, 10.0f, ImColor( 1.0f, 1.0f, 1.0f, 0.2f ), 40 );
			else
				window->DrawList->AddCircleFilled( { check_bb.GetCenter( ).x - 3, check_bb.GetCenter( ).y - 1 }, 10.0f, ImColor( accent_primary.Value.x, accent_primary.Value.y, accent_primary.Value.z, 0.2f ), 40 );
	}



	//if ( animationActive )


	if ( *v )
	{
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( accent_primary.Value.x, accent_primary.Value.y, accent_primary.Value.z, 1.f ) );
		ImGui::RenderTextClipped( ImVec2( check_bb.Min.x, check_bb.Min.y + 1 ), check_bb.Max - style.FramePadding, ICON_FA_CHECK_SQUARE, nullptr, &label_size, ImVec2( 0, 0 ), &check_bb );
		ImGui::PopStyleColor( 1 );
	}
	else
	{
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 170 / 255.f, 170 / 255.f, 170 / 255.f, 255 ) );
		ImGui::RenderTextClipped( ImVec2( check_bb.Min.x, check_bb.Min.y + 1 ), check_bb.Max - style.FramePadding, ICON_FA_SQUARE, nullptr, &label_size, ImVec2( 0, 0 ), &check_bb );
		ImGui::PopStyleColor( 1 );
	}

	if ( label_size.x > 0.0f )
		ImGui::RenderText( ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y ), label );

	/*ImGui::RenderFrame( check_bb.Min, check_bb.Max, ImGui::GetColorU32( ( held && hovered ) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg ), true, style.FrameRounding );
	if ( *v )
	{
		const float pad = ImMax( 1.0f, ( float )( int )( square_sz / 6.0f ) );
		ImGui::RenderCheckMark( check_bb.Min + ImVec2( pad, pad ), ImGui::GetColorU32( ImGuiCol_CheckMark ), square_sz - pad * 2.0f );
	}*/

	if ( g.LogEnabled )
		ImGui::LogRenderedText( &total_bb.Min, *v ? "[x]" : "[ ]" );

	IMGUI_TEST_ENGINE_ITEM_INFO( id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | ( *v ? ImGuiItemStatusFlags_Checked : 0 ) );
	return pressed;
}

float ui::pulse( const float frequency )
{
	return 0.5f * ( 1 + sin( 1.0f * math::pi *frequency * g_csgo.m_global_vars->m_cur_time ) );
}
