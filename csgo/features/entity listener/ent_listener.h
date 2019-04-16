#pragma once
#include "../../inc.h"

struct ent_container_t {
	ent_container_t( ) { }
	ent_container_t( c_base_entity* ent ) {
		m_idx = ent->get_index( );
		m_player = static_cast< c_csplayer* >( ent );
	}
	c_csplayer* m_player;
	int m_idx;
};

class c_custom_listener : public i_client_entity_listener {
public:
	void on_entity_created(c_base_entity* ent) override;
	void on_entity_deleted(c_base_entity* ent) override;

	void init();
	void remove();

	std::vector< ent_container_t > m_players;
};

extern c_custom_listener g_listener;