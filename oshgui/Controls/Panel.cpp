/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "Panel.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI Panel::DefaultSize( 200, 200 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Panel::Panel() {
		type_ = ControlType::Panel;

		SetSize( DefaultSize );

		ApplyStyle( Application::Instance().GetStyle() );

		isFocusable_ = false;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Panel::PopulateGeometry() {
		using namespace Drawing;

		if( GetBackColor().GetAlpha() > 0 ) {
			Graphics g( *geometry_ );
			
			g.FillRectangle( GetBackColor() - Color::FromARGB( 0, 100, 100, 110 ), GetBounds() );
			const auto color = GetBackColor() - Color::FromARGB( 0, 90, 90, 100 );
			g.FillRectangleGradient( ColorRectangle( GetBackColor(), GetBackColor(), color, color ),
			                         RectangleF( GetLocation() + PointF( 1, 1 ), GetSize() - SizeF( 2, 2 ) ) );
		}
	}

	//---------------------------------------------------------------------------
}
