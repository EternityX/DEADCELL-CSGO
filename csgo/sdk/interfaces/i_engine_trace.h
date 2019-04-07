#pragma once

class i_trace_filter {
public:
	virtual bool should_hit_entity( i_handle_entity *pEntity, int contentsMask ) = 0;
	virtual trace_type_t get_trace_type() const = 0;
};

class c_trace_filter : public i_trace_filter {
public:
	void *m_skip;
	char *m_ignore = "";

	bool should_hit_entity( i_handle_entity *pEntityHandle, int /*contentsMask*/ ) override {
		client_class *ent_cc = ( (i_client_entity *)pEntityHandle )->get_client_class();
		if( ent_cc && strcmp( m_ignore, "" ) ) {
			if( ent_cc->m_network_name == m_ignore )
				return false;
		}

		return !( pEntityHandle == m_skip );
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_EVERYTHING;
	}

	void set_ignore_class( char *cc ) {
		m_ignore = cc;
	}
};

class c_trace_filter_skip_entity : public c_trace_filter {
public:
	void *m_skip;

	c_trace_filter_skip_entity( i_handle_entity *pEntityHandle ) {
		m_skip = pEntityHandle;
	}

	bool should_hit_entity( i_handle_entity *pEntityHandle, int /*contentsMask*/ ) override {
		return !( pEntityHandle == m_skip );
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_EVERYTHING;
	}
};

class c_trace_filter_entities_only : public c_trace_filter {
public:
	bool should_hit_entity( i_handle_entity *pEntityHandle, int /*contentsMask*/ ) override {
		return true;
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_ENTITIES_ONLY;
	}
};

class c_trace_filter_world_only : public c_trace_filter {
public:
	bool should_hit_entity( i_handle_entity * /*pServerEntity*/, int /*contentsMask*/ ) override {
		return false;
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_WORLD_ONLY;
	}
};

class c_trace_filter_world_and_props_only : public c_trace_filter {
public:
	bool should_hit_entity( i_handle_entity * /*pServerEntity*/, int /*contentsMask*/ ) override {
		return false;
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_EVERYTHING;
	}
};

class c_trace_filter_players_only_skip_one : public c_trace_filter {
public:
	c_trace_filter_players_only_skip_one( i_client_entity *ent ) {
		pEnt = ent;
	}

	bool should_hit_entity( i_handle_entity *pEntityHandle, int /*contentsMask*/ ) override {
		return pEntityHandle != pEnt && ( (i_client_entity*)pEntityHandle )->get_client_class()->m_class_id == CCSPlayer;
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_ENTITIES_ONLY;
	}

private:
	i_client_entity *pEnt;
};

typedef bool (*should_hit_func_t)( i_handle_entity *pHandleEntity, int contentsMask );

class c_trace_filter_simple : public c_trace_filter {
public:
	// It does have a base, but we'll never network anything below here..
	c_trace_filter_simple( const i_handle_entity *passentity, int collisionGroup,
	                    should_hit_func_t pExtraShouldHitCheckFn = NULL );
	virtual bool should_hit_entity( i_handle_entity *pHandleEntity, int contentsMask );

	virtual void SetPassEntity( const i_handle_entity *pPassEntity ) {
		m_pPassEnt = pPassEntity;
	}

	virtual void SetCollisionGroup( int iCollisionGroup ) {
		m_collisionGroup = iCollisionGroup;
	}

	const i_handle_entity *GetPassEntity( void ) {
		return m_pPassEnt;
	}

private:
	const i_handle_entity *m_pPassEnt;
	int m_collisionGroup;
	should_hit_func_t m_pExtraShouldHitCheckFunction;
};

class c_trace_filter_skip_two_entities : public c_trace_filter {
private:
	i_client_entity *m_ent1;
	i_client_entity *m_ent2;

public:
	c_trace_filter_skip_two_entities( i_client_entity *ent1, i_client_entity *ent2 ) {
		m_ent1 = ent1;
		m_ent2 = ent2;
	}

	bool should_hit_entity( i_handle_entity *pEntityHandle, int /*contentsMask*/ ) override {
		return !( pEntityHandle == m_ent1 || pEntityHandle == m_ent2 );
	}

	trace_type_t get_trace_type() const override {
		return trace_type_t::TRACE_EVERYTHING;
	}
};

class c_trace_filter_hit_all : public c_trace_filter {
public:
	bool should_hit_entity( i_handle_entity * /*pServerEntity*/, int /*contentsMask*/ ) override {
		return true;
	}
};

struct ray_t {
private:
public:
	vector_aligned m_start;
	vector_aligned m_delta;
	vector_aligned m_start_offset;
	vector_aligned m_extents;
	const matrix3x4_t *m_world_axis_transform;
	bool m_is_ray;
	bool m_is_swept;

