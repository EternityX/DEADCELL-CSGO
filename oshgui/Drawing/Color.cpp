/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "Color.hpp"
#include <algorithm>

namespace OSHGui {
	namespace Drawing {
		template< typename T >
		T clamp( const T &val, const T &min, const T &max ) {
			return val < min ? min : val > max ? max : val;
		}

		argb_t ARGBCombine( uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue ) {
			return static_cast< argb_t >(alpha) << 24 |
				static_cast< argb_t >(red) << 16 |
				static_cast< argb_t >(green) << 8 |
				static_cast< argb_t >(blue);
		}

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Color::Color()
			: alpha_( 0 ),
			  red_( 0 ),
			  green_( 0 ),
			  blue_( 0 ),
			  argb_( 0 ) { }

		//---------------------------------------------------------------------------
		Color::Color( uint32_t argb ) {
			SetARGB( argb );
		}

		//---------------------------------------------------------------------------
		Color::Color( float red, float green, float blue )
			: alpha_( 1.0f ),
			  red_( red ),
			  green_( green ),
			  blue_( blue ) {
			CalculateARGB();
		}

		//---------------------------------------------------------------------------
		Color::Color( float alpha, float red, float green, float blue )
			: alpha_( alpha < 0.f ? 0.f : alpha > 1.f ? 1.f : alpha ),
			  red_( red ),
			  green_( green ),
			  blue_( blue ) {
			CalculateARGB();
		}

		//---------------------------------------------------------------------------
		Color Color::FromRGB( uint8_t red, uint8_t green, uint8_t blue ) {
			return FromARGB( 255, red, green, blue );
		}

		//---------------------------------------------------------------------------
		Color Color::FromARGB( uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue ) {
			return Color( ARGBCombine( alpha, red, green, blue ) );
		}

		//---------------------------------------------------------------------------
		Color Color::FromARGB( float *clr, float alpha ) {
			return Color( ARGBCombine( alpha, clr[1], clr[2], clr[3] ) );
		}

		Color Color::FromARGB( float *clr ) {
			return Color( ARGBCombine( clr[0], clr[1], clr[2], clr[3] ) );
		}

		//---------------------------------------------------------------------------
		//predefined colors
		//---------------------------------------------------------------------------
		Color Color::Empty() {
			return Color();
		}

		Color Color::Red() {
			return Color( 1, 1, 0, 0 );
		}

		Color Color::Lime() {
			return Color( 1, 0, 1, 0 );
		}

		Color Color::Blue() {
			return Color( 1, 0, 0, 1 );
		}

		Color Color::Black() {
			return Color( 1, 0, 0, 0 );
		}

		Color Color::Gray() {
			return Color( 1, 0.5f, 0.5f, 0.5f );
		}

		Color Color::White() {
			return Color( 1, 1, 1, 1 );
		}

		Color Color::Yellow() {
			return Color( 1, 1, 1, 0 );
		}

		Color Color::Fuchsia() {
			return Color( 1, 1, 0, 1 );
		}

		Color Color::Cyan() {
			return Color( 1, 0, 1, 1 );
		}

		Color Color::Orange() {
			return Color( 1, 1, 0.5f, 0 );
		}

		Color Color::Maroon() {
			return Color( 1, 0.5f, 0, 0 );
		}

		Color Color::Green() {
			return Color( 1, 0, 0.5f, 0 );
		}

		Color Color::Navy() {
			return Color( 1, 0, 0, 0.5f );
		}

		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		float Color::GetRed() const {
			return red_;
		}

		//---------------------------------------------------------------------------
		float Color::GetGreen() const {
			return green_;
		}

		//---------------------------------------------------------------------------
		float Color::GetBlue() const {
			return blue_;
		}

		//---------------------------------------------------------------------------
		float Color::GetAlpha() const {
			return alpha_;
		}

		void Color::SetAlpha( float alpha ) {
			alpha_ = alpha;
		}

		//---------------------------------------------------------------------------
		argb_t Color::GetARGB() const {
			return argb_;
		}

		//---------------------------------------------------------------------------
		void Color::SetARGB( argb_t argb ) {
			argb_ = argb;

			blue_ = static_cast< float >(argb & 0xFF) / 255.0f;
			argb >>= 8;
			green_ = static_cast< float >(argb & 0xFF) / 255.0f;
			argb >>= 8;
			red_ = static_cast< float >(argb & 0xFF) / 255.0f;
			argb >>= 8;
			alpha_ = static_cast< float >(argb & 0xFF) / 255.0f;
		}

		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool Color::IsTranslucent() const {
			return alpha_ < 0.00001f;
		}

		//---------------------------------------------------------------------------
		void Color::Clamp() {
			alpha_ = clamp( alpha_, 0.f, 1.f );
			red_ = clamp( red_, 0.f, 1.f );
			green_ = clamp( green_, 0.f, 1.f );
			blue_ = clamp( blue_, 0.f, 1.f );

			CalculateARGB();
		}

