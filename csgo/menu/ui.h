#pragma once

namespace ui
{
	void setup_style();

	void tabs();
	void window_bar();

	bool tab_button( const char* label, const char* icon, bool is_selected, const ImVec2& size_arg, int flags = 0 );
	bool checkbox( const char* label, bool* v );

	float pulse( const float frequency );

	extern ImColor accent_primary ;
	extern ImColor grey24;
	extern ImColor grey28;
	extern ImColor grey32;
}