#pragma once
#include "chandle.h"

class IClientEntityListener {
public:
	virtual void OnEntityCreated( C_BaseEntity *pEntity ) {}
	virtual void OnEntityDeleted( C_BaseEntity *pEntity ) {}
};

class IClientEntityList {
public:
	virtual IClientNetworkable *GetClientNetworkable( int entnum ) = 0;
	virtual void *vtablepad0x1( ) = 0;
	virtual void *vtablepad0x2( ) = 0;
	virtual IClientEntity *GetClientEntity( int entNum ) = 0;
	virtual IClientEntity *GetClientEntityFromHandle( CBaseHandle hEnt ) = 0;
	virtual int NumberOfEntities( bool bIncludeNonNetworkable ) = 0;
	virtual int GetHighestEntityIndex( ) = 0;
	virtual void SetMaxEntities( int maxEnts ) = 0;
	virtual int GetMaxEntities( ) = 0;

	template< class T >
	T *Get( int index ) {
		return reinterpret_cast< T * >( GetClientEntity( index ) );
	}

	template< class T >
	T *Get( CBaseHandle hEnt ) {
		return reinterpret_cast< T * >( GetClientEntityFromHandle( hEnt ) );
	}

	void AddListenerEntity( IClientEntityListener *pListener ) {
		m_entityListeners.AddToTail( pListener );
	}

	void RemoveListenerEntity( IClientEntityListener *pListener ) {
		m_entityListeners.FindAndRemove( pListener );
	}

private:
	CUtlVector< IClientEntityListener* > m_entityListeners;
};
