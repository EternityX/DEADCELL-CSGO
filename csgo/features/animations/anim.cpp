#include "anim.h"

c_animations g_anim;

// never finished.

void c_animations::Init( ) {
	g_csgo.m_entity_list->AddListenerEntity( this );
}

void c_animations::Remove( ) {
	g_csgo.m_entity_list->RemoveListenerEntity( this );
}

void c_animations::OnEntityCreated( C_BaseEntity *ent ) {
	if( !ent )
		return;

	int index = ent->GetIndex( );
	if( index < 0 )
		return;

	ClientClass *cc = ent->GetClientClass( );
	if( !cc )
		return;

	switch( cc->m_ClassID ) {
		case CCSPlayer: {

			m_track[ index ].m_index = index;
			m_track[ index ].m_vmt = std::make_unique< c_vmt >( ent );
			m_track[ index ].m_vmt->hook_method( 193, animations::DoExtraBonesProcessing );
			m_track[ index ].m_hooked = true;

			break;
		}
		default: {
			return;
		}
	}

}

void c_animations::OnEntityDeleted( C_BaseEntity *ent ) {
	if( !ent )
		return;

	int index = ent->GetIndex( );
	if( index < 0 )
		return;

	auto it = std::find_if( m_track.begin( ), m_track.end( ), [ & ]( const Container &data ) {
		return data.m_index == index;
	} );

	if( it == m_track.end( ) )
		return;

	if( m_track[ it->m_index ].m_hooked )
		m_track[ it->m_index ].m_vmt->unhook_all( );
}