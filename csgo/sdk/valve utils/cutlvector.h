#pragma once
#include <cassert>

inline int UtlMemory_CalcNewAllocationCount( int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem ) {
	if( nGrowSize ) {
		nAllocationCount = ( ( 1 + ( ( nNewSize - 1 ) / nGrowSize ) ) * nGrowSize );
	}
	else {
		if( !nAllocationCount ) {
			// Compute an allocation which is at least as big as a cache line...
			nAllocationCount = ( 31 + nBytesItem ) / nBytesItem;
		}

		while( nAllocationCount < nNewSize ) {
			nAllocationCount *= 2;
		}
	}

	return nAllocationCount;
}

template< class T, class I = int >
class CUtlMemory {
public:
	bool IsIdxValid( I i ) const {
		long x = i;
		return ( x >= 0 ) && ( x < m_nAllocationCount );
	}

	T &operator[]( I i );
	const T &operator[]( I i ) const;

	T *Base( ) {
		return m_pMemory;
	}

	int NumAllocated( ) const {
		return m_nAllocationCount;
	}

	void Grow( int num ) {
		assert( num > 0 );

		auto oldAllocationCount = m_nAllocationCount;
		// Make sure we have at least numallocated + num allocations.
		// Use the grow rules specified for this memory (in m_nGrowSize)
		int nAllocationRequested = m_nAllocationCount + num;

		int nNewAllocationCount = UtlMemory_CalcNewAllocationCount( m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof( T ) );

		// if m_nAllocationRequested wraps index type I, recalculate
		if( ( int )( I )nNewAllocationCount < nAllocationRequested ) {
			if( ( int )( I )nNewAllocationCount == 0 && ( int )( I )( nNewAllocationCount - 1 ) >= nAllocationRequested ) {
				--nNewAllocationCount; // deal w/ the common case of m_nAllocationCount == MAX_USHORT + 1
			}
			else {
				if( ( int )( I )nAllocationRequested != nAllocationRequested ) {
					// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
					assert( 0 );
					return;
				}
				while( ( int )( I )nNewAllocationCount < nAllocationRequested ) {
					nNewAllocationCount = ( nNewAllocationCount + nAllocationRequested ) / 2;
				}
			}
		}

		m_nAllocationCount = nNewAllocationCount;

		if( m_pMemory ) {
			auto ptr = new unsigned char[ m_nAllocationCount * sizeof( T ) ];

			memcpy( ptr, m_pMemory, oldAllocationCount * sizeof( T ) );
			m_pMemory = ( T* )ptr;
		}
		else {
			m_pMemory = ( T* )new unsigned char[ m_nAllocationCount * sizeof( T ) ];
		}
	}

protected:
	T *m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template< class T, class I >
T &CUtlMemory< T, I >::operator[]( I i ) {
	assert( IsIdxValid( i ) );
	return m_pMemory[ i ];
}

template< class T, class I >
const T &CUtlMemory< T, I >::operator[]( I i ) const {
	assert( IsIdxValid( i ) );
	return m_pMemory[ i ];
}

template< class T >
void Destruct( T *pMemory ) {
	pMemory->~T( );
}

template< class T >
T *Construct( T *pMemory ) {
	return ::new( pMemory ) T;
}

template< class T >
T *CopyConstruct( T *pMemory, T const &src ) {
	return ::new( pMemory ) T( src );
}

template< class T, class A = CUtlMemory< T > >
class CUtlVector {
	typedef A CAllocator;

	typedef T *iterator;
	typedef const T *const_iterator;
public:
	T &operator[]( int i );
	const T &operator[]( int i ) const;

	T &Element( int i ) {
		return m_Memory[ i ];
	}

	const T &Element( int i ) const {
		assert( IsValidIndex( i ) );
		return m_Memory[ i ];
	}

	T *Base( ) {
		return m_Memory.Base( );
	}

	int Count( ) const {
		return m_Size;
	}

	void RemoveAll( ) {
		for( int i = m_Size; --i >= 0; )
			Destruct( &Element( i ) );

		m_Size = 0;
	}

	bool IsValidIndex( int i ) const {
		return ( i >= 0 ) && ( i < m_Size );
	}

	void ShiftElementsRight( int elem, int num = 1 ) {
		assert( IsValidIndex( elem ) || ( m_Size == 0 ) || ( num == 0 ) );
		int numToMove = m_Size - elem - num;
		if( ( numToMove > 0 ) && ( num > 0 ) )
			memmove( &Element( elem + num ), &Element( elem ), numToMove * sizeof( T ) );
	}

	void ShiftElementsLeft( int elem, int num = 1 ) {
		assert( IsValidIndex(elem) || ( m_Size == 0 ) || ( num == 0 ));
		int numToMove = m_Size - elem - num;
		if( ( numToMove > 0 ) && ( num > 0 ) )
			memmove( &Element( elem ), &Element( elem + num ), numToMove * sizeof( T ) );
	}

	void GrowVector( int num = 1 ) {
		if( m_Size + num > m_Memory.NumAllocated( ) ) {
			m_Memory.Grow( m_Size + num - m_Memory.NumAllocated( ) );
		}

		m_Size += num;
	}

	int InsertBefore( int elem ) {
		// Can insert at the end
		assert( ( elem == Count() ) || IsValidIndex( elem ) );

		GrowVector( );
		ShiftElementsRight( elem );
		Construct( &Element( elem ) );
		return elem;
	}

	int AddToHead( ) {
		return InsertBefore( 0 );
	}

	int AddToTail( ) {
		return InsertBefore( m_Size );
	}

	int InsertBefore( int elem, const T &src ) {
		// Can't insert something that's in the list... reallocation may hose us
		assert( (Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count()) ) );

		// Can insert at the end
		assert( (elem == Count()) || IsValidIndex(elem) );

		GrowVector( );
		ShiftElementsRight( elem );
		CopyConstruct( &Element( elem ), src );
		return elem;
	}

	int AddToTail( const T &src ) {
		// Can't insert something that's in the list... reallocation may hose us
		return InsertBefore( m_Size, src );
	}

	int Find( const T &src ) const {
		for( int i = 0; i < Count( ); ++i ) {
			if( Element( i ) == src )
				return i;
		}
		return -1;
	}

	void Remove( int elem ) {
		Destruct( &Element( elem ) );
		ShiftElementsLeft( elem );
		--m_Size;
	}

	bool FindAndRemove( const T &src ) {
		int elem = Find( src );
		if( elem != -1 ) {
			Remove( elem );
			return true;
		}
		return false;
	}

	iterator begin( ) {
		return Base( );
	}

	const_iterator begin( ) const {
		return Base( );
	}

	iterator end( ) {
		return Base( ) + Count( );
	}

	const_iterator end( ) const {
		return Base( ) + Count( );
	}

protected:
	CAllocator m_Memory;
	int m_Size;
	T *m_pElements;
};

template< typename T, class A >
T &CUtlVector< T, A >::operator[]( int i ) {
	assert( i < m_Size );
	return m_Memory[ i ];
}

template< typename T, class A >
const T &CUtlVector< T, A >::operator[]( int i ) const {
	assert( i < m_Size );
	return m_Memory[ i ];
}
