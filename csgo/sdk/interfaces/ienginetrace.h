#pragma once

class ITraceFilter {
public:
	virtual bool ShouldHitEntity( IHandleEntity *pEntity, int contentsMask ) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter {
public:
	void *m_skip;
	char *m_ignore = "";

	bool ShouldHitEntity( IHandleEntity *pEntityHandle, int /*contentsMask*/ ) override {
		ClientClass *ent_cc = ( (IClientEntity *)pEntityHandle )->GetClientClass();
		if( ent_cc && strcmp( m_ignore, "" ) ) {
			if( ent_cc->m_pNetworkName == m_ignore )
				return false;
		}

		return !( pEntityHandle == m_skip );
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_EVERYTHING;
	}

	void SetIgnoreClass( char *cc ) {
		m_ignore = cc;
	}
};

class CTraceFilterSkipEntity : public CTraceFilter {
public:
	void *m_skip;

	CTraceFilterSkipEntity( IHandleEntity *pEntityHandle ) {
		m_skip = pEntityHandle;
	}

	bool ShouldHitEntity( IHandleEntity *pEntityHandle, int /*contentsMask*/ ) override {
		return !( pEntityHandle == m_skip );
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_EVERYTHING;
	}
};

class CTraceFilterEntitiesOnly : public CTraceFilter {
public:
	bool ShouldHitEntity( IHandleEntity *pEntityHandle, int /*contentsMask*/ ) override {
		return true;
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_ENTITIES_ONLY;
	}
};

class CTraceFilterWorldOnly : public CTraceFilter {
public:
	bool ShouldHitEntity( IHandleEntity * /*pServerEntity*/, int /*contentsMask*/ ) override {
		return false;
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_WORLD_ONLY;
	}
};

class CTraceFilterWorldAndPropsOnly : public CTraceFilter {
public:
	bool ShouldHitEntity( IHandleEntity * /*pServerEntity*/, int /*contentsMask*/ ) override {
		return false;
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_EVERYTHING;
	}
};

class CTraceFilterPlayersOnlySkipOne : public CTraceFilter {
public:
	CTraceFilterPlayersOnlySkipOne( IClientEntity *ent ) {
		pEnt = ent;
	}

	bool ShouldHitEntity( IHandleEntity *pEntityHandle, int /*contentsMask*/ ) override {
		return pEntityHandle != pEnt && ( (IClientEntity*)pEntityHandle )->GetClientClass()->m_ClassID == CCSPlayer;
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_ENTITIES_ONLY;
	}

private:
	IClientEntity *pEnt;
};

typedef bool (*ShouldHitFunc_t)( IHandleEntity *pHandleEntity, int contentsMask );

class CTraceFilterSimple : public CTraceFilter {
public:
	// It does have a base, but we'll never network anything below here..
	CTraceFilterSimple( const IHandleEntity *passentity, int collisionGroup,
	                    ShouldHitFunc_t pExtraShouldHitCheckFn = NULL );
	virtual bool ShouldHitEntity( IHandleEntity *pHandleEntity, int contentsMask );

	virtual void SetPassEntity( const IHandleEntity *pPassEntity ) {
		m_pPassEnt = pPassEntity;
	}

	virtual void SetCollisionGroup( int iCollisionGroup ) {
		m_collisionGroup = iCollisionGroup;
	}

	const IHandleEntity *GetPassEntity( void ) {
		return m_pPassEnt;
	}

private:
	const IHandleEntity *m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};

class CTraceFilterSkipTwoEntities : public CTraceFilter {
private:
	IClientEntity *m_ent1;
	IClientEntity *m_ent2;

public:
	CTraceFilterSkipTwoEntities( IClientEntity *ent1, IClientEntity *ent2 ) {
		m_ent1 = ent1;
		m_ent2 = ent2;
	}

	bool ShouldHitEntity( IHandleEntity *pEntityHandle, int /*contentsMask*/ ) override {
		return !( pEntityHandle == m_ent1 || pEntityHandle == m_ent2 );
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_EVERYTHING;
	}
};

class CTraceFilterHitAll : public CTraceFilter {
public:
	bool ShouldHitEntity( IHandleEntity * /*pServerEntity*/, int /*contentsMask*/ ) override {
		return true;
	}
};

struct Ray_t {
private:
public:
	vectorAligned m_start;
	vectorAligned m_delta;
	vectorAligned m_start_offset;
	vectorAligned m_extents;
	const matrix3x4_t *m_world_axis_transform;
	bool m_is_ray;
	bool m_is_swept;

