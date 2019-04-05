#pragma once

class c_netvars {
private:
	struct netvar_data {
		bool m_datamap_var; // we can't do proxies on stuff from datamaps :).
		recv_prop *m_prop_ptr;
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
	c_netvars() {
		
	}

	// dtor.
	~c_netvars() {
		m_offsets.clear();
	}

	bool init( ) {
		// sanity check on client->
		if( !g_csgo.m_client )
			return false;

		// grab linked list.
		client_class *list = g_csgo.m_client->get_all_classes( );
		if( !list )
			return false;

		// iterate list of netvars.
		for( ; list != nullptr; list = list->m_next )
			store_table( list->m_recv_table->m_net_table_name, list->m_recv_table );

		return true;
	}

	void store_table( const std::string &name, recv_table *table, size_t offset = 0 ) {
		const hash32_t base{ util::hash::fnv1a_32( name ) };

		// iterate props
		for( int i{ }; i < table->m_num_props; ++i ) {
			recv_prop *prop = &table->m_props[ i ];
			recv_table *child = prop->m_data_table;

			// we have a child table, that contains props.
			if( child && child->m_num_props > 0 )
				store_table( name, child, prop->m_offset + offset );

			// hash var name.
			hash32_t var = util::hash::fnv1a_32( prop->m_var_name );

			// insert if not present.
			if( !m_offsets[ base ][ var ].m_offset ) {
				m_offsets[ base ][ var ].m_datamap_var = false;
				m_offsets[ base ][ var ].m_prop_ptr = prop;
				m_offsets[ base ][ var ].m_offset = static_cast< size_t >( prop->m_offset + offset );
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
		const auto tb = util::hash::fnv1a_32( table );
		const auto p = util::hash::fnv1a_32( prop );
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
