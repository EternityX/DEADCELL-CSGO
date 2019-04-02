#pragma once

namespace util {
	using NtCreateThreadEx_t = long(__stdcall*)(PHANDLE, ACCESS_MASK, PVOID, HANDLE, LPTHREAD_START_ROUTINE, PVOID, ULONG, ULONG_PTR, SIZE_T, SIZE_T, PVOID);
	using DisableThreadLibraryCalls_t = long(__stdcall*)(HMODULE);
	using CloseHandle_t = long(__stdcall*)(HANDLE);
	using GetCurrentProcess_t = HANDLE(__stdcall*)(void);

	namespace mem {
		__forceinline static HANDLE get_current_process() {
			static GetCurrentProcess_t get_cur_process{};

			if (get_cur_process == nullptr)
				get_cur_process = pe::get_export<GetCurrentProcess_t>( pe::get_module( "kernel32.dll" ), "GetCurrentProcess" );

			if (get_cur_process != nullptr)
				return get_cur_process();

			return INVALID_HANDLE_VALUE;
		}

		__forceinline static HWND find_window( const std::string &window_name ) {
			static decltype(&FindWindowA) find_window{};

			if (find_window == nullptr)
				find_window = pe::get_export< decltype( &FindWindowA ) >( pe::get_module( "user32.dll" ), "FindWindowA" );

			if (find_window != nullptr)
				return find_window(window_name.c_str(), nullptr);

			return 0;
		}

		// sneaky way of invoking ntcreatethreadex in DllMain
		__forceinline static HANDLE create_thread(LPTHREAD_START_ROUTINE routine) {
			static NtCreateThreadEx_t create_thread_ex = nullptr;

			HANDLE                    out = INVALID_HANDLE_VALUE;

			if (create_thread_ex == nullptr)
				create_thread_ex = pe::get_export< NtCreateThreadEx_t >( pe::get_module( "ntdll.dll" ), "NtCreateThreadEx" );

			if (create_thread_ex != nullptr) {
				create_thread_ex(
					&out,
					THREAD_ALL_ACCESS,
					nullptr,
					get_current_process(),
					routine,
					nullptr,
					0x4,
					0,
					0,
					0,
					nullptr
				);
			}

			return out;
		}

		// sneaky way of closing a handle
		__forceinline static bool close_handle(HANDLE handle) {
			static CloseHandle_t close_handle{};

			if (close_handle == nullptr)
				close_handle = pe::get_export< CloseHandle_t >( pe::get_module( "kernel32.dll" ), "CloseHandle" );

			if (close_handle != nullptr)
				return !!close_handle(handle);

			return false;
		}

		// sneaky way of disabling thread lib calls in dllmain
		__forceinline static bool disable_thread_lib_calls(HMODULE inst) {
			static DisableThreadLibraryCalls_t disable_calls{};

			if (disable_calls == nullptr)
				disable_calls = pe::get_export< DisableThreadLibraryCalls_t >( pe::get_module( "kernel32.dll" ), "DisableThreadLibraryCalls" );

			if (disable_calls != nullptr)
				return !!disable_calls(inst);

			return false;
		}
	}

	namespace misc {
		bool valid_code_ptr( uintptr_t addr );

		std::string wide_to_multibyte( const std::wstring &str );
		std::wstring multibyte_to_wide( const std::string &str );
		std::string unicode_to_ascii( const std::wstring& unicode );
		std::wstring ascii_to_unicode( const std::string& ascii );

		// get method from VMT.
		template< typename t >
		t vfunc( void *this_ptr, uint16_t index ) {
			return ( *static_cast< t ** >(this_ptr) )[ index ];
		}

		// follow relative32 offset.
		// input argument is the address of the relative offset.
		template< typename t = uintptr_t >
		t follow_rel32( uintptr_t addr ) {
			if( !addr )
				return 0;

			// get rel32 offset.
			uint32_t disp = *reinterpret_cast< uint32_t * >( addr );
			if( !disp )
				return 0;

			// displacement is relative to address of the next instruction.
			return ( addr + sizeof( uint32_t ) ) + disp;
		}

		// returns a random floating point value in range.
		template< typename t >
		t get_random_float_range( const t &min, const t &max ) {
			static_assert( std::is_floating_point< t >::value, "invalid type for: get_random_float_range." );

			std::random_device rd;
			std::mt19937 e2( rd() );
			std::uniform_real_distribution< t > distribution( min, max );

			return distribution( e2 );
		}

		// returns a random integral value in range.
		template< typename t >
		t get_random_int_range( const t &min, const t &max ) {
			static_assert( std::is_integral< t >::value, "invalid type for: get_random_int_range." );

			std::random_device rd;
			std::mt19937 e2( rd() );
			std::uniform_int_distribution< t > distribution( min, max );

			return distribution( e2 );
		}
		template < typename t > 
		t clamp( t in, t low, t high ) {
			return std::min( std::max( in, low ), high );
		}
		class Timer {
		private:
			std::chrono::high_resolution_clock::time_point m_start;

		public:
			Timer( bool start = false ) {
				if( start )
					reset();
			}

			void start() {
				reset();
			}

			void reset() {
				m_start = std::chrono::high_resolution_clock::now();
			}

			ulong_t elapsed() {
				auto elapsed = std::chrono::high_resolution_clock::now() - m_start;
				return std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count();
			}
		};
	}
}