	Ray_t() : m_world_axis_transform( nullptr ), m_is_ray{ false }, m_is_swept{ false } { }

	Ray_t( vec3_t const &start, vec3_t const &end ) {
		init( start, end );
	}

	Ray_t( vec3_t const &start, vec3_t const &end, vec3_t const &mins, vec3_t const &maxs ) :
		m_world_axis_transform( nullptr ), m_is_ray{ false }, m_is_swept{ false } {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.Length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.LengthSqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}

	void init( vec3_t const &start, vec3_t const &end ) {
		m_delta = end - start;

		m_is_swept = m_delta.LengthSqr() != 0;

		m_extents.Init( 0.f, 0.f, 0.f );

		m_world_axis_transform = nullptr;
		m_is_ray = true;

		m_start_offset.Init( 0.f, 0.f, 0.f );
		m_start = start;
	}

	void init( vec3_t const &start, vec3_t const &end, vec3_t const &mins, vec3_t const &maxs ) {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.Length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.LengthSqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}
};

class CBaseTrace {
public:
	bool IsDispSurface( void ) {
		return ( ( dispFlags & DISPSURF_FLAG_SURFACE ) != 0 );
	}

	bool IsDispSurfaceWalkable( void ) {
		return ( ( dispFlags & DISPSURF_FLAG_WALKABLE ) != 0 );
	}

	bool IsDispSurfaceBuildable( void ) {
		return ( ( dispFlags & DISPSURF_FLAG_BUILDABLE ) != 0 );
	}

	bool IsDispSurfaceProp1( void ) {
		return ( ( dispFlags & DISPSURF_FLAG_SURFPROP1 ) != 0 );
	}

	bool IsDispSurfaceProp2( void ) {
		return ( ( dispFlags & DISPSURF_FLAG_SURFPROP2 ) != 0 );
	}

public:

	// these members are aligned!!
	vec3_t startpos; // start position
	vec3_t endpos; // final position
	cplane_t plane; // surface normal at impact

	float fraction; // time completed, 1.0 = didn't hit anything

	int contents; // contents on other side of surface hit
	unsigned short dispFlags; // displacement flags for marking surfaces with data

	bool allsolid; // if true, plane is not valid
	bool startsolid; // if true, the initial point was in a solid area

	CBaseTrace() {}

};

class CGameTrace : public CBaseTrace {
public:
	int GetEntityIndex() const { }

	bool DidHit() const {
		return fraction < 1 || allsolid || startsolid;
	}

	bool IsVisible() const {
		return fraction > 0.97f;
	}

public:
	float fractionleftsolid; // time we left a solid, only valid if we started in solid
	csurface_t surface; // surface hit (impact surface)
	int hitgroup; // 0 == generic, non-zero is specific body part
	short physicsbone; // physics bone hit by trace in studio
	unsigned short worldSurfaceIndex; // Index of the msurface2_t, if applicable
	IClientEntity *hit_entity;
	int hitbox; // box hit by trace in studio

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace( const CGameTrace &other ) :
		fractionleftsolid( other.fractionleftsolid ),
		surface( other.surface ),
		hitgroup( other.hitgroup ),
		physicsbone( other.physicsbone ),
		worldSurfaceIndex( other.worldSurfaceIndex ),
		hit_entity( other.hit_entity ),
		hitbox( other.hitbox ) {
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}
};

typedef CGameTrace trace_t;

class IEngineTrace {
public:
	virtual int GetPointContents( const vec3_t &vecAbsPosition, int contentsMask = MASK_ALL,
	                              IHandleEntity **ppEntity = nullptr ) = 0;
	virtual int GetPointContents_WorldOnly( const vec3_t &vecAbsPosition, int contentsMask = MASK_ALL ) = 0;
	virtual int GetPointContents_Collideable( ICollideable *pCollide, const vec3_t &vecAbsPosition ) = 0;
	virtual void ClipRayToEntity( const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, trace_t *pTrace ) = 0;
	virtual void ClipRayToCollideable( const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, trace_t *pTrace ) =
	0;
	virtual void TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace ) = 0;
};
