#pragma once

class Stack : public Address
{
public:
	// ctor.
	__forceinline Stack() : Address{ *(uintptr_t *)(_AddressOfReturnAddress()) - sizeof(uintptr_t) } { }

	// get return address.
	__forceinline Address get_return_address() {
		if (!m_ptr)
			return Address{};

		return *(uintptr_t*) (m_ptr + sizeof(uintptr_t));
	}

	// go up stack frame.
	__forceinline Stack& next() {
		return *(Stack*) m_ptr;
	}

	// get local variable.
	template< typename t = Address >
	__forceinline t get_local(size_t offset) {
		return (t) (m_ptr - offset);
	}

	// get arg.
	template< typename t = Address >
	__forceinline t get_arg(size_t offset) {
		return (t) (m_ptr + offset);
	}
};
