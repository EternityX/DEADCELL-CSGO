/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "IBeam.hpp"
#include "../Drawing/Graphics.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void IBeamCursor::Initialize() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		const PointF offset( -4, -7 );

		auto color2 = Color::FromARGB( 255, 211, 211, 211 );

		g.FillRectangle( color2, PointF( 0, 0 ) + offset, SizeF( 7, 1 ) );
		g.FillRectangle( color2, PointF( 0, 13 ) + offset, SizeF( 7, 1 ) );
		g.FillRectangle( color2, PointF( 3, 1 ) + offset, SizeF( 1, 12 ) );
	}

	//---------------------------------------------------------------------------
}
