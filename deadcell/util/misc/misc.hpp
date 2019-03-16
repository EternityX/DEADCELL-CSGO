#pragma once

namespace util {
	namespace misc {
		bool valid_code_ptr( uintptr_t addr );

		std::string wide_to_multibyte( const std::wstring &str );
		std::wstring multibyte_to_wide( const std::string &str );
		std::string unicode_to_ascii( const std::wstring& unicode );
		std::wstring ascii_to_unicode( const std::string& ascii );

		// get method from VMT.
		template< typename t >
		t vfunc( void *this_ptr, uint16_t index ) {
			return ( *static_cast< t ** >(this_ptr) )[ index ];
		}

		// follow relative32 offset.
		// input argument is the address of the relative offset.
		template< typename t = uintptr_t >
		t follow_rel32( uintptr_t addr ) {
			if( !addr )
				return 0;

			// get rel32 offset.
			uint32_t disp = *(uint32_t *)addr;
			if( !disp )
				return 0;

			// displacement is relative to address of the next instruction.
			return ( addr + sizeof( uint32_t ) ) + disp;
		}

		// returns a random floating point value in range.
		template< typename t >
		t get_random_float_range( const t &min, const t &max ) {
			static_assert( std::is_floating_point< t >::value, "invalid type for: get_random_float_range." );

			std::random_device rd;
			std::mt19937 e2( rd() );
			std::uniform_real_distribution< t > distribution( min, max );

			return distribution( e2 );
		}

		// returns a random integral value in range.
		template< typename t >
		t get_random_int_range( const t &min, const t &max ) {
			static_assert( std::is_integral< t >::value, "invalid type for: get_random_int_range." );

			std::random_device rd;
			std::mt19937 e2( rd() );
			std::uniform_int_distribution< t > distribution( min, max );

			return distribution( e2 );
		}
		template < typename t > 
		t clamp( t in, t low, t high ) {
			return std::min( std::max( in, low ), high );
		}
		class Timer {
		private:
			std::chrono::high_resolution_clock::time_point m_start;

		public:
			Timer( bool start = false ) {
				if( start )
					reset();
			}

			void reset() {
				m_start = std::chrono::high_resolution_clock::now();
			}

			ulong_t elapsed() {
				auto elapsed = std::chrono::high_resolution_clock::now() - m_start;
				return std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count();
			}
		};
	}
}
