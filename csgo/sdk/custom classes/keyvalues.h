#pragma once


class KeyValues {
public:
	~KeyValues( ){
		g_csgo.m_memalloc->Free( this );
	}

	void init_key_values( const char *key_name ) {
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
};