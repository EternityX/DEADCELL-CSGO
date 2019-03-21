#include "autowall.h"

c_autowall g_autowall;

float c_autowall::hitgroup_dmg( int hitgroup ) {
	switch( hitgroup ) {
		case HITGROUP_HEAD:
			return 4.f;
		case HITGROUP_STOMACH:
			return 1.25f;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return 0.75f;
		default:
			return 1.f;
	}
}

// note: not 100%.
void c_autowall::scale_damage( const int hitgroup, C_CSPlayer *ent, const float weapon_armor_ratio, float &current_damage ) {
	const float armor = static_cast< float >( ent->armor( ) );
	current_damage *= hitgroup_dmg( hitgroup );

	if( armor > 0.f && hitgroup < HITGROUP_LEFTLEG ) {
		if( hitgroup == HITGROUP_HEAD && !ent->helmet( ) )
			return;

		auto scaled = weapon_armor_ratio * .5f * current_damage;

		if( ( current_damage - scaled ) * .5f > armor )
			scaled = current_damage - armor * 2.f;

		current_damage = scaled;
	}
}

bool c_autowall::is_breakable( C_BaseEntity *entity ) const {
	if( !entity || !entity->GetIndex( ) )
		return false;

	// backup original take_damage value.
	const int take_damage = entity->get_take_damage( );

	const ClientClass *client_class = entity->GetClientClass( );

	if( client_class->m_pNetworkName[ 1 ] == 'B' && client_class->m_pNetworkName[ 9 ] == 'e' && client_class->m_pNetworkName[ 10 ] == 'S' && client_class->m_pNetworkName[ 16 ] == 'e' )
		entity->get_take_damage( ) = 2;

	else if( client_class->m_pNetworkName[ 1 ] != 'B' && client_class->m_pNetworkName[ 5 ] != 'D' )
		entity->get_take_damage( ) = 2;

	else if( client_class->m_pNetworkName[ 1 ] != 'F' && client_class->m_pNetworkName[ 4 ] != 'c' && client_class->m_pNetworkName[ 5 ] != 'B' && client_class->m_pNetworkName[ 9 ] != 'h' ) // CFuncBrush
		entity->get_take_damage( ) = 2;

	using IsBreakableEntity_t = bool( __thiscall *)( C_BaseEntity * );
	static IsBreakableEntity_t IsBreakableEntityEx = nullptr;

	if( !IsBreakableEntityEx )
		IsBreakableEntityEx = pattern::find< IsBreakableEntity_t >( g_csgo.m_client_dll, "55 8B EC 51 56 8B F1 85 F6 74 68" );

	entity->get_take_damage( ) = take_damage;

	return IsBreakableEntityEx( entity );
}

