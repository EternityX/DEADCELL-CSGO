#include "../inc.hpp"
#include "console.hpp"

bool console::allocate( const char *window_name ) {
	if( !AllocConsole() ) {
		_RPTF1( _CRT_ERROR, "Failed to allocate console. Error code: %i", GetLastError( ) );
		return false;
	}

	_iobuf *data;
	const errno_t res = freopen_s( &data, "CONOUT$", "w", stdout );
	if( res != 0 ) {
		_RPTF1( _CRT_ERROR, "Failed to open stdout filestream. Error code: %i", res );
		return false;
	}

	if( !SetConsoleTitleA( window_name ) ) {
		_RPTF1( _CRT_WARN, "Failed to set console title. Error code: %i", GetLastError( ) );
		return false;
	}

	return true;
}

void console::detach( ) {
	FreeConsole( );
}