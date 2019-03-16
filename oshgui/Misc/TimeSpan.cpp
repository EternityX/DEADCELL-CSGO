/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "TimeSpan.hpp"
#include "Exceptions.hpp"

namespace OSHGui {
	namespace Misc {
		const long long TimeSpan::TicksPerMillisecond = 10000i64;
		const long long TimeSpan::TicksPerSecond = TicksPerMillisecond * 1000i64;
		const long long TimeSpan::TicksPerMinute = TicksPerSecond * 60i64;
		const long long TimeSpan::TicksPerHour = TicksPerMinute * 60i64;
		const long long TimeSpan::TicksPerDay = TicksPerHour * 24i64;

		const double TimeSpan::MillisecondsPerTick = 1.0 / TicksPerMillisecond;
		const double TimeSpan::SecondsPerTick = 1.0 / TicksPerSecond;
		const double TimeSpan::MinutesPerTick = 1.0 / TicksPerMinute;
		const double TimeSpan::HoursPerTick = 1.0 / TicksPerHour;
		const double TimeSpan::DaysPerTick = 1.0 / TicksPerDay;

		const int TimeSpan::MillisPerSecond = 1000;
		const int TimeSpan::MillisPerMinute = MillisPerSecond * 60;
		const int TimeSpan::MillisPerHour = MillisPerMinute * 60;
		const int TimeSpan::MillisPerDay = MillisPerHour * 24;

		const long long TimeSpan::MaxSeconds = 9223372036854775807i64 / TicksPerSecond;
		const long long TimeSpan::MinSeconds = -9223372036854775806i64 / TicksPerSecond;
		const long long TimeSpan::MaxMilliSeconds = 9223372036854775807i64 / TicksPerMillisecond;
		const long long TimeSpan::MinMilliSeconds = -9223372036854775806i64 / TicksPerMillisecond;

		const long long TimeSpan::TicksPerTenthSecond = TicksPerMillisecond * 100;

		const TimeSpan TimeSpan::Zero( 0 );
		const TimeSpan TimeSpan::MaxValue( 9223372036854775807i64 );
		const TimeSpan TimeSpan::MinValue( -9223372036854775806i64 );

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan()
			: ticks_( 0 ) { }

		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan( long long ticks )
			: ticks_( ticks ) { }

		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan( int hours, int minutes, int seconds )
			: ticks_( TimeToTicks( hours, minutes, seconds ) ) { }

		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan( int days, int hours, int minutes, int seconds ) {
			const auto totalMilliSeconds = ( static_cast< long long >(days) * 3600 * 24
				+ static_cast< long long >(hours) * 3600
				+ static_cast< long long >(minutes) * 60
				+ seconds ) * 1000;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
			{
				throw ArgumentOutOfRangeException("milliseconds");
			}
#endif
			ticks_ = totalMilliSeconds * TicksPerMillisecond;
		}

		//---------------------------------------------------------------------------
		TimeSpan::TimeSpan( int days, int hours, int minutes, int seconds, int milliseconds ) {
			const auto totalMilliSeconds = ( static_cast< long long >(days) * 3600 * 24
					+ static_cast< long long >(hours) * 3600
					+ static_cast< long long >(minutes) * 60
					+ seconds ) * 1000
				+ milliseconds;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
			{
				throw ArgumentOutOfRangeException("milliseconds");
			}
#endif
			ticks_ = totalMilliSeconds * TicksPerMillisecond;
		}

		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		int TimeSpan::GetDays() const {
			return static_cast< int >(ticks_ / TicksPerDay);
		}

		//---------------------------------------------------------------------------
		int TimeSpan::GetHours() const {
			return static_cast< int >(( ticks_ / TicksPerHour ) % 24);
		}

		//---------------------------------------------------------------------------
		int TimeSpan::GetMinutes() const {
			return static_cast< int >(( ticks_ / TicksPerMinute ) % 60);
		}

		//---------------------------------------------------------------------------
		int TimeSpan::GetSeconds() const {
			return static_cast< int >(( ticks_ / TicksPerSecond ) % 60);
		}

		//---------------------------------------------------------------------------
		int TimeSpan::GetMilliseconds() const {
			return static_cast< int >(( ticks_ / TicksPerMillisecond ) % 1000);
		}

		//---------------------------------------------------------------------------
		long long TimeSpan::GetTicks() const {
			return ticks_;
		}

		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalDays() const {
			return static_cast< double >(ticks_) * DaysPerTick;
		}

		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalHours() const {
			return static_cast< double >(ticks_) * HoursPerTick;
		}

		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalMinutes() const {
			return static_cast< double >(ticks_) * MinutesPerTick;
		}

		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalSeconds() const {
			return static_cast< double >(ticks_) * SecondsPerTick;
		}

