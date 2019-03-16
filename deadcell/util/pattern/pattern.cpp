#include "../../inc.hpp"
#include "pattern.hpp"

pattern::Pattern::Pattern( const std::string &str ) {
	std::string part;
	if( str.empty() )
		return;

	auto ss = std::stringstream( str );

	// iterate string stream, splitting by space delim.
	do {
		// too long...
		if( part.empty() || part.size() > 2 )
			continue;

		// ensure it's a valid wildcard or byte.
		if( part[ 0 ] == '?' )
			m_pattern.push_back( { } );

		else if( std::isxdigit( (uint8_t)part[ 0 ] ) && std::isxdigit( (uint8_t)part[ 1 ] ) )
			m_pattern.push_back( { (uint8_t)std::strtoul( part.c_str(), nullptr, 16 ), false } );
	}
	while( ss >> part );
}

uintptr_t pattern::find( uintptr_t start, size_t len, const Pattern &pattern ) {
	uint8_t *scan_start, *scan_end;

	static auto search_pred = [ ]( const uint8_t a, const PatternByte_t &b )
	{
		return b.match( a );
	};

	if( !start || !len || pattern.empty() )
		return 0;

	scan_start = (uint8_t *)start;
	scan_end = scan_start + len;

	auto it = std::search(
		scan_start,
		scan_end,
		pattern.begin(),
		pattern.end(),
		search_pred
	);

	// nothing found...
	if( it == scan_end )
		return 0;

	return (uintptr_t)it;
}

// scan for pattern in range.
uintptr_t pattern::find( uintptr_t start, size_t len, const std::string &pattern ) {
	return find( start, len, Pattern( pattern ) );
}

// scan for pattern in entire module.
uintptr_t pattern::find( const pe::Module &module, const std::string &pattern ) {
	return find( module.get_code_base(), module.get_code_size(), pattern );
}

// scan for pattern in entire module by module name hash.
uintptr_t pattern::find( hash32_t hash, const std::string &pattern ) {
	pe::Module module;

	if( !hash || pattern.empty() )
		return 0;

	module = pe::get_module( hash );
	if( !module )
		return 0;

	return find( module.get_code_base(), module.get_code_size(), pattern );
}

// scan for pattern in entire module by module name string.
uintptr_t pattern::find( const std::string &str, const std::string &pattern ) {
	pe::Module module;

	if( str.empty() || pattern.empty() )
		return 0;

	module = pe::get_module( str );
	if( !module )
		return 0;

	return find( module.get_code_base(), module.get_code_size(), pattern );
}
