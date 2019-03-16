/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include "Label.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label()
		: textHelper_( GetFont() ) {
		type_ = ControlType::Label;

		SetAutoSize( true );

		ApplyStyle( Application::Instance().GetStyle() );

		canRaiseEvents_ = false;
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText( const Misc::AnsiString &text ) {
		textHelper_.SetText( text );
		if( autoSize_ ) {
			Control::SetSize( textHelper_.GetSize() );
		}
	}

	void Label::SetStyle( int style ) {
		style_ = style;
	}

	//---------------------------------------------------------------------------
	const Misc::AnsiString &Label::GetText() const {
		return textHelper_.GetText();
	}

	//---------------------------------------------------------------------------
	void Label::SetFont( const Drawing::FontPtr &font ) {
		Control::SetFont( font );

		textHelper_.SetFont( font );
		if( autoSize_ ) {
			Control::SetSize( textHelper_.GetSize() );
		}
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Label::Intersect( const Drawing::PointI &point ) const {
		return false;
	}

	//---------------------------------------------------------------------------
	void Label::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		if( GetBackColor().GetAlpha() > 0 ) {
			g.FillRectangle( GetBackColor(), RectangleF( PointF(), GetSize() ) );
		}

		if( style_ == 1 )
			g.DrawString( textHelper_.GetText(), GetFont(), Color::FromARGB( 200, 8, 8, 8 ), PointF( 1, 1 ) );

		if( style_ == 2 ) {
			g.DrawString( textHelper_.GetText(), GetFont(), Color::FromARGB( 200, 8, 8, 8 ), PointF( 0, 1 ) );
			g.DrawString( textHelper_.GetText(), GetFont(), Color::FromARGB( 200, 8, 8, 8 ), PointF( 0, -1 ) );
			g.DrawString( textHelper_.GetText(), GetFont(), Color::FromARGB( 200, 8, 8, 8 ), PointF( 1, 0 ) );
			g.DrawString( textHelper_.GetText(), GetFont(), Color::FromARGB( 200, 8, 8, 8 ), PointF( -1, 0 ) );
		}

		g.DrawString( textHelper_.GetText(), GetFont(), GetForeColor(), PointF( 0, 0 ) );
	}

	//---------------------------------------------------------------------------
}
