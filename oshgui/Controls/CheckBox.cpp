/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include "CheckBox.hpp"
#include "Label.hpp"
#include "../Misc/Exceptions.hpp"
#include "Timer.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::PointI CheckBox::DefaultLabelOffset( 20, -1 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox()
		: checked_( false ),
		  label_( new Label() ) {
		type_ = ControlType::CheckBox;

		SetSize( DefaultCheckBoxSize, DefaultCheckBoxSize );
		SetAutoSize( true );

		label_->SetLocation( DefaultLabelOffset );
		label_->SetBackColor( Drawing::Color::Empty() );
		label_->SetStyle( 1 );

		ApplyStyle( Application::Instance().GetStyle() );
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked( bool checked ) {
		if( checked_ != checked ) {
			checked_ = checked;

			checkedChangedEvent_.Invoke( this );

			Invalidate();
		}
	}

	//---------------------------------------------------------------------------
	bool CheckBox::GetChecked() const {
		return checked_;
	}

	//---------------------------------------------------------------------------
	void CheckBox::SetText( const Misc::AnsiString &text ) {
		const auto offset = label_->GetLocation();

		label_->SetText( text );
		if( autoSize_ ) {
			size_ = label_->GetSize().InflateEx( offset.Left, offset.Top );
			if( size_.Height < DefaultCheckBoxSize ) {
				size_.Height = DefaultCheckBoxSize;
			}
		}
	}

	//---------------------------------------------------------------------------
	const Misc::AnsiString &CheckBox::GetText() const {
		return label_->GetText();
	}

	//---------------------------------------------------------------------------
	void CheckBox::SetFont( const Drawing::FontPtr &font ) {
		Control::SetFont( font );

		label_->SetFont( font );
		if( autoSize_ ) {
			size_ = label_->GetSize();
			if( GetFont()->GetFontHeight() < DefaultCheckBoxSize ) {
				checkBoxLocation_ = Drawing::PointI( 0, 0 );
				auto y = static_cast< int >(DefaultCheckBoxSize / 2.0f - GetFont()->GetFontHeight() / 2.0f + 0.5f);
				label_->SetLocation( Drawing::PointI( DefaultLabelOffset.Left, y ) );
			}
			else {
				label_->SetLocation( DefaultLabelOffset );
				auto y = static_cast< int >(GetFont()->GetFontHeight() / 2.0f - DefaultCheckBoxSize / 2.0f + 0.5f);
				checkBoxLocation_ = Drawing::PointI( 0, y );
			}
		}
	}

	//---------------------------------------------------------------------------
	void CheckBox::SetForeColor( const Drawing::Color &color ) {
		Control::SetForeColor( color );

		label_->SetForeColor( Drawing::Color::FromARGB( 255, 205, 205, 205 ) );
	}

	//---------------------------------------------------------------------------
	CheckedChangedEvent &CheckBox::GetCheckedChangedEvent() {
		return checkedChangedEvent_;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void CheckBox::CalculateAbsoluteLocation() {
		Control::CalculateAbsoluteLocation();

		checkBoxAbsoluteLocation_ = absoluteLocation_ + checkBoxLocation_;

		label_->SetParent( this );
	}

	//---------------------------------------------------------------------------
	void CheckBox::DrawSelf( Drawing::RenderContext &context ) {
		Control::DrawSelf( context );

		label_->Render();
	}

	//---------------------------------------------------------------------------
	void CheckBox::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		const auto color = isFocused_ || isInside_
			                   ? Color::FromARGB( 255, 75, 75, 86 ) + GetMouseOverFocusColor()
			                   : Color::FromARGB( 255, 75, 75, 86 );

		g.FillRectangle( Color::FromARGB( 255, 18, 18, 24 ), RectangleF( PointF( 1, 1 ), SizeF( 8, 8 ) ) );

		//g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Color::FromARGB(0, 137, 137, 137)), RectangleF(PointF(1, 1), SizeF(15, 15)));
		g.FillRectangleGradient( color, RectangleF( PointF( 2, 2 ), SizeF( 6, 6 ) ) );

		if( checked_ ) {
			//g.FillRectangle(Color::White(), RectangleF(PointF(5, 5), SizeF(7, 7)));
			g.FillRectangleGradient( ColorRectangle( Color::FromARGB( 255, 206, 115, 136 ), Color::FromARGB( 255, 206, 115, 136 ) - Color::FromARGB( 0, 45, 40, 50 ) ),
			                         RectangleF( PointF( 2, 2 ), SizeF( 6, 6 ) ) );
		}
	}

	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void CheckBox::OnMouseClick( const MouseMessage &mouse ) {
		Control::OnMouseClick( mouse );

		SetChecked( !GetChecked() );
	}

	//---------------------------------------------------------------------------
	bool CheckBox::OnKeyUp( const KeyboardMessage &keyboard ) {
		if( !Control::OnKeyUp( keyboard ) ) {
			if( keyboard.GetKeyCode() == Key::Space ) {
				SetChecked( !GetChecked() );
			}
		}

		return true;
	}

	//---------------------------------------------------------------------------
}
