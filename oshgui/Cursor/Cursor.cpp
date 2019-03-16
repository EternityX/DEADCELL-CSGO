/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "Cursor.hpp"
#include "../Application.hpp"
#include "../Drawing/Graphics.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Cursor::Cursor()
		: geometry_( Application::Instance().GetRenderer().CreateGeometryBuffer() ) { }

	//---------------------------------------------------------------------------
	Cursor::~Cursor() { }
	//---------------------------------------------------------------------------
	Drawing::GeometryBufferPtr Cursor::GetGeometry() {
		return geometry_;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Cursor::Initialize() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		auto color = Color::FromARGB( 185, 140, 140, 140 );
		auto color2 = Color::FromARGB( 255, 211, 211, 211 );

		g.FillRectangle( color, PointF( 0, 0 ), SizeF( 1, 12 ) );
		g.FillRectangle( color, PointF( 1, 0 ), SizeF( 1, 11 ) );
		g.FillRectangle( color, PointF( 1, 11 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 2, 1 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 2, 10 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 3, 2 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 3, 9 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 4, 3 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 5, 4 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 6, 5 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 7, 6 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 8, 7 ), SizeF( 1, 1 ) );
		g.FillRectangle( color, PointF( 4, 8 ), SizeF( 5, 1 ) );

		g.FillRectangle( color2, PointF( 1, 1 ), SizeF( 1, 10 ) );
		g.FillRectangle( color2, PointF( 2, 2 ), SizeF( 1, 8 ) );
		g.FillRectangle( color2, PointF( 3, 3 ), SizeF( 1, 6 ) );
		g.FillRectangle( color2, PointF( 4, 4 ), SizeF( 1, 4 ) );
		g.FillRectangle( color2, PointF( 5, 5 ), SizeF( 1, 3 ) );
		g.FillRectangle( color2, PointF( 6, 6 ), SizeF( 1, 2 ) );
		g.FillRectangle( color2, PointF( 7, 7 ), SizeF( 1, 1 ) );
	}

	//---------------------------------------------------------------------------
}
