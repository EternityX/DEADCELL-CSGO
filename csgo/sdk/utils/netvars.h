#pragma once

class c_netvars {
private:
	struct netvar_data {
		bool m_datamap_var; // we can't do proxies on stuff from datamaps :).
		RecvProp *m_prop_ptr;
		size_t m_offset;

		netvar_data() : m_datamap_var{ }, m_prop_ptr{ }, m_offset{ } { }
	};

	std::unordered_map< hash32_t, int > m_client_ids;

	// netvar container.
	std::unordered_map< hash32_t, // hash key + tables
	                    std::unordered_map< hash32_t, // hash key + props
	                                        netvar_data // prop offset / prop ptr
	                    > > m_offsets;

public:
	// ctor.
	c_netvars() : m_offsets{ } { }

	// dtor.
	~c_netvars() {
		m_offsets.clear();
	}

	void init( ) {
		ClientClass *list;

		// sanity check on client->
		if( !g_csgo.m_client )
			return;

		// grab linked list.
		list = g_csgo.m_client->GetAllClasses();
		if( !list )
			return;

		// iterate list of netvars.
		for( ; list != nullptr; list = list->m_pNext ) {
			store_table( list->m_pRecvTable->m_pNetTableName, list->m_pRecvTable );
		}
			
	}

	void store_table( const std::string &name, RecvTable *table, size_t offset = 0 ) {
		std::ofstream fs( "vars.txt", std::ios::out | std::ios::app );

		hash32_t var, base{ util::hash::fnv1a_32( name ) };
		RecvProp *prop;
		RecvTable *child;

		// iterate props
		for( int i{ }; i < table->m_nProps; ++i ) {
			prop = &table->m_pProps[ i ];
			child = prop->m_pDataTable;

			// we have a child table, that contains props.
			if( child && child->m_nProps > 0 )
				store_table( name, child, prop->m_Offset + offset );

			// hash var name.
			var = util::hash::fnv1a_32( prop->m_pVarName );

			// insert if not present.
			if( !m_offsets[ base ][ var ].m_offset ) {
				fs << "> " << prop->m_pVarName << ": ";
				fs << "0x" << std::setprecision( 4 ) << std::hex << std::uppercase << (size_t)( prop->m_Offset + offset ) << std::endl;

				m_offsets[ base ][ var ].m_datamap_var = false;
				m_offsets[ base ][ var ].m_prop_ptr = prop;
				m_offsets[ base ][ var ].m_offset = (size_t)( prop->m_Offset + offset );
			}
		}
	}

	// get client id.
	int get_client_id( hash32_t network_name ) {
		return m_client_ids[ network_name ];
	}

	// get netvar offset.
	int get( hash32_t table, hash32_t prop ) {
		return m_offsets[ table ][ prop ].m_offset;
	}

	int get_offset( const char *table, const char *prop ) {
		auto tb = util::hash::fnv1a_32( table );
		auto p = util::hash::fnv1a_32( prop );
		return m_offsets[ tb ][ p ].m_offset;
	}
};

#define NETVAR( t, n, tb, p )\
t &n( )\
{\
	static auto offset = g_netvars.get_offset( tb, p );\
	return *( t * )( (uintptr_t)this + offset );\
}

#define PNETVAR( t, n, tb, p )\
t *n( )\
{\
	static auto offset = g_netvars.get_offset( tb, p );\
	return ( t * )( (uintptr_t)this + offset );\
}

#define OFFSET( t, n, o )\
t &n( )\
{\
	return *( t * )( (uintptr_t)this + o );\
}

extern c_netvars g_netvars;
