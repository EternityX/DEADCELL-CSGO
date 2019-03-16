/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#pragma once

namespace OSHGui
{
	/**
	 * Auflistung der Arten von Steuerelementen.
	 */
	enum class ControlType
	{
		None = 0,

		Button = 1,
		CheckBox,
		ColorBar,
		ColorPicker,
		ComboBox,
		Form,
		GroupBox,
		HotkeyControl,
		Label,
		LinkLabel,
		ListBox,
		Panel,
		PictureBox,
		ProgressBar,
		RadioButton,
		ScrollBar,
		TabControl,
		TabPage,
		TextBox,
		Timer,
		TrackBar,
		ComboList
	};
}
