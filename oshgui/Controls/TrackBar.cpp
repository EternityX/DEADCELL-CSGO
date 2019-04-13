/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#pragma warning( disable : 4101 ) // unreferenced local variable

#include "TrackBar.hpp"
#include "Label.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"
#include <sstream>
#include <iomanip>
#include "Drawing/FontManager.hpp"
#include "Timer.hpp"
#include <iostream>

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI TrackBar::GrabSize( Drawing::SizeF( SliderSize.Width - 9, 7 ) );
	const Drawing::SizeI TrackBar::SliderSize( 8, 16 );
	const Drawing::SizeI TrackBar::DefaultSize( 166, TrackBar::SliderSize.Height + 2 );
	const Drawing::PointI TrackBar::DefaultLabelOffset( 4, -5 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TrackBar::TrackBar()
		: drag_( false ),
		minimum_( 1 ),
		maximum_( 10 ),
		tickFrequency_( 1 ),
		precision_( 0 ),
		sliderLocation_( -SliderSize.Width / 2, 1 ),
		label_( new Label() ),
		label_value_( new Label() )
	{
		type_ = ControlType::TrackBar;

		SetSize( DefaultSize );

		label_->SetLocation( DefaultLabelOffset );
		label_->SetBackColor( Drawing::Color::Empty() );
		label_->SetForeColor( Drawing::Color::FromARGB( 255, 205, 205, 205 ) );
		label_->SetStyle( 1 );
		label_->SetFont( Application::Instance().GetDefaultFont() );

		label_value_->SetLocation( Drawing::PointF( sliderLocation_.Left - label_value_->GetWidth() / 2 + 5, 8 ) );
		label_value_->SetForeColor( Drawing::Color::FromARGB( 255, 205, 205, 205 ) );
		label_value_->SetStyle( 2 );

		SetValueInternal( 0 );

		ApplyStyle( Application::Instance().GetStyle() );
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TrackBar::SetSize( const Drawing::SizeI &size )
	{
		if( size.Height < SliderSize.Height + 2 )
		{
			Control::SetSize( Drawing::SizeI( size.Width, SliderSize.Height + 2 ) );
		}
		else
		{
			Control::SetSize( size );
		}

		SetValueInternal( value_ );
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetText( const Misc::AnsiString &text )
	{
		const auto offset = label_->GetLocation();

		label_->SetText( text );
		if( autoSize_ )
		{
			size_ = label_->GetSize().InflateEx( offset.Left, offset.Top );
			if( size_.Height < 8 )
			{
				size_.Height = 8;
			}
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetPrecision( int precision )
	{
		precision_ = precision;
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetPrecision() const
	{
		return precision_;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetMinimum( float minimum )
	{
		minimum_ = minimum;

		SetValueInternal( value_ );
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetMinimum() const
	{
		return minimum_;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetMaximum( float maximum )
	{
		maximum_ = maximum;

		SetValueInternal( value_ );
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetMaximum() const
	{
		return maximum_;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetTickFrequency( float tickFrequency )
	{
		tickFrequency_ = tickFrequency;

		SetValueInternal( value_ );
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetTickFrequency() const
	{
		return tickFrequency_;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValue( float value )
	{
		SetValueInternal( value - minimum_ );
	}
	//---------------------------------------------------------------------------
	float TrackBar::GetValue() const
	{
		return value_ + minimum_;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetAppendText( std::string text )
	{
		appendText_ = text;
	}
	//---------------------------------------------------------------------------
	ValueChangedEvent& TrackBar::GetValueChangedEvent()
	{
		return valueChangedEvent_;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void TrackBar::SetValueInternal( float value )
	{
		try { // epic bandaid fix.
			std::ostringstream string_stream;

			pixelsPerTick_ = (float)( GetWidth() - SliderSize.Width ) / ( ( maximum_ - minimum_ ) / tickFrequency_ );

			if( value < 0 )
				value = 0;

			if( value > maximum_ - minimum_ )
				value = maximum_ - minimum_;

			string_stream << std::fixed << std::setprecision( precision_ ) << value + minimum_ << appendText_.c_str( );
			label_value_->SetText( string_stream.str( ) );

			if( value_ != value ) {
				value_ = value;

				valueChangedEvent_.Invoke( this );

				const auto tick = value / tickFrequency_;
				sliderLocation_.Left = tick * pixelsPerTick_;
				sliderAbsoluteLocation_.Left = absoluteLocation_.Left + sliderLocation_.Left;

				label_value_->SetText( string_stream.str() );
				Invalidate();
			}

			label_value_->SetLocation( Drawing::PointF( sliderLocation_.Left - label_value_->GetWidth() / 2 + 5, 8 ) );
		}
		catch( const std::exception &ex ) {
			_RPTF0( _CRT_ERROR, ex.what( ) );
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::DrawSelf( Drawing::RenderContext &context )
	{
		Control::DrawSelf( context );

		label_->Render();
		label_value_->Render();
	}
	//---------------------------------------------------------------------------
	void TrackBar::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		sliderAbsoluteLocation_ = absoluteLocation_ + sliderLocation_;

		label_->SetParent( this );
		label_value_->SetParent( this );
	}
	//---------------------------------------------------------------------------
	void TrackBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g( *geometry_ );

		const auto color = isFocused_ || isInside_ ? Color::FromARGB( 255, 46, 46, 56 ) : Color::FromARGB( 255, 36, 36, 46 );

		g.FillRectangleGradient( Color::FromARGB( 255, 18, 18, 24 ), PointF( 4, DefaultTickOffset ), SizeF( 160, 7 ) );
		g.FillRectangleGradient( ColorRectangle( color, Color::FromARGB( 255, 55, 55, 62 ) ), PointF( 5, DefaultTickOffset + 1 ), SizeF( 158, 5 ) );

		if( value_ ) {
			g.FillRectangleGradient( ColorRectangle( GetBackColor(), GetBackColor() - Color::FromARGB( 0, 45, 40, 45 ) ), RectangleF( PointF( sliderLocation_.Left + 5, 8 ), SizeF( SliderSize.Width - sliderLocation_.Left - 8, 5 ) ) );
		}

		/*if( value_ != GetMinimum() ) {
			g.DrawRectangle( OSHGui::Drawing::Color::FromARGB( 255, 105, 105, 112 ), RectangleF( -3, 10, 5, 1 ) );
		}

		if( value_ != GetMaximum() ) {
			g.DrawRectangle( OSHGui::Drawing::Color::FromARGB( 255, 105, 105, 112 ), RectangleF( GetWidth() + 2, 8, 1, 5 ) );
			g.DrawRectangle( OSHGui::Drawing::Color::FromARGB( 255, 105, 105, 112 ), RectangleF( GetWidth(), 10, 5, 1 ) );
		}

		g.DrawRectangle( OSHGui::Drawing::Color::FromARGB( 122, 255, 0, 0 ), absoluteLocation_.OffsetEx( 3, -8 ), Drawing::SizeF( 5, 5 ) );*/
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseDown( const MouseMessage &mouse )
	{
		Control::OnMouseDown( mouse );

		drag_ = true;
		OnGotMouseCapture();
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseUp( const MouseMessage &mouse )
	{
		Control::OnMouseUp( mouse );

		drag_ = false;
		OnLostMouseCapture();
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseClick( const MouseMessage &mouse )
	{
		Control::OnMouseClick( mouse );

		

		if( !drag_ )
		{
			HandleMouseEvent( mouse );
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseMove( const MouseMessage &mouse )
	{
		Control::OnMouseMove( mouse );

		if( Intersection::TestRectangle( absoluteLocation_.OffsetEx( 3, -8 ), Drawing::SizeF( 5, 5 ), mouse.GetLocation() ) ) {
			SetValueInternal( --value_ );
		}

		if( drag_ )
		{
			HandleMouseEvent( mouse );
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::HandleMouseEvent( const MouseMessage &mouse )
	{
		float tick = ( mouse.GetLocation().Left - absoluteLocation_.Left ) / pixelsPerTick_;
		SetValueInternal( tick * tickFrequency_ );
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseScroll( const MouseMessage &mouse )
	{
		Control::OnMouseScroll( mouse );

		SetValueInternal( value_ + mouse.GetDelta() );
	}
	//---------------------------------------------------------------------------
	bool TrackBar::OnKeyDown( const KeyboardMessage &keyboard )
	{
		if( !Control::OnKeyDown( keyboard ) )
		{
			switch( keyboard.GetKeyCode() )
			{
			case Key::Home:
				SetValueInternal( 0 );
				break;;
			case Key::End:
				SetValueInternal( maximum_ - minimum_ );
				break;
				case Key::Left:
				SetValueInternal( value_ - 1.f );
				break;
			case Key::Down:
				SetValueInternal( value_ - 1 );
				break;
				case Key::Right:
				SetValueInternal( value_ + 1.f );
				break;
			case Key::Up:
				SetValueInternal( value_ + 1 );
				break;
			case Key::PageDown:
				SetValueInternal( value_ - std::max( 10.0f, ( maximum_ - minimum_ ) / 10.0f ) );
				break;
			case Key::PageUp:
				SetValueInternal( value_ + std::max( 10.0f, ( maximum_ - minimum_ ) / 10.0f ) );
				break;
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
}
