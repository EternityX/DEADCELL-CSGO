#pragma once

class Address {
protected:
	uintptr_t m_ptr;

public:
	// default c/dtor
	__forceinline Address( ) : m_ptr{} { };
	__forceinline ~Address( ) = default;

	__forceinline Address( uintptr_t a ) :
		m_ptr{ a } { }

	__forceinline Address( const void *a ) :
		m_ptr{ reinterpret_cast< uintptr_t >( a ) } { }

	__forceinline operator uintptr_t( ) const {
		return m_ptr;
	}

	__forceinline operator void*( ) const {
		return reinterpret_cast< void* >( m_ptr );
	}

	__forceinline operator const void*( ) const {
		return reinterpret_cast< const void* >( m_ptr );
	}

	// to is like as but dereferences.
	template< typename t = Address >
	__forceinline t to( ) const {
		return *reinterpret_cast< t* >( m_ptr );
	}

	template< typename t = Address >
	__forceinline t as( ) const {
		return reinterpret_cast< t >( m_ptr );
	}

	template< typename t = Address >
	__forceinline t at( ptrdiff_t offset ) const {
		return *reinterpret_cast< t* >( m_ptr + offset );
	}

	template< typename t = Address >
	__forceinline t add( ptrdiff_t offset ) const {
		return reinterpret_cast< t >( m_ptr + offset );
	}

	template< typename t = Address >
	__forceinline t sub( ptrdiff_t offset ) const {
		return reinterpret_cast< t >( m_ptr - offset );
	}

	template< typename t = Address >
	__forceinline t get( size_t dereferences = 1 ) {
		return reinterpret_cast< t >( get_( dereferences ) );
	}

	template< typename t = Address >
	__forceinline void set( t val ) {
		*reinterpret_cast< t* >( m_ptr ) = val;
	}

	template< typename t = Address >
	__forceinline t rel( size_t offset = 0 ) {

		uintptr_t out = m_ptr + offset;

		uint32_t rel = *reinterpret_cast< uint32_t * >( out );
		if( !rel )
			return t{};

		out = ( out + 0x4 ) + rel;

		return reinterpret_cast< t >( out );
	}

	__forceinline static bool safe( Address to_check ) {
		static MEMORY_BASIC_INFORMATION32 mbi{};

		if( !to_check
			|| to_check < 0x10000
			|| to_check > 0xFFE00000
			|| !VirtualQuery( to_check, reinterpret_cast< PMEMORY_BASIC_INFORMATION >( &mbi ), sizeof( mbi ) ) )
			return false;

		if( !mbi.AllocationBase
			|| mbi.State != MEM_COMMIT
			|| mbi.Protect == PAGE_NOACCESS
			|| mbi.Protect & PAGE_GUARD )
			return false;

		return true;
	}

private:
	__forceinline uintptr_t get_( size_t dereferences ) {
		uintptr_t temp = m_ptr;

		while( dereferences-- && safe( temp ) )
			temp = *reinterpret_cast< uintptr_t* >( temp );

		return temp;
	}
};
