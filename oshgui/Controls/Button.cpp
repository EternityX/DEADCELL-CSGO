/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include "Button.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI Button::DefaultSize( 92, 24 );
	const Drawing::PointI Button::DefaultLabelOffset( 6, 5 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Button::Button()
		: label_( new Label() ) {
		type_ = ControlType::Button;

		label_->SetLocation( DefaultLabelOffset );
		label_->SetBackColor( Drawing::Color::Empty() );
		label_->SetForeColor( Drawing::Color( 255, 201, 201, 201 ) );

		SetSize( DefaultSize );

		ApplyStyle( Application::Instance().GetStyle() );
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Button::SetAutoSize( bool autoSize ) {
		Control::SetAutoSize( autoSize );

		label_->SetAutoSize( autoSize );
	}

	//---------------------------------------------------------------------------
	void Button::SetSize( const Drawing::SizeI &size ) {
		Control::SetSize( size );

		CalculateLabelLocation();
	}

	//---------------------------------------------------------------------------
	void Button::SetText( const Misc::AnsiString &text ) {
		label_->SetText( text );
		if( autoSize_ ) {
			SetSize( label_->GetSize().InflateEx( DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2 ) );
		}

		CalculateLabelLocation();
	}

	//---------------------------------------------------------------------------
	const Misc::AnsiString &Button::GetText() const {
		return label_->GetText();
	}

	//---------------------------------------------------------------------------
	void Button::SetFont( const Drawing::FontPtr &font ) {
		Control::SetFont( font );

		label_->SetFont( font );
		if( autoSize_ ) {
			SetSize( label_->GetSize().InflateEx( DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2 ) );
		}
	}

	//---------------------------------------------------------------------------
	void Button::SetForeColor( const Drawing::Color &color ) {
		Control::SetForeColor( color );

		label_->SetForeColor( Drawing::Color( 255, 201, 201, 201 ) );
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Button::CalculateAbsoluteLocation() {
		Control::CalculateAbsoluteLocation();

		label_->SetParent( this );
	}

	//---------------------------------------------------------------------------
	void Button::CalculateLabelLocation() {
		label_->SetLocation( Drawing::PointI( GetSize().Width / 2.f - label_->GetSize().Width / 2.f,
		                                      GetSize().Height / 2.f - label_->GetSize().Height / 2.f ) );
	}

	//---------------------------------------------------------------------------
	void Button::DrawSelf( Drawing::RenderContext &context ) {
		Control::DrawSelf( context );

		label_->Render();
	}

	//---------------------------------------------------------------------------
	void Button::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		auto color = isFocused_ || isInside_ ? Color::FromARGB( 255, 46, 46, 54 ) : Color::FromARGB( 255, 41, 41, 49 );

		if( isClicked_ )
			color = Color::FromARGB( 255, 56, 56, 64 );

		g.FillRectangle( Color::FromARGB( 255, 18, 18, 24 ), PointF( 0, 0 ), GetSize() );
		g.FillRectangleGradient( ColorRectangle( color, Color::FromARGB( 255, 36, 36, 43 ) ), PointF( 1, 1 ),
		                         GetSize() - SizeF( 2, 2 ) );
	}

	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool Button::OnKeyUp( const KeyboardMessage &keyboard ) {
		if( !Control::OnKeyUp( keyboard ) ) {
			if( keyboard.GetKeyCode() == Key::Return ) {
				clickEvent_.Invoke( this );
			}
		}

		return true;
	}

	//---------------------------------------------------------------------------
}
