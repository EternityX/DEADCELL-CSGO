#include "anim.h"

c_animations g_anim;

// never finished.

bool c_animations::init( ) {
	if( !g_csgo.m_entity_list ) // should probably check for validity in interface_manager.h instead
		return false;

	g_csgo.m_entity_list->AddListenerEntity( this );

	return true;
}

void c_animations::remove( ) {
	g_csgo.m_entity_list->RemoveListenerEntity( this );

	for( int i = 0; i < 64; i++ ) {
		if( m_track[ i ].m_hooked )
			m_track[ i ].m_vmt->unhook_all( );
	}
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
			m_track[ index ].m_vmt->hook_method( hook::idx::DO_EXTRA_BONE_PROC, animations::DoExtraBonesProcessing );
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

	auto it = std::find_if( m_track.begin( ), m_track.end( ), [ & ]( const container_t &data ) {
		return data.m_index == index;
	} );

	if( it == m_track.end( ) )
		return;

	if( m_track[ it->m_index ].m_hooked )
		m_track[ it->m_index ].m_vmt->unhook_all( );
}