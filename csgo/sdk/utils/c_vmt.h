#pragma once

class c_vmt {
private:
	uintptr_t m_vmt_base; // VMT base ( on heap / in rdata ).
	uintptr_t *m_old_vmt; // actual VMT.
	uint16_t m_total_methods; // total number of methods in VMT.
	std::unique_ptr< uintptr_t[] > m_new_vmt; // our newly allocated VMT.
	uintptr_t *m_new_vmt_start; // the start ptr to our new VMT ( since RTTI was copied ).

	__forceinline uint16_t count_methods( ) const {
		uint16_t i = 0;

		while( util::misc::valid_code_ptr( m_old_vmt[ i ] ) )
			++i;

		return i;
	}

public:
	c_vmt( ) : m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr }, m_new_vmt_start{ nullptr } { }

	explicit c_vmt( void *base ) : m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr }, m_new_vmt_start{ nullptr } {
		init( base );
	}

	explicit c_vmt( const uintptr_t base ) : m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr }, m_new_vmt_start{ nullptr } {
		init( base );
	}

	~c_vmt( ) {

	}

	bool init( void *base ) {
		return init( reinterpret_cast< uintptr_t >( base ) );
	}

	bool init( uintptr_t base ) {
		if( !base )
			return false;

		m_vmt_base = base;

		m_old_vmt = *reinterpret_cast< uintptr_t ** >( base );
		if( !m_old_vmt )
			return false;

		m_total_methods = count_methods( );
		if( !m_total_methods )
			return false;

		// allocate new VMT, making room for RTTI ptr.
		m_new_vmt = std::make_unique< uintptr_t[] >( m_total_methods + 1 );
		if( !m_new_vmt )
			return false;

		// get address of our newly allocated memory.
		const auto alloc_base = reinterpret_cast< uintptr_t >( m_new_vmt.get( ) );

		// copy VMT, starting from RTTI.
		std::memcpy(
			reinterpret_cast< void * >( alloc_base ),
			static_cast< const void * >( m_old_vmt - 1 ),
			( m_total_methods + 1 ) * sizeof( uintptr_t )
		);

		// get start for new VMT.
		// VMT start is actually 1 function ahead due to RTTI copy.
		m_new_vmt_start = reinterpret_cast< uintptr_t * >( alloc_base + sizeof( uintptr_t ) );

		// set base to our new VMT.
		*reinterpret_cast< uintptr_t * >( base ) = reinterpret_cast< uintptr_t >( m_new_vmt_start );

		return true;
	}

	bool hook_method( uint16_t index, void *func ) {
		if( !m_old_vmt || !m_new_vmt_start || index > m_total_methods || !func )
			return false;

		m_new_vmt_start[ index ] = reinterpret_cast< uintptr_t >( func );

		console::print( "hooking 0x% ==> 0x%.\n", m_old_vmt[ index ], func );
		return true;
	}

	bool unhook_method( uint16_t index ) {
		if( !m_old_vmt || !m_new_vmt_start || index > m_total_methods )
			return false;

		m_old_vmt[ index ] = m_new_vmt_start[ index ];

		return true;
	}

	template< typename T = uintptr_t >
	__forceinline T get_old_method( uint16_t index ) {
		return reinterpret_cast< T >( m_old_vmt[ index ] );
	}

	bool unhook_all( ) {
		if( !m_old_vmt || !m_vmt_base )
			return false;

		*reinterpret_cast< uintptr_t * >( m_vmt_base ) = reinterpret_cast< uintptr_t >( m_old_vmt );

		return true;
	}

	template< typename T = Address >
	__forceinline static T get_method( Address this_ptr, size_t index ) {
		return static_cast< T >( this_ptr.to< T* >( )[ index ] );
	}
};
