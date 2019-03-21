#include "../inc.hpp"
#include "console.hpp"

bool console::allocate( const char *window_name ) {
	if( !AllocConsole( ) )
		return false;

	_iobuf *data;
	const errno_t res = freopen_s( &data, "CONOUT$", "w", stdout );
	if( res != 0 )
		return false;

	if( !SetConsoleTitleA( window_name ) )
		return false;

	return true;
}

void console::detach( ) {
	FreeConsole( );
}