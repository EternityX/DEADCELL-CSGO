#pragma once

namespace pe {
	// container type for "get_all_modules" function.
	using modules_t = std::vector< Module >;

	// get Thread Environment Block.
	__forceinline types::TEB *get_TEB() {
		return reinterpret_cast< pe::types::TEB * >( __readfsdword( 0x18 ) );
	}

	// get Thread Environment Block.
	__forceinline types::PEB *get_PEB() {
		auto teb{ get_TEB() };
		if( !teb )
			return nullptr;

		return teb->ProcessEnvironmentBlock;
	}

	// returns a vector containing all modules in current process.
	static bool get_all_modules( modules_t &out ) {
		Module mod;

		types::PEB *peb = get_PEB( );
		if( !peb )
			return false;

		// valid list?
		if( !peb->Ldr->InMemoryOrderModuleList.Flink )
			return false;

		types::LIST_ENTRY *list = &peb->Ldr->InMemoryOrderModuleList;

		// iterate doubly linked list.
		for( auto i = list->Flink; i != list; i = i->Flink ) {
			// get current entry.
			types::LDR_DATA_TABLE_ENTRY *ldr_entry = CONTAINING_RECORD( i, types::LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
			if( !ldr_entry )
				continue;

			// attempt to initialize current module.
			if( !mod.init( ldr_entry ) )
				continue;

			out.push_back( mod );
		}

		return !out.empty();
	}

	// get module by name hash.
	static pe::Module get_module( hash32_t hash ) {
		// PE::Types::PEB  *peb;
		modules_t modules;

		if( !get_all_modules( modules ) )
			return { };

		for( const auto &m : modules ) {
			if( util::hash::fnv1a_32( m.get_module_nameA() ) == hash )
				return m;
		}

		return { };
	}

	// get module by name.
	static pe::Module get_module( std::string name ) {
		if( name.empty() )
			return get_module( 0 );

		std::transform( name.begin(), name.end(), name.begin(), tolower );

		return get_module( util::hash::fnv1a_32( name ) );
	}

	// get export by hash.
	template< typename t = uintptr_t >
	static t get_export( const pe::Module &mod, hash32_t hash ) {
		if( !mod )
			return t{ };

		// get export data directory info.
		auto export_dir = mod.get_data_dir< IMAGE_EXPORT_DIRECTORY >( IMAGE_DIRECTORY_ENTRY_EXPORT );
		if( !export_dir )
			return t{ };

		// get needed arrays.
		auto names = mod.RVA< uint32_t * >( export_dir.m_va_ptr->AddressOfNames );
		auto funcs = mod.RVA< uint32_t * >( export_dir.m_va_ptr->AddressOfFunctions );
		auto ords = mod.RVA< uint16_t * >( export_dir.m_va_ptr->AddressOfNameOrdinals );
		if( !names || !funcs || !ords )
			return t{ };

		for( size_t i = 0; i < export_dir.m_va_ptr->NumberOfNames; ++i ) {
			std::string export_name = mod.RVA< const char * >( names[ i ] );
			if( export_name.empty() )
				continue;

			if( util::hash::fnv1a_32( export_name ) == hash ) {
				uintptr_t export_ptr = mod.RVA( funcs[ ords[ i ] ] );
				if( !export_ptr )
					continue;

				// if the export ptr is inside the dir,  then it's a fowarder export and we must resolve it.
				if( export_ptr >= reinterpret_cast< uintptr_t >( export_dir.m_va_ptr ) && export_ptr < ( reinterpret_cast< uintptr_t >( export_dir.m_va_ptr ) + export_dir.
					m_size ) ) {
					// get forwarder string.
					std::string fwd_str = reinterpret_cast< const char * >( export_ptr );

					size_t delim = fwd_str.find_last_of( '.' );
					if( delim == std::string::npos )
						return t{ };

					// get forwarder module name.
					std::string fwd_module_name = fwd_str.substr( 0, delim + 1 );
					fwd_module_name += "dll";

					// get forwarder export name.
					std::string fwd_export_name = fwd_str.substr( delim );

					// get real export ptr (recursively).
					export_ptr = get_export( get_module( fwd_module_name ), fwd_export_name );
					if( !export_ptr )
						return { };

					return (t)( export_ptr );
				}

				return (t)( export_ptr );
			}
		}

		return t{ };
	}

	// get export by name.
	template< typename t = uintptr_t >
	static t get_export( const pe::Module &mod, const std::string &name ) {
		return get_export< t >( mod, util::hash::fnv1a_32( name ) );
	}
}
