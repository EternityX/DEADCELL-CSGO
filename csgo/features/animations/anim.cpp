#include "anim.h"

c_animations g_anim;

// never finished.

void c_animations::init( ) {
	g_csgo.m_entity_list->add_listener_entity( this );
}

void c_animations::remove( ) {
	g_csgo.m_entity_list->remove_listener_entity( this );

	for( int i = 0; i < 64; i++ ) {
		if( m_track.at( i ).m_hooked ) {
			m_track.at( i ).m_vmt->unhook_all( );
			m_track.at( i ).m_renderable_vmt->unhook_all( );
		}
	}
}

void c_animations::on_entity_created( c_base_entity *ent ) {
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

			m_track.at( idx ) = container_t( ent );
			break;
		}
		default:
			break;
	}
}

void c_animations::on_entity_deleted( c_base_entity *ent ) {
	if( !ent )
		return;

	int idx = ent->get_index( );
	if( idx < 0 )
		return;

	const auto it = std::find_if( m_track.begin( ), m_track.end( ), [ & ]( const container_t &data ) {
		return data.m_idx == idx;
	} );

	if( it == m_track.end( ) )
		return;

	if( m_track.at( it->m_idx ).m_hooked ){
		m_track.at( it->m_idx ).m_vmt->unhook_all( );
		m_track.at( it->m_idx ).m_renderable_vmt->unhook_all( );
		m_track.at( it->m_idx ).m_hooked = false;
	}
		
}
