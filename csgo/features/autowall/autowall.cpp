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
void c_autowall::scale_damage( const int hitgroup, c_csplayer *ent, const float weapon_armor_ratio, float &current_damage ) {
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

bool c_autowall::is_breakable( c_base_entity *entity ) const {
	if( !entity || !entity->get_index( ) )
		return false;

	// backup original take_damage value.
	const int take_damage = entity->get_take_damage( );

	const client_class *client_class = entity->get_client_class( );
	if( !client_class )
		return false;

	// convert to string since std::string::at throws std::out_of_range when pos > length
	std::string network_name = client_class->m_network_name;
	try { 

		if( network_name.at( 1 ) == 'B' && network_name.at( 9 ) == 'e' && network_name.at( 10 ) == 'S' && network_name.at( 16 ) == 'e' )
			entity->get_take_damage( ) = 2;

		else if( network_name.at( 1 ) != 'B' && network_name.at( 5 ) != 'D' )
			entity->get_take_damage( ) = 2;

		else if( network_name.at( 1 ) != 'F' && network_name.at( 4 ) != 'c' && network_name.at( 5 ) != 'B' && network_name.at( 9 ) != 'h' ) // CFuncBrush
			entity->get_take_damage( ) = 2;

	} catch( std::out_of_range &ex ){
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_WARN, "Out of range access on the entity's networked name, ( autowall )", ex.what( ) );
		return nullptr;
	}

	using IsBreakableEntity_t = bool( __thiscall *)( c_base_entity * );
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

		const int point_contents = g_csgo.m_engine_trace->get_point_contents( end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );
		if( point_contents & MASK_SHOT_HULL && !( point_contents & CONTENTS_HITBOX ) )
			continue;

		vec3_t a1 = end - dir * 4.f;
		g_csgo.m_engine_trace->trace_ray( ray_t{ end, a1 }, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, exit_trace );

		if( exit_trace->m_startsolid && exit_trace->m_surface.m_flags & SURF_HITBOX ) {
			c_trace_filter_skip_entity filter( exit_trace->m_hit_entity );
			g_csgo.m_engine_trace->trace_ray( ray_t{ end, start }, MASK_SHOT_HULL, &filter, exit_trace );

			if( ( exit_trace->m_fraction < 1.f || exit_trace->m_allsolid ) && !exit_trace->m_startsolid ) {
				end = exit_trace->m_endpos;
				return true;
			}

			continue;
		}

		if( !exit_trace->did_hit( ) || exit_trace->m_startsolid ) {
			if( enter_trace->m_hit_entity && ( enter_trace->m_hit_entity != nullptr && enter_trace->m_hit_entity != g_csgo.m_entity_list->get_client_entity( 0 ) ) ) {
				*exit_trace = *enter_trace;
				exit_trace->m_endpos = start + dir;
				return true;
			}

			continue;
		}

		if( !exit_trace->did_hit( ) || exit_trace->m_startsolid ) {
			if( enter_trace->m_hit_entity != nullptr && !enter_trace->m_hit_entity->get_index( ) == 0 && is_breakable( static_cast< c_base_entity* >( enter_trace->m_hit_entity ) ) ) {
				*exit_trace = *enter_trace;
				exit_trace->m_endpos = start + dir;
				return true;
			}

			continue;
		}

		if( exit_trace->m_surface.m_flags >> 7 & SURF_LIGHT && !( enter_trace->m_surface.m_flags >> 7 & SURF_LIGHT ) )
			continue;

		if( exit_trace->m_plane.m_normal.dot( dir ) <= 1.f ) {
			end = end - dir * ( exit_trace->m_fraction * 4.f );
			return true;
		}
	}

	return false;
}