	ray_t() : m_world_axis_transform( nullptr ), m_is_ray{ false }, m_is_swept{ false } { }

	ray_t( vec3_t const &start, vec3_t const &end ) {
		init( start, end );
	}

	ray_t( vec3_t const &start, vec3_t const &end, vec3_t const &mins, vec3_t const &maxs ) :
		m_world_axis_transform( nullptr ), m_is_ray{ false }, m_is_swept{ false } {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.length_sqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}

	void init( vec3_t const &start, vec3_t const &end ) {
		m_delta = end - start;

		m_is_swept = m_delta.length_sqr() != 0;

		m_extents.init( 0.f, 0.f, 0.f );

		m_world_axis_transform = nullptr;
		m_is_ray = true;

		m_start_offset.init( 0.f, 0.f, 0.f );
		m_start = start;
	}

	void init( vec3_t const &start, vec3_t const &end, vec3_t const &mins, vec3_t const &maxs ) {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.length_sqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}
};

class c_base_trace {
public:
	bool is_disp_surface( void ) {
		return ( ( m_disp_flags & DISPSURF_FLAG_SURFACE ) != 0 );
	}

	bool is_disp_surface_walkable( void ) {
		return ( ( m_disp_flags & DISPSURF_FLAG_WALKABLE ) != 0 );
	}

	bool is_disp_surface_buildable( void ) {
		return ( ( m_disp_flags & DISPSURF_FLAG_BUILDABLE ) != 0 );
	}

	bool is_disp_surface_prop1( void ) {
		return ( ( m_disp_flags & DISPSURF_FLAG_SURFPROP1 ) != 0 );
	}

	bool is_disp_surface_prop2( void ) {
		return ( ( m_disp_flags & DISPSURF_FLAG_SURFPROP2 ) != 0 );
	}

public:

	// these members are aligned!!
	vec3_t m_startpos; // start position
	vec3_t m_endpos; // final position
	cplane_t m_plane; // surface normal at impact

	float m_fraction; // time completed, 1.0 = didn't hit anything

	int m_contents; // contents on other side of surface hit
	unsigned short m_disp_flags; // displacement flags for marking surfaces with data

	bool m_allsolid; // if true, plane is not valid
	bool m_startsolid; // if true, the initial point was in a solid area

	c_base_trace() {}

};

class c_game_trace : public c_base_trace {
public:
	int get_entity_index() const { }

	bool did_hit() const {
		return m_fraction < 1 || m_allsolid || m_startsolid;
	}

	bool is_visible() const {
		return m_fraction > 0.97f;
	}

public:
	float m_fractionleftsolid; // time we left a solid, only valid if we started in solid
	csurface_t m_surface; // surface hit (impact surface)
	int m_hitgroup; // 0 == generic, non-zero is specific body part
	short m_physicsbone; // physics bone hit by trace in studio
	unsigned short m_world_surface_index; // Index of the msurface2_t, if applicable
	i_client_entity *m_hit_entity;
	int m_hitbox; // box hit by trace in studio

	c_game_trace() {}

private:
	// No copy constructors allowed
	c_game_trace( const c_game_trace &other ) :
		m_fractionleftsolid( other.m_fractionleftsolid ),
		m_surface( other.m_surface ),
		m_hitgroup( other.m_hitgroup ),
		m_physicsbone( other.m_physicsbone ),
		m_world_surface_index( other.m_world_surface_index ),
		m_hit_entity( other.m_hit_entity ),
		m_hitbox( other.m_hitbox ) {
		m_startpos = other.m_startpos;
		m_endpos = other.m_endpos;
		m_plane = other.m_plane;
		m_fraction = other.m_fraction;
		m_contents = other.m_contents;
		m_disp_flags = other.m_disp_flags;
		m_allsolid = other.m_allsolid;
		m_startsolid = other.m_startsolid;
	}
};

typedef c_game_trace trace_t;

class i_engine_trace {
public:
	virtual int get_point_contents( const vec3_t &vecAbsPosition, int contentsMask = MASK_ALL,
	                              i_handle_entity **ppEntity = nullptr ) = 0;
	virtual int get_point_contents_world_only( const vec3_t &vecAbsPosition, int contentsMask = MASK_ALL ) = 0;
	virtual int get_point_contents_collideable( i_collideable *pCollide, const vec3_t &vecAbsPosition ) = 0;
	virtual void clip_ray_to_entity( const ray_t &ray, unsigned int fMask, i_handle_entity *pEnt, trace_t *pTrace ) = 0;
	virtual void clip_ray_to_collideable( const ray_t &ray, unsigned int fMask, i_collideable *pCollide, trace_t *pTrace ) =
	0;
	virtual void trace_ray( const ray_t &ray, unsigned int fMask, i_trace_filter *pTraceFilter, trace_t *pTrace ) = 0;
};