		//---------------------------------------------------------------------------
		double TimeSpan::GetTotalMilliseconds() const {
			const auto temp = static_cast< double >(ticks_) * MillisecondsPerTick;
			if( temp > MaxMilliSeconds ) {
				return static_cast< double >(MaxMilliSeconds);
			}
			if( temp < MinMilliSeconds ) {
				return static_cast< double >(MinMilliSeconds);
			}
			return temp;
		}

		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Add( TimeSpan ts ) const {
			const auto result = ticks_ + ts.ticks_;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if ((ticks_ >> 63 == ts.ticks_ >> 63) && (ticks_ >> 63 != result >> 63))
			{
				throw ArgumentOutOfRangeException("ticks");
			}
#endif
			return TimeSpan( result );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Subtract( TimeSpan ts ) const {
			const auto result = ticks_ - ts.ticks_;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if ((ticks_ >> 63 != ts.ticks_ >> 63) && (ticks_ >> 63 != result >> 63))
			{
				throw ArgumentOutOfRangeException("ticks");
			}
#endif
			return TimeSpan( result );
		}

		//---------------------------------------------------------------------------
		bool TimeSpan::operator ==( const TimeSpan &ts ) const {
			return ticks_ == ts.ticks_;
		}

		//---------------------------------------------------------------------------
		bool TimeSpan::operator !=( const TimeSpan &ts ) const {
			return ticks_ != ts.ticks_;
		}

		//---------------------------------------------------------------------------
		bool TimeSpan::operator <( const TimeSpan &ts ) const {
			return ticks_ < ts.ticks_;
		}

		//---------------------------------------------------------------------------
		bool TimeSpan::operator >( const TimeSpan &ts ) const {
			return ticks_ > ts.ticks_;
		}

		//---------------------------------------------------------------------------
		bool TimeSpan::operator <=( const TimeSpan &ts ) const {
			return ticks_ <= ts.ticks_;
		}

		//---------------------------------------------------------------------------
		bool TimeSpan::operator >=( const TimeSpan &ts ) const {
			return ticks_ >= ts.ticks_;
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::operator-() const {
			return TimeSpan( -ticks_ );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::operator+() const {
			return *this;
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::operator-( const TimeSpan &ts ) const {
			return Subtract( ts );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::operator+( const TimeSpan &ts ) const {
			return Add( ts );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Duration() const {
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks_ == MinValue.ticks_)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
#endif
			return TimeSpan( ticks_ >= 0 ? ticks_ : -ticks_ );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Negate() const {
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (ticks_ == MinValue.ticks_)
			{
				throw ArgumentOutOfRangeException("ticks");
			}
#endif
			return TimeSpan( -ticks_ );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromDays( double value ) {
			return Interval( value, MillisPerDay );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromHours( double value ) {
			return Interval( value, MillisPerHour );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromMinutes( double value ) {
			return Interval( value, MillisPerMinute );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromSeconds( double value ) {
			return Interval( value, MillisPerSecond );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromMilliseconds( double value ) {
			return Interval( value, 1 );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::FromTicks( long long value ) {
			return TimeSpan( value );
		}

		//---------------------------------------------------------------------------
		TimeSpan TimeSpan::Interval( double value, int scale ) {
			//if (value == 0.0 / 0.0)
			//{
			//	throw ArgumentException("Invalid argument: value is NAN");
			//}
			const auto temp = value * scale;
			const auto millis = temp + ( value >= 0.0 ? 0.5 : -0.5 );
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if ((millis > MaxMilliSeconds) || (millis < MinMilliSeconds))
			{
				throw ArgumentOutOfRangeException("value");
			}
#endif
			return TimeSpan( static_cast< long long >(millis) * TicksPerMillisecond );
		}

		//---------------------------------------------------------------------------
		long long TimeSpan::TimeToTicks( int hour, int minute, int second ) {
			const auto totalSeconds = static_cast< long long >(hour) * 3600
				+ static_cast< long long >(minute) * 60
				+ static_cast< long long >(second);
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
			{
				throw ArgumentOutOfRangeException("totalSeconds");
			}
#endif
			return totalSeconds * TicksPerSecond;
		}

		//---------------------------------------------------------------------------
		AnsiString TimeSpan::ToString() const {
			const auto days = GetDays();
			const auto milliseconds = GetMilliseconds();
			if( milliseconds != 0 ) {
				if( days != 0 ) {
					return String::Format( "%u.%02u:%02u:%02u.%03u0000", days, GetHours(), GetMinutes(), GetSeconds(), milliseconds );
				}
				return String::Format( "%02u:%02u:%02u.%03u0000", GetHours(), GetMinutes(), GetSeconds(), milliseconds );
			}
			return String::Format( "%02u:%02u:%02u", GetHours(), GetMinutes(), GetSeconds() );
		}

		//---------------------------------------------------------------------------
	}
}