bool c_autowall::handle_bullet_pen( surface_data_t *enter_surface, trace_t *enter_trace, const vec3_t &direction, vec3_t *origin, float penetration, int &penetration_count, float &current_damage, float min_dmg ) {
	bool a5 = enter_trace->m_contents >> 3 & CONTENTS_SOLID;
	bool v19 = enter_trace->m_surface.m_flags >> 7 & SURF_LIGHT;

	vec3_t end;
	trace_t exit_trace;

	if( !trace_to_exit( end, enter_trace->m_endpos, direction, enter_trace, &exit_trace ) && !( g_csgo.m_engine_trace->get_point_contents( end, MASK_SHOT_HULL, nullptr ) & MASK_SHOT_HULL ) )
		return false;

	if( enter_trace->m_surface.m_name )
		m_enter_material_name = enter_trace->m_surface.m_name;

	if( exit_trace.m_surface.m_name )
		m_exit_material_name = exit_trace.m_surface.m_name;

	// stops autowall from shooting a material on cache with inconsistent penetration values.
	// DE_CACHE/DE_CACHE_TELA_03
	// CS_ITALY/CR_MISCWOOD2B
	if( m_enter_material_name.find( "TELA" ) != std::string::npos && m_exit_material_name.find( "ITALY" ) != std::string::npos )
		return false;

	surface_data_t *exit_surface_data = g_csgo.m_physics_surface->get_surface_data( exit_trace.m_surface.m_surface_props );

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
	const float pen_len = ( exit_trace.m_endpos - enter_trace->m_endpos ).length( );
	const float lost_dmg = modifier * 3.f * std::fmaxf( 0.f, 3.f / penetration * 1.25f ) + current_damage * damage_modifier + pen_len * pen_len * modifier / 24.f;

	m_pen_len = pen_len;
	m_lost_dmg = lost_dmg;

	current_damage -= std::fmaxf( 0.f, lost_dmg );
	if( current_damage < 1.f || current_damage < min_dmg )
		return false;

	*origin = exit_trace.m_endpos;
	--penetration_count;

	return true;
}

void c_autowall::clip_trace( const vec3_t &src, vec3_t &end, trace_t *tr, c_base_entity *target ) {
	if( !target )
		return;

	const vec3_t mins = target->mins( );
	const vec3_t maxs = target->maxs( );

	vec3_t dir( end - src );
	dir.normalize( );

	const vec3_t center = ( maxs + mins ) / 2;
	const vec3_t pos( center + target->origin( ) );

	vec3_t to = pos - src;
	const float range_along = dir.dot( to );

	float range;
	if( range_along < 0.f ) {
		range = -to.length( );
	}
	else if( range_along > dir.length( ) ) {
		range = -( pos - end ).length( );
	}
	else {
		auto ray( pos - ( dir * range_along + src ) );
		range = ray.length( );
	}

	if( range <= 60.f ) {
		trace_t trace;
		g_csgo.m_engine_trace->clip_ray_to_entity( ray_t{ src, end }, MASK_SHOT_HULL | CONTENTS_HITBOX, target, &trace );
		if( tr->m_fraction > trace.m_fraction )
			*tr = trace;
	}
}

bool c_autowall::think( const vec3_t &position, c_csplayer *entity, const int mindmg, const bool run_bullet_pen ) {
	auto local = c_csplayer::get_local( );
	if( !entity || !local )
		return false;

	if( entity->is_dormant( ) )
		return false;

	c_base_combat_weapon *weapon = local->get_active_weapon( );
	if( !weapon )
		return false;

	const weapon_info_t *weapon_info = weapon->get_weapon_info( );
	if( !weapon_info )
		return false;

	m_autowall_dmg = static_cast< float >( weapon_info->damage );

	vec3_t start = local->eye_pos( );
	vec3_t direction( position - start );
	direction.normalize( );

	int hits_left = 4;
	float trace_length = 0.f;

	trace_t trace;
	c_trace_filter_skip_entity filter( local );

	while( m_autowall_dmg > 0.f ) {
		const float trace_length_remaining = weapon_info->range - trace_length;
		vec3_t end = start + direction * trace_length_remaining;

		g_csgo.m_engine_trace->trace_ray( ray_t{ start, end }, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace );
		clip_trace( end + direction * 40.f, start, &trace, entity );

		if( trace.m_fraction == 1.f )
			break;

		trace_length += trace.m_fraction * trace_length_remaining;
		m_autowall_dmg *= std::pow( weapon_info->range_modifier, trace_length / 500 );

		if( trace.m_hitgroup > HITGROUP_GENERIC && trace.m_hitgroup <= HITGROUP_RIGHTLEG && trace.m_hit_entity != nullptr && entity == trace.m_hit_entity ) {
			scale_damage( trace.m_hitgroup, entity, weapon_info->armor_ratio, m_autowall_dmg );
			return m_autowall_dmg >= mindmg;
		}

		if( run_bullet_pen ) {
			const auto enter_surface_data = g_csgo.m_physics_surface->get_surface_data( trace.m_surface.m_surface_props );

			if( trace_length > 3000.f || enter_surface_data->game.penetrationmodifier < 0.1f )
				hits_left = 0;

			if( !handle_bullet_pen( enter_surface_data, &trace, direction, &start, weapon_info->penetration, hits_left, m_autowall_dmg, mindmg ) )
				break;
		}
	}

	return false;
}