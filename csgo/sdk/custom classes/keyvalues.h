#pragma once


class key_values {
public:
	~key_values( ){
		g_csgo.m_memalloc->free( this );
	}

	void init( const char *key_name ) {
		using InitKeyValues_t = void(__thiscall *)( void *, const char * );
		static InitKeyValues_t InitKeyValuesEx = nullptr;

		if( !InitKeyValuesEx )
			InitKeyValuesEx = pattern::find< InitKeyValues_t >( g_csgo.m_client_dll, "55 8B EC 51 33 C0 C7 45" );

		InitKeyValuesEx( this, key_name );
	}

	void load_from_buffer( const char *resource_name, const char *buf, void *file_sys = nullptr, const char *path_id = nullptr, void *eval_sym_proc = nullptr, void *unk = nullptr ){
		using LoadFromBuffer_t = void(__thiscall *)( void *, const char *, const char *, void *, const char *, void *, void * );
		static LoadFromBuffer_t LoadFromBufferEx = nullptr;

		if( !LoadFromBufferEx )
			LoadFromBufferEx = pattern::find< LoadFromBuffer_t >( g_csgo.m_client_dll, "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" );

		LoadFromBufferEx( this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk );
	}

	key_values *find_key( const char *name, bool bCreate ){
		using findkey_t = key_values*( __thiscall* )( key_values*, const char*, bool );
		static auto FindKey = pattern::find< findkey_t >( g_csgo.m_client_dll, "55 8B EC 83 EC 1C 53 8B D9 85 DB" );
		return FindKey( this, name, bCreate );
	}

	void set_int( const char *keyName, int val ) {
		auto key_int = find_key( keyName, true );
		if( key_int ) {
			*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( key_int ) + 0xC ) = val;
			*reinterpret_cast< char* >( reinterpret_cast< uintptr_t >( key_int ) + 0x10 ) = 2;
		}
	}

	void set_string( const char *name, const char* a2 ){
		auto key = find_key(name, true);
		using setstring_t =  void( __thiscall* )( void*, const char* );
		if ( key ) {
			static auto SetString = pattern::find< setstring_t >( g_csgo.m_client_dll, "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01" );
			SetString( key, a2) ;
		}
	}
};