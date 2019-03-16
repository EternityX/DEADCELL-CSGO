/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "PictureBox.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI PictureBox::DefaultSize( 100, 100 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PictureBox::PictureBox()
		: stretch_( false ) {
		type_ = ControlType::PictureBox;

		SetSize( DefaultSize );

		ApplyStyle( Application::Instance().GetStyle() );

		isFocusable_ = false;
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void PictureBox::SetImage( const Drawing::ImagePtr &image ) {
		image_ = image;

		Invalidate();
	}

	//---------------------------------------------------------------------------
	Drawing::ImagePtr &PictureBox::GetImage() {
		return image_;
	}

	//---------------------------------------------------------------------------
	void PictureBox::SetStretch( bool stretch ) {
		stretch_ = stretch;
		if( image_ ) {
			Invalidate();
		}
	}

	//---------------------------------------------------------------------------
	bool PictureBox::GetStretch() const {
		return stretch_;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void PictureBox::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		if( GetBackColor().GetAlpha() > 0 ) {
			g.FillRectangle( GetBackColor(), PointF( 0, 0 ), GetSize() );
		}

		if( image_ ) {
			if( stretch_ ) {
				g.DrawImage( image_, Color::White(), RectangleF( PointF( 0, 0 ), GetSize() ) );
			}
			else {
				g.DrawImage( image_, Color::White(), PointF( 0, 0 ), RectangleF( PointF( 0, 0 ), GetSize() ) );
			}
		}
	}

	//---------------------------------------------------------------------------
}
