#include "../../inc.hpp"
#include "misc.hpp"

bool util::misc::valid_code_ptr( uintptr_t addr ) {
	if( !addr )
		return false;

	MEMORY_BASIC_INFORMATION mbi;
	if( !VirtualQuery( reinterpret_cast< const void * >( addr ), &mbi, sizeof mbi ) )
		return false;

	if( !( mbi.Protect & PAGE_EXECUTE_READWRITE || mbi.Protect & PAGE_EXECUTE_READ ) )
		return false;

	return true;
}

std::string util::misc::wide_to_multibyte( const std::wstring &str ) {
	if( str.empty( ) )
		return {};

	int str_len = WideCharToMultiByte( CP_UTF8, 0, str.data( ), static_cast< int >( str.size( ) ), nullptr, 0, nullptr, nullptr );

	std::string out;
	out.resize( str_len );

	WideCharToMultiByte( CP_UTF8, 0, str.data( ), static_cast< int >( str.size( ) ), &out[ 0 ], str_len, nullptr, nullptr );

	return out;
}

std::wstring util::misc::multibyte_to_wide( const std::string &str ) {
	if( str.empty( ) )
		return {};

	int str_len = MultiByteToWideChar( CP_UTF8, 0, str.data( ), static_cast< int >( str.size( ) ), nullptr, 0 );

	std::wstring out;
	out.resize( str_len );

	MultiByteToWideChar( CP_UTF8, 0, str.data( ), static_cast< int >( str.size( ) ), &out[ 0 ], str_len );

	return out;
}

std::string util::misc::unicode_to_ascii( const std::wstring &unicode ) {
	std::string ascii_str( unicode.begin( ), unicode.end( ) );
	return ascii_str;
}

std::wstring util::misc::ascii_to_unicode( const std::string &ascii ) {
	std::wstring unicode_str( ascii.begin( ), ascii.end( ) );
	return unicode_str;
}
