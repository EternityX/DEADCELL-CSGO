/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include "GroupBox.hpp"
#include "../Misc/Exceptions.hpp"
#include "Label.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox() {
		type_ = ControlType::GroupBox;

		captionLabel_ = new Label();
		captionLabel_->SetLocation( Drawing::PointI( 15, -1 ) );
		captionLabel_->SetBackColor( Drawing::Color::Empty() );
		captionLabel_->SetStyle( 1 );
		AddSubControl( captionLabel_ );

		containerPanel_ = new Panel();
		containerPanel_->SetLocation( Drawing::PointI( 3, 10 ) );
		containerPanel_->SetBackColor( Drawing::Color::Empty() );
		AddSubControl( containerPanel_ );

		ApplyStyle( Application::Instance().GetStyle() );

		canRaiseEvents_ = false;
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetSize( const Drawing::SizeI &size ) {
		Control::SetSize( size );

		containerPanel_->SetSize( size.InflateEx( -3 * 2, -3 * 2 - 10 ) );
	}

	//---------------------------------------------------------------------------
	void GroupBox::SetText( const Misc::AnsiString &text ) {
		captionLabel_->SetText( text );
	}

	//---------------------------------------------------------------------------
	const Misc::AnsiString &GroupBox::GetText() const {
		return captionLabel_->GetText();
	}

	//---------------------------------------------------------------------------
	void GroupBox::SetFont( const Drawing::FontPtr &font ) {
		Control::SetFont( font );

		captionLabel_->SetFont( font );
	}

	//---------------------------------------------------------------------------
	void GroupBox::SetForeColor( const Drawing::Color &color ) {
		Control::SetForeColor( color );

		captionLabel_->SetForeColor( Drawing::Color::FromARGB( 255, 205, 205, 205 ) );
	}

	//---------------------------------------------------------------------------
	const std::deque< Control* > &GroupBox::GetControls() const {
		return containerPanel_->GetControls();
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void GroupBox::AddControl( Control *control ) {
		containerPanel_->AddControl( control );
	}

	//---------------------------------------------------------------------------
	void GroupBox::RemoveControl( Control *control ) {
		containerPanel_->RemoveControl( control );
	}

	//---------------------------------------------------------------------------
	void GroupBox::DrawSelf( Drawing::RenderContext &context ) {
		Control::DrawSelf( context );

		captionLabel_->Render();
		containerPanel_->Render();
	}

	//---------------------------------------------------------------------------
	void GroupBox::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		g.FillRectangle( Color::FromARGB( 255, 27, 27, 34 ), PointF( 0, 6 ), GetSize() - SizeI( 0, 5 ) );

		g.FillRectangle( Color::FromARGB( 255, 51, 51, 58 ), PointF( 0, 5 ), SizeF( 10, 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 51, 51, 58 ), PointF( 20 + captionLabel_->GetWidth(), 5 ),
		                 SizeF( GetWidth() - captionLabel_->GetWidth() - 20, 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 51, 51, 58 ), PointF( 0, 6 ), SizeF( 1, GetHeight() - 6 ) );
		g.FillRectangle( Color::FromARGB( 255, 51, 51, 58 ), PointF( GetWidth() - 1, 6 ), SizeF( 1, GetHeight() - 6 ) );
		g.FillRectangle( Color::FromARGB( 255, 51, 51, 58 ), PointF( 1, GetHeight() - 1 ), SizeF( GetWidth() - 2, 1 ) );

		//outline
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 26 ), PointF( 0, 4 ), SizeF( 10, 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 26 ), PointF( 20 + captionLabel_->GetWidth(), 4 ),
		                 SizeF( GetWidth() - captionLabel_->GetWidth() - 20, 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 26 ), PointF( -1, 4 ), SizeF( 1, GetHeight() - 3 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 26 ), PointF( GetWidth(), 4 ), SizeF( 1, GetHeight() - 3 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 26 ), PointF( 0, GetHeight() ), SizeF( GetWidth(), 1 ) );
	}

	//---------------------------------------------------------------------------
}
