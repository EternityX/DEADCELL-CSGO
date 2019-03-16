#include "../inc.hpp"
#include "console.hpp"

void console::allocate( const char *window_name ) {
	AllocConsole( );

	_iobuf *data;
	freopen_s( &data, "CONIN$", "r", stdin );
	freopen_s( &data, "CONOUT$", "w", stdout );

	SetConsoleTitleA( window_name );
}

void console::detach() {
	HMENU menu = GetSystemMenu( GetConsoleWindow(), true ); // use true to restore buttons.
	if( !menu )
		return;

	DeleteMenu( menu, SC_CLOSE, MF_BYCOMMAND );

	FreeConsole( );
}

void console::set_text_colour( console_colour colour ) {
	SetConsoleTextAttribute( GetStdHandle( static_cast< uintptr_t >( -11 ) ), *reinterpret_cast< unsigned short * >( &colour ) );
}

void console::print( std::string str, ... ) {
	char buffer [ 1024 ];
	// setup buffer
	buffer[ 0 ] = '\0';

	// concatenate arguments
	va_list vlist;
	__crt_va_start( vlist, str );
	size_t len = vsprintf_s( buffer, str.c_str( ), vlist );
	__crt_va_end( vlist );

	auto wide_string = new wchar_t [ len + 1 ];
	mbstowcs( wide_string, buffer, len + 1 );

	// Now lets print the merged string
	set_text_colour( console_colour::CYAN );
	printf( ( "$> " ) );
	set_text_colour( console_colour::WHITE );
	printf( ( "%s\n" ), util::misc::wide_to_multibyte( wide_string ).c_str( ) );

	delete [ ] wide_string;
}

void console::error( std::string text, ... ) {
	char buffer [ 1024 ];
	// setup buffer
	buffer[ 0 ] = '\0';

	// concatenate arguments
	va_list vlist;
	__crt_va_start( vlist, text );
	size_t len = vsprintf_s( buffer, text.c_str( ), vlist );
	__crt_va_end( vlist );

	auto wide_string = new wchar_t [ len + 1 ];
	mbstowcs( wide_string, buffer, len + 1 );

	// Now lets print the merged string
	set_text_colour( console_colour::RED );
	printf( ( "$> " ) );
	set_text_colour( console_colour::WHITE );
	printf( ( "%s\n" ), util::misc::wide_to_multibyte( wide_string ).c_str( ) );
}
