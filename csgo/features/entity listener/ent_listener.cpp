#include "ent_listener.h"

c_custom_listener g_listener;

void c_custom_listener::init( ) {
	g_csgo.m_entity_list->add_listener_entity( this );
}

void c_custom_listener::remove( ) {
	g_csgo.m_entity_list->remove_listener_entity( this );
}

void c_custom_listener::on_entity_created( c_base_entity *ent ) {
	if( !ent )
		return;

	const int idx = ent->get_index( );
	if( idx < 0 )
		return;

	client_class *cc = ent->get_client_class( );
	if( !cc )
		return;

	switch( cc->m_class_id ) {
		case CCSPlayer: {

			m_players.emplace_back( ent_container_t( ent ) );

			break;
		}
		default:
			break;
	}
}

void c_custom_listener::on_entity_deleted( c_base_entity *ent ) {
	if( !ent )
		return;

	int idx = ent->get_index( );
	if( idx < 0 )
		return;

	const auto it = std::find_if( m_players.begin( ), m_players.end( ), [ & ]( const ent_container_t &data ) {
		return data.m_idx == idx;
	} );

	if ( it == m_players.end( ) )
		return;

	m_players.erase( it );
}