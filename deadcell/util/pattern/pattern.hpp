#pragma once

namespace pattern {
	// wraps a uint8_t and has a bool if the byte should be ignored.
	class PatternByte_t {
	private:
		uint8_t m_byte;
		bool m_is_wildcard;

	public:
		PatternByte_t() : m_byte{ 0 }, m_is_wildcard{ true } { }
		PatternByte_t( uint8_t byte, bool is_wildcard ) : m_byte{ byte }, m_is_wildcard{ is_wildcard } { }

		uint8_t get_byte() const {
			return m_byte;
		}

		bool is_wildcard() const {
			return m_is_wildcard;
		}

		bool match( const uint8_t other ) const {
			return m_byte == other || m_is_wildcard;
		}
	};

	// converts a string into a pattern.
	class Pattern {
	private:
		using pattern_t = std::vector< PatternByte_t >;
		using pattern_citer_t = pattern_t::const_iterator;

		pattern_t m_pattern;

	public:
		Pattern() = default;
		explicit Pattern( const std::string &str );

		const PatternByte_t &operator []( size_t index ) const {
			if( index >= m_pattern.size() )
				return { };

			return m_pattern[ index ];
		}

		bool empty() const {
			return m_pattern.empty();
		}

		pattern_citer_t begin () const {
			return m_pattern.cbegin( );
		}

		pattern_citer_t end( ) const {
			return m_pattern.cend( );
		}
	};

	uintptr_t find( uintptr_t start, size_t len, const Pattern &pattern );
	uintptr_t find( uintptr_t start, size_t len, const std::string &pattern );
	uintptr_t find( const pe::Module &module, const std::string &pattern );
	uintptr_t find( hash32_t hash, const std::string &pattern );
	uintptr_t find( const std::string &str, const std::string &pattern );

	template < typename t = uintptr_t >
	static t find( const std::string &str, const std::string &pattern ){
		return ( t )( find( str, pattern ) );
	}

	template < typename t = uintptr_t >
	static t find( const pe::Module &module, const std::string &pattern, int offset = 0 ){
		return ( t )( find( module, pattern ) + offset );
	}
}
