#pragma once

class c_vmt {
private:
	uintptr_t m_vmt_base; // VMT base ( on heap / in rdata ).
	uintptr_t *m_old_vmt; // actual VMT.
	uint16_t m_total_methods; // total number of methods in VMT.
	std::unique_ptr< uintptr_t[] > m_new_vmt; // our newly allocated VMT.
	uintptr_t *m_new_vmt_start; // the start ptr to our new VMT ( since RTTI was copied ).

	__forceinline uint16_t count_methods() const {
		uint16_t i = 0;

		while( util::misc::valid_code_ptr( m_old_vmt[ i ] ) )
			++i;

		return i;
	}

public:
	c_vmt() : m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr },
	          m_new_vmt_start{ nullptr } { }

	c_vmt( void *base ) : m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr },
	                      m_new_vmt_start{ nullptr } {
		init( base );
	}

	c_vmt( uintptr_t base ) : m_vmt_base{ 0 }, m_old_vmt{ nullptr }, m_total_methods{ 0 }, m_new_vmt{ nullptr },
	                          m_new_vmt_start{ nullptr } {
		init( base );
	}

	~c_vmt() {
		// todo - dex; what to do here?
	}

	bool init( void *base ) {
		return init( (uintptr_t)base );
	}

	bool init( uintptr_t base ) {
		uintptr_t alloc_base;

		if( !base )
			return false;

		m_vmt_base = base;

		m_old_vmt = *(uintptr_t **)base;
		if( !m_old_vmt )
			return false;

		m_total_methods = count_methods();
		if( !m_total_methods )
			return false;

		// allocate new VMT, making room for RTTI ptr.
		m_new_vmt = std::make_unique< uintptr_t[] >( m_total_methods + 1 );
		if( !m_new_vmt )
			return false;

		// get address of our newly allocated memory.
		alloc_base = (uintptr_t)m_new_vmt.get();

		// copy VMT, starting from RTTI.
		std::memcpy(
			(void *)alloc_base,
			(const void *)( m_old_vmt - 1 ),
			( m_total_methods + 1 ) * sizeof( uintptr_t )
		);

		// get start for new VMT.
		// VMT start is actually 1 function ahead due to RTTI copy.
		m_new_vmt_start = (uintptr_t *)( alloc_base + sizeof( uintptr_t ) );

		// set base to our new VMT.
		*(uintptr_t *)base = (uintptr_t)m_new_vmt_start;

		return true;
	}

	bool hook_method( uint16_t index, void *func ) {
		if( !m_old_vmt || !m_new_vmt_start || index > m_total_methods || !func )
			return false;

		m_new_vmt_start[ index ] = (uintptr_t)func;

		console::print( "hooking 0x%p ==> 0x%p.", m_old_vmt[ index ], func );
		return true;
	}

	bool unhook_method( uint16_t index ) {
		if( !m_old_vmt || !m_new_vmt_start || index > m_total_methods )
			return false;

		m_old_vmt[ index ] = m_new_vmt_start[ index ];

		return true;
	}

	template< typename t = uintptr_t >
	__forceinline t get_old_method( uint16_t index ) {
		return (t)m_old_vmt[ index ];
	}

	bool unhook_all() {
		if( !m_old_vmt || !m_vmt_base )
			return false;

		*(uintptr_t *)m_vmt_base = (uintptr_t)m_old_vmt;

		return true;
	}
	template< typename t = Address >
	__forceinline static t get_method( Address this_ptr, size_t index ) {
		return ( t )this_ptr.to< t* >( )[ index ];
	}

};