bool c_autowall::trace_to_exit( vec3_t &end, const vec3_t &start, const vec3_t &dir, trace_t *enter_trace, trace_t *exit_trace ) const {
	float dist = 0.f;

	while( dist <= 90.f ) {
		dist += 4.f;
		end = start + dir * dist;

		const int point_contents = g_csgo.m_engine_trace->GetPointContents( end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );
		if( point_contents & MASK_SHOT_HULL && !( point_contents & CONTENTS_HITBOX ) )
			continue;

		vec3_t a1 = end - dir * 4.f;
		g_csgo.m_engine_trace->TraceRay( Ray_t{ end, a1 }, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, exit_trace );

		if( exit_trace->startsolid && exit_trace->surface.m_flags & SURF_HITBOX ) {
			CTraceFilterSkipEntity filter( exit_trace->hit_entity );
			g_csgo.m_engine_trace->TraceRay( Ray_t{ end, start }, MASK_SHOT_HULL, &filter, exit_trace );

			if( ( exit_trace->fraction < 1.f || exit_trace->allsolid ) && !exit_trace->startsolid ) {
				end = exit_trace->endpos;
				return true;
			}

			continue;
		}

		if( !exit_trace->DidHit( ) || exit_trace->startsolid ) {
			if( enter_trace->hit_entity && ( enter_trace->hit_entity != nullptr && enter_trace->hit_entity != g_csgo.m_entity_list->GetClientEntity( 0 ) ) ) {
				*exit_trace = *enter_trace;
				exit_trace->endpos = start + dir;
				return true;
			}

			continue;
		}

		if( !exit_trace->DidHit( ) || exit_trace->startsolid ) {
			if( enter_trace->hit_entity != nullptr && !enter_trace->hit_entity->GetIndex( ) == 0 && is_breakable( static_cast< C_BaseEntity* >( enter_trace->hit_entity ) ) ) {
				*exit_trace = *enter_trace;
				exit_trace->endpos = start + dir;
				return true;
			}

			continue;
		}

		if( exit_trace->surface.m_flags >> 7 & SURF_LIGHT && !( enter_trace->surface.m_flags >> 7 & SURF_LIGHT ) )
			continue;

		if( exit_trace->plane.m_normal.Dot( dir ) <= 1.f ) {
			end = end - dir * ( exit_trace->fraction * 4.f );
			return true;
		}
	}

	return false;
}

bool c_autowall::handle_bullet_pen( SurfaceData_t *enter_surface, trace_t *enter_trace, const vec3_t &direction, vec3_t *origin, float penetration, int &penetration_count, float &current_damage ) {
	bool a5 = enter_trace->contents >> 3 & CONTENTS_SOLID;
	bool v19 = enter_trace->surface.m_flags >> 7 & SURF_LIGHT;

	vec3_t end;
	trace_t exit_trace;

	if( !trace_to_exit( end, enter_trace->endpos, direction, enter_trace, &exit_trace ) && !( g_csgo.m_engine_trace->GetPointContents( end, MASK_SHOT_HULL, nullptr ) & MASK_SHOT_HULL ) )
		return false;

	if( enter_trace->surface.m_name )
		m_enter_material_name = enter_trace->surface.m_name;

	if( exit_trace.surface.m_name )
		m_exit_material_name = exit_trace.surface.m_name;

	// stops autowall from shooting a material on cache with inconsistent penetration values.
	// DE_CACHE/DE_CACHE_TELA_03
	// CS_ITALY/CR_MISCWOOD2B
	if( m_enter_material_name.find( "TELA" ) != std::string::npos && m_exit_material_name.find( "ITALY" ) != std::string::npos )
		return false;

	SurfaceData_t *exit_surface_data = g_csgo.m_physics_surface->GetSurfaceData( exit_trace.surface.m_surface_props );

	float damage_modifier = 0.16f;
	float average_penetration_modifier = ( enter_surface->game.penetrationmodifier + exit_surface_data->game.penetrationmodifier ) / 2;

	const unsigned short enter_material = enter_surface->game.material;
	if( enter_material == CHAR_TEX_GRATE || enter_material == CHAR_TEX_GLASS ) {
		damage_modifier = 0.05f;
		average_penetration_modifier = 3.f;
	}
	else if( a5 || v19 ) {
		damage_modifier = 0.16f;
		average_penetration_modifier = 1.f;
	}

	if( enter_material == exit_surface_data->game.material ) {
		if( exit_surface_data->game.material == CHAR_TEX_WOOD || exit_surface_data->game.material == CHAR_TEX_CARDBOARD )
			average_penetration_modifier = 3.f;
		if( exit_surface_data->game.material == CHAR_TEX_PLASTIC )
			average_penetration_modifier = 2.f;
	}

	m_dmg_mod = damage_modifier;
	m_avg_pen = average_penetration_modifier;

	const float modifier = std::fmaxf( 0.f, 1.f / average_penetration_modifier );
	const float pen_len = ( exit_trace.endpos - enter_trace->endpos ).Length( );
	const float lost_dmg = modifier * 3.f * std::fmaxf( 0.f, 3.f / penetration * 1.25f ) + current_damage * damage_modifier + pen_len * pen_len * modifier / 24.f;

	m_pen_len = pen_len;
	m_lost_dmg = lost_dmg;

	current_damage -= std::fmaxf( 0.f, lost_dmg );
	if( current_damage < 1.f )
		return false;

	*origin = exit_trace.endpos;
	--penetration_count;

	return true;
}

