#pragma once

namespace pe {
	static bool get_file_headers( uintptr_t base, PIMAGE_DOS_HEADER &out_dos, PIMAGE_NT_HEADERS &out_nt );

	template< typename t >
	class DataEntry_t {
	public:
		uintptr_t m_va; // virtual offset.
		uintptr_t m_size; // size of data entry.
		t *m_va_ptr; // virtual offset ptr.
		// uintptr_t m_end;  // end ptr of data entry.

		DataEntry_t() : m_va{ 0 }, m_size{ 0 }, m_va_ptr{ nullptr } { }

		// valid checks.
		explicit operator bool() const {
			return m_va;
		}

		bool operator !() const {
			return !m_va;
		}
	};

	class Module {
	private:
		types::LDR_DATA_TABLE_ENTRY *m_ldr_entry;
		IMAGE_DOS_HEADER *m_dos;
		IMAGE_NT_HEADERS *m_nt;
		uintptr_t m_base;
		std::wstring m_pathW;

	public:
		Module();

		bool init( types::LDR_DATA_TABLE_ENTRY *ldr_entry );

		uintptr_t get_base() const;
		IMAGE_DOS_HEADER *get_dos_header() const;
		IMAGE_NT_HEADERS *get_nt_headers() const;
		size_t get_code_size() const;
		size_t get_image_size() const;
		uintptr_t get_code_base() const;
		std::wstring get_pathW() const;
		std::string get_pathA() const;
		std::wstring get_module_nameW() const;
		std::string get_module_nameA() const;

		explicit operator uintptr_t() const {
			return m_base;
		}

		// valid checks.
		explicit operator bool() const {
			return get_base();
		}

		bool operator !() const {
			return !get_base();
		}

		template< typename t = uintptr_t >
		t RVA( size_t offset ) const {
			if( !m_base || !offset )
				return t{ };

			return (t)( m_base + offset );
		}

		// returns info about a data directory entry from OptionalHeader->DataDirectory array.
		template< typename t >
		DataEntry_t< t > get_data_dir( size_t entry ) const {
			DataEntry_t< t > out;

			if( !m_nt || entry > IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
				return { };

			auto data_dir = &m_nt->OptionalHeader.DataDirectory[ entry ];
			if( !data_dir )
				return { };

			out.m_va = data_dir->VirtualAddress;
			out.m_size = data_dir->Size;
			out.m_va_ptr = RVA< t * >( data_dir->VirtualAddress );;

			return out;
		}
	};
}
