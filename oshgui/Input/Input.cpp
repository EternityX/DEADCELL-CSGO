/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "Input.hpp"
#include "../Application.hpp"

namespace OSHGui {
	namespace Input {
		Input::Input()
			: enableMouseInput( true ),
			  enableKeyboardInput( true ) { }

		//---------------------------------------------------------------------------
		void Input::SetMouseInputEnabled( bool enable ) {
			enableMouseInput = enable;
		}

		//---------------------------------------------------------------------------
		void Input::SetKeyboardInputEnabled( bool enable ) {
			enableKeyboardInput = enable;
		}

		//---------------------------------------------------------------------------
		bool Input::InjectMouseMessage( const MouseMessage &mouse ) {
			return Application::Instance().ProcessMouseMessage( mouse );
		}

		//---------------------------------------------------------------------------
		bool Input::InjectKeyboardMessage( const KeyboardMessage &keyboard ) {
			return Application::Instance().ProcessKeyboardMessage( keyboard );
		}

		//---------------------------------------------------------------------------
	}
}