void c_autowall::clip_trace( const vec3_t &src, vec3_t &end, trace_t *tr, C_BaseEntity *target ) {
	if( !target )
		return;

	const vec3_t mins = target->mins( );
	const vec3_t maxs = target->maxs( );

	vec3_t dir( end - src );
	dir.Normalize( );

	const vec3_t center = ( maxs + mins ) / 2;
	const vec3_t pos( center + target->origin( ) );

	vec3_t to = pos - src;
	const float range_along = dir.Dot( to );

	float range;
	if( range_along < 0.f ) {
		range = -to.Length( );
	}
	else if( range_along > dir.Length( ) ) {
		range = -( pos - end ).Length( );
	}
	else {
		auto ray( pos - ( dir * range_along + src ) );
		range = ray.Length( );
	}

	if( range <= 60.f ) {
		trace_t trace;
		g_csgo.m_engine_trace->ClipRayToEntity( Ray_t{ src, end }, MASK_SHOT_HULL | CONTENTS_HITBOX, target, &trace );
		if( tr->fraction > trace.fraction )
			*tr = trace;
	}
}

bool c_autowall::think( C_CSPlayer *from_ent, C_CSPlayer *to_ent, const vec3_t &position, int mindmg, bool run_bullet_pen ) {
	if( !from_ent )
		from_ent = C_CSPlayer::get_local( );
	if ( !from_ent )
		return false;

	if( to_ent->IsDormant( ) )
		return false;

	C_BaseCombatWeapon *weapon = from_ent->get_active_weapon( );
	if( !weapon )
		return false;

	const WeaponInfo_t *weapon_info = weapon->get_weapon_info( );
	if( !weapon_info )
		return false;

	m_autowall_dmg = static_cast< float >( weapon_info->damage );

	vec3_t start = from_ent->eye_pos( );
	vec3_t direction( position - start );
	direction.Normalize( );

	int hits_left = 4;
	float trace_length = 0.f;

	trace_t trace;
	CTraceFilterSkipEntity filter( from_ent );

	while( m_autowall_dmg > 0.f ) {
		const float trace_length_remaining = weapon_info->range - trace_length;
		vec3_t end = start + direction * trace_length_remaining;

		g_csgo.m_engine_trace->TraceRay( Ray_t{ start, end }, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace );
		clip_trace( end + direction * 40.f, start, &trace, to_ent );

		if( trace.fraction == 1.f )
			break;

		trace_length += trace.fraction * trace_length_remaining;
		m_autowall_dmg *= std::pow( weapon_info->range_modifier, trace_length / 500 );

		if( trace.hitgroup > HITGROUP_GENERIC && trace.hitgroup <= HITGROUP_RIGHTLEG && trace.hit_entity != nullptr && to_ent == trace.hit_entity ) {
			scale_damage( trace.hitgroup, to_ent, weapon_info->armor_ratio, m_autowall_dmg );
			return m_autowall_dmg >= mindmg;
		}

		if( run_bullet_pen ) {
			const auto enter_surface_data = g_csgo.m_physics_surface->GetSurfaceData( trace.surface.m_surface_props );

			if( trace_length > 3000.f || enter_surface_data->game.penetrationmodifier < 0.1f )
				hits_left = 0;

			if( !handle_bullet_pen( enter_surface_data, &trace, direction, &start, weapon_info->penetration, hits_left, m_autowall_dmg ) )
				break;
		}
	}

	return false;
}