		//---------------------------------------------------------------------------
		Color Color::Clamped() const {
			auto copy( *this );
			copy.Clamp();
			return copy;
		}

		//---------------------------------------------------------------------------
		void Color::CalculateARGB() {
			argb_ = ARGBCombine(
				static_cast< uint8_t >(( clamp( alpha_, 0.f, 1.f ) * 255.0f ) + 0.5f),
				static_cast< uint8_t >(( clamp( red_, 0.f, 1.f ) * 255.0f ) + 0.5f),
				static_cast< uint8_t >(( clamp( green_, 0.f, 1.f ) * 255.0f ) + 0.5f),
				static_cast< uint8_t >(( clamp( blue_, 0.f, 1.f ) * 255.0f ) + 0.5f)
			);
		}

		//---------------------------------------------------------------------------
		float Color::Hue() const {
			if( red_ == green_ && green_ == blue_ ) {
				return 0.0f;
			}

			const auto r = red_;
			const auto g = green_;
			const auto b = blue_;

			const auto max = r > g ? r : g > b ? g : b;
			const auto min = r < g ? r : g < b ? g : b;
			const auto delta = max - min;
			auto hue = 0.0f;

			if( r == max ) {
				hue = ( g - b ) / delta;
			}
			else if( g == max ) {
				hue = 2 + ( b - r ) / delta;
			}
			else if( b == max ) {
				hue = 4 + ( r - g ) / delta;
			}
			hue *= 60;

			if( hue < 0.0f ) {
				hue += 360.0f;
			}
			return hue;
		}

		//---------------------------------------------------------------------------
		float Color::Saturation() const {
			const auto r = red_;
			const auto g = green_;
			const auto b = blue_;

			const auto max = r > g ? r : g > b ? g : b;
			const auto min = r < g ? r : g < b ? g : b;
			auto s = 0.0f;

			if( max != min ) {
				const auto l = ( max + min ) / 2;

				if( l <= 0.5f ) {
					s = ( max - min ) / ( max + min );
				}
				else {
					s = ( max - min ) / ( 2 - max - min );
				}
			}

			return s;
		}

		//---------------------------------------------------------------------------
		float Color::Brightness() const {
			const auto r = red_;
			const auto g = green_;
			const auto b = blue_;

			const auto max = r > g ? r : g > b ? g : b;
			const auto min = r < g ? r : g < b ? g : b;

			return ( max + min ) / 2;
		}

		//---------------------------------------------------------------------------
		Color Color::FromHSB( float hue, float saturation, float brightness ) {
			const auto h = hue == 1.0f ? 0 : hue * 6.0f;
			const auto f = h - static_cast< int >(h);
			const auto p = brightness * ( 1.0f - saturation );
			const auto q = brightness * ( 1.0f - saturation * f );
			const auto t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

			if( h < 1 ) {
				return Color( brightness, t, p );
			}
			else if( h < 2 ) {
				return Color( q, brightness, p );
			}
			else if( h < 3 ) {
				return Color( p, brightness, t );
			}
			else if( h < 4 ) {
				return Color( p, q, brightness );
			}
			else if( h < 5 ) {
				return Color( t, p, brightness );
			}
			else {
				return Color( brightness, p, q );
			}
		}

		//---------------------------------------------------------------------------
		bool operator==( const Color &lhs, const Color &rhs ) {
			return lhs.red_ == rhs.red_
				&& lhs.green_ == rhs.green_
				&& lhs.blue_ == rhs.blue_
				&& lhs.alpha_ == rhs.alpha_;
		}

		//---------------------------------------------------------------------------
		bool operator!=( const Color &lhs, const Color &rhs ) {
			return !( lhs == rhs );
		}

		//---------------------------------------------------------------------------
		Color operator+( const Color &lhs, const Color &rhs ) {
			return Color( lhs.alpha_ + rhs.alpha_, lhs.red_ + rhs.red_, lhs.green_ + rhs.green_, lhs.blue_ + rhs.blue_ );
		}

		//---------------------------------------------------------------------------
		Color operator-( const Color &lhs, const Color &rhs ) {
			return Color( lhs.alpha_ - rhs.alpha_, lhs.red_ - rhs.red_, lhs.green_ - rhs.green_, lhs.blue_ - rhs.blue_ );
		}

		//---------------------------------------------------------------------------
		Color operator*( const Color &lhs, const Color &rhs ) {
			return Color( lhs.alpha_ * rhs.alpha_, lhs.red_ * rhs.red_, lhs.green_ * rhs.green_, lhs.blue_ * rhs.blue_ );
		}

		//---------------------------------------------------------------------------
		Color operator*( const Color &lhs, float rhs ) {
			return Color( lhs.alpha_ * rhs, lhs.red_ * rhs, lhs.green_ * rhs, lhs.blue_ * rhs );
		}

		//---------------------------------------------------------------------------
	}
}
