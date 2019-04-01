#pragma once

enum class console_color {
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKCYAN,
	DARKRED,
	DARKPINK,
	DARKYELLOW,
	LIGHTGREY,
	DARKGREY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PINK,
	YELLOW,
	WHITE
};

namespace console {
	bool allocate( const char *window_name );
	void detach( );

	template< class E, class T >
	constexpr std::basic_ostream< E, T > &operator<<( std::basic_ostream< E, T > &os, console_color col ) {
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), static_cast< unsigned short >( col ) );
		return os;
	}

	inline void print( const char *format ) {
		std::cout << format;
	}

	template< typename T, typename... Targs >
	void print( const char *format, T value, Targs ... f_args ) {
		for( ; *format != '\0'; format++ ) {
			if( *format == '%' ) {
				std::cout << value;
				print( format + 1, f_args... ); // recursive call
				return;
			}

			std::cout << *format;
		}
	}
}
