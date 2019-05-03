#include "ragebot.h"
#include "../animations/anim.h"
#include "../geometry/geometry.h"

c_ragebot g_ragebot;

void c_ragebot::work( c_user_cmd *cmd ) {
	if( !g_vars.rage.enabled )
		return;

	if( g_vars.rage.key && !g_input.key_pressed( g_vars.rage.key ) )
		return;

	m_cmd = cmd;

	select_target( );
	choose_angles( );
	restore_players( );

	if ( !m_players.empty( ) )
		m_players.clear( );
}

void c_ragebot::select_target( ) {
	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon || weapon->clip( ) <= 0 )
		return;

	auto bounding_check = [ this, local ]( c_csplayer *e, const lag_record_t &record, float min_dmg ) -> bool {
		auto *studio_hdr = e->model_ptr( )->m_studio_hdr;
		if( !studio_hdr )
			return false;

		auto head_hitbox = studio_hdr->hitbox( head, 0 );
		if( !head_hitbox )
			return false;

		const auto bbmin = record.m_mins + record.m_origin;
		const auto bbmax = record.m_maxs + record.m_origin;

		vec3_t points[ 7 ];
		points[ 0 ] = bbmax;
		points[ 1 ] = ( bbmin + bbmax ) * 0.5f;
		points[ 2 ] = ( head_hitbox->bb_min + head_hitbox->bb_max ) * 0.5f;
		points[ 3 ] = vec3_t( ( bbmax.x + bbmin.x ) * 0.5f, ( bbmax.y + bbmin.y ) * 0.5f, bbmin.z );
		points[ 4 ] = vec3_t( bbmax.x, bbmin.y, bbmax.z );
		points[ 5 ] = vec3_t( bbmin.x, bbmin.y, bbmax.z );
		points[ 6 ] = vec3_t( bbmin.x, bbmax.y, bbmax.z );

		for( const auto &p : points ) {
			if( g_autowall.think( p, e, 1, true ) )
				return true;
		}

		return false;
	};

	try {
		std::vector< int > hitboxes = {};
		hitboxes.push_back( g_vars.rage.primary_hitbox );

		if( g_vars.rage.dynamic_hitbox ) {
			hitboxes.push_back( head );
			hitboxes.push_back( neck );
			hitboxes.push_back( l_chest );
			hitboxes.push_back( u_chest );
			hitboxes.push_back( thorax );
			hitboxes.push_back( pelvis );
			hitboxes.push_back( l_upperarm );
			hitboxes.push_back( r_upperarm );
			hitboxes.push_back( l_forearm );
			hitboxes.push_back( r_forearm );
			hitboxes.push_back( l_hand );
			hitboxes.push_back( r_hand );
			hitboxes.push_back( l_thigh );
			hitboxes.push_back( r_thigh );
			hitboxes.push_back( l_calf );
			hitboxes.push_back( r_calf );
			hitboxes.push_back( l_foot );
			hitboxes.push_back( r_foot );
		}
		else {
			if( g_vars.rage.hitbox_head ) {
				hitboxes.push_back( head );
				hitboxes.push_back( neck );
			}
			if( g_vars.rage.hitbox_pelvis ) {
				hitboxes.push_back( l_chest );
				hitboxes.push_back( u_chest );
				hitboxes.push_back( thorax );
				hitboxes.push_back( pelvis );
			}
			if( g_vars.rage.hitbox_arms ) {
				hitboxes.push_back( l_upperarm );
				hitboxes.push_back( r_upperarm );
				hitboxes.push_back( l_forearm );
				hitboxes.push_back( r_forearm );
				hitboxes.push_back( l_hand );
				hitboxes.push_back( r_hand );
			}
			if( g_vars.rage.hitbox_legs ) {
				hitboxes.push_back( l_thigh );
				hitboxes.push_back( r_thigh );
				hitboxes.push_back( l_calf );
				hitboxes.push_back( r_calf );
				hitboxes.push_back( l_foot );
				hitboxes.push_back( r_foot );
			}
		}

		for( auto &entry : g_listener.m_players ) {
			int idx = entry.m_idx;
			c_csplayer *e = entry.m_player;
			if( !is_valid( e ) )
				continue;

			lag_record_t player_record;
			float player_best_damage = 0.f;
			vec3_t player_best_point = vec3_t( 0.f, 0.f, 0.f );

			const int best_min_dmg = local->get_active_weapon( )->clip( ) <= 3 ? e->health( ) : g_vars.rage.min_dmg; // ensure we get the kill
			std::deque< lag_record_t > best_records;
			if( !get_best_records( e, best_records ) )
				continue;

			for( size_t i = 0; i < best_records.size(); ++i ) {
				auto record = best_records.at( i );

				if( !record.is_valid( ) )
					continue;

				if( !g_backtrack.restore( e, record ) )
					continue;

				if( g_vars.rage.save_fps && !bounding_check( e, record, best_min_dmg ) )
					continue;

				std::vector< vec3_t > points;
				if( !get_points_from_hitbox( e, hitboxes, record.m_matrix, points, best_records, i, g_vars.rage.pointscale / 100.f ) )
					continue;

				if( points.empty( ) )
					continue;

				for( auto &p : points ) {
					if( g_vars.visuals.extra.points )
						g_csgo.m_debug_overlay->add_box_overlay( p, vec3_t( -0.7f, -0.7f, -0.7f ), vec3_t( 0.7f, 0.7f, 0.7f ), vec3_t( 0.f, 0.f, 0.f ), 0, 255, 0, 100, g_csgo.m_global_vars->m_interval_per_tick * 2 );

					if( g_autowall.think( p, e, best_min_dmg, true ) ) {
						if( g_autowall.m_autowall_dmg > player_best_damage ) {
							player_best_damage = g_autowall.m_autowall_dmg;
							player_best_point = p;
							player_record = record;
						}
					}
				}
			}

			m_players.emplace_back( e, player_record, idx, static_cast< int >( player_best_damage ), player_best_point, e->abs_origin( ).distance( local->abs_origin( ) ) );
		}

		std::sort( m_players.begin( ), m_players.end( ), [ & ]( rage_t &a, rage_t &b ) {
			switch( g_vars.rage.target_selection ) {
				case 0:
					return a.m_damage > b.m_damage;
				case 1:
					return a.distance < b.distance;
				case 2:
					return a.index < b.index;
				default:
					return false;
			}
		} );
	}
	catch( const std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT0( _CRT_ERROR, ex.what( ) );
	}
}

void c_ragebot::restore_players( ) {
	for( auto entry : g_listener.m_players ) {
		int idx = entry.m_idx;
		c_csplayer *e = entry.m_player;
		if( !is_valid( e ) )
			continue;

		auto &records = g_backtrack.get( idx )->m_records;
		if( !records.empty( ) )
			g_backtrack.restore( e, records.front( ) );
	}
}

bool c_ragebot::hitchance( vec3_t &angle, c_csplayer *ent ) {
	int traces_hit = 0;
	auto local = c_csplayer::get_local( );
	if( !local )
		return false;

	vec3_t forward, right, up;
	const vec3_t eye_position = local->eye_pos( );
	math::angle_to_vectors( angle + local->punch_angle( ) * 2.f, &forward, &right, &up ); // maybe add an option to not account for punch.

	c_base_combat_weapon *weapon = local->get_active_weapon( );
	if( !weapon )
		return false;

	weapon->update_accuracy( );
	float weapon_spread = weapon->spread( );
	float weapon_cone = weapon->inaccuracy( );

	const auto get_bullet_location = [ & ]( int seed ) {
		static auto random_seed = pe::get_export< void( *)( int ) >( pe::get_module( "vstdlib.dll" ), "RandomSeed" );
		random_seed( seed );

		float a = math::random_float( 0.f, 1.f );
		float b = math::random_float( 0.f, 2.f * math::pi );
		float c = math::random_float( 0.f, 1.f );
		float d = math::random_float( 0.f, 2.f * math::pi );

		const float generated_spread = a * weapon_spread;
		const float generated_cone = c * weapon_cone;

		const vec3_t spread = vec3_t(
			std::cos( b ) * generated_spread + std::cos( d ) * generated_cone,
			std::sin( b ) * generated_spread + std::sin( d ) * generated_cone,
			0.f
		);

		return vec3_t( forward + right * -spread.x + up * -spread.y ).normalized( );
	};

	for( int i = 1; i <= 256; i++ ) {
		vec3_t spread_angle;
		vec3_t bullet_end;

		math::vector_angle( get_bullet_location( i ), spread_angle );
		math::angle_to_vector( angle - ( spread_angle - angle ), bullet_end );

		trace_t trace;
		ray_t ray;
		ray.init( eye_position, eye_position + bullet_end * weapon->get_weapon_info( )->range );

		g_csgo.m_engine_trace->clip_ray_to_entity( ray, MASK_SHOT, ent, &trace );

		if( trace.m_hit_entity == ent )
			++traces_hit;

		if( traces_hit >= static_cast< int >( g_vars.rage.hitchance * 2.56f ) )
			return true;
	}

	return false;
}

void c_ragebot::choose_angles( ) {
	c_csplayer *selected_target = nullptr;
	lag_record_t best_record{};
	vec3_t best_hitboxpos;
	float best_damage = 0.f;

	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;

	for( auto &data : m_players ) {
		auto target = data.m_player;

		auto calc_damage = [ & ]( ) {
			lag_record_t player_record = data.m_record;
			float player_best_damage = data.m_damage;
			vec3_t player_best_point = data.m_bestpoint;

			if( player_best_damage > best_damage ) {
				selected_target = target;
				best_record = player_record;
				best_hitboxpos = player_best_point;
				best_damage = player_best_damage;
			}
			if( player_best_damage >= target->health( ) ) {
				selected_target = target;
				best_record = player_record;
				best_hitboxpos = player_best_point;
				best_damage = player_best_damage;
				return true;
			}
			return false;
		};

		if( calc_damage( ) )
			break;
	}
	if( !selected_target )
		return;

	m_last_target = selected_target;

	if( g_vars.rage.autoscope == 1 && ( !local->is_scoped( ) && weapon->has_scope( ) ) && selected_target )
		m_cmd->m_buttons |= IN_ATTACK2;

	vec3_t aim_angle = math::calc_angle( local->eye_pos( ), best_hitboxpos );
	aim_angle.clamp( );

	if( !hitchance( aim_angle, selected_target ) ) {
		if( g_vars.rage.autoscope == 2 && ( !local->is_scoped( ) && weapon->has_scope( ) ) )
			m_cmd->m_buttons |= IN_ATTACK2;

		if( g_vars.rage.autostop )
			quickstop( weapon );

		if( g_vars.rage.auto_fire )
			return;
	}


	if( !local->can_shoot( weapon ) )
		return;

	if( g_vars.rage.auto_fire && !( m_cmd->m_buttons & IN_ATTACK ) ) {
		m_cmd->m_buttons |= IN_ATTACK;
		shots_fired++;
	}

	if( m_cmd->m_buttons & IN_ATTACK ) {
		m_cmd->m_viewangles = aim_angle;

		weapon->update_accuracy( );
		m_cmd->m_viewangles -= local->punch_angle( ) * 2.f;

		if( !g_vars.rage.silent )
			g_csgo.m_engine->set_viewangles( m_cmd->m_viewangles );

		if( g_vars.misc.client_hitboxes )
			g_misc.capsule_overlay( selected_target, g_vars.misc.client_hitboxes_duration, best_record.m_matrix );

		g_backtrack.process_cmd( m_cmd, selected_target, best_record );
	}
}

bool c_ragebot::get_best_records( c_csplayer *e, std::deque< lag_record_t > &out ) {
	if( !e || !g_cl.m_local )
		return false;

	out = g_backtrack.get( e->get_index( ) )->m_records;

	out.erase( std::find_if( out.begin( ), out.end( ), [ ]( lag_record_t &record ) {
		return !record.is_valid( );
	} ), out.end( ) );

	out.erase( std::unique( out.begin( ), out.end( ), [ ]( lag_record_t &a, lag_record_t &b ) {
		return ( a.m_angles == b.m_angles && a.m_origin == b.m_origin ) || a.m_origin.distance( b.m_origin ) < 15.f;
	} ), out.end( ) );

	return true;
}

bool c_ragebot::get_points_from_hitbox( c_csplayer *e, std::vector< int > hitboxes, matrix3x4_t *matrix, std::vector< vec3_t > &points, const std::deque< lag_record_t > &sorted_records, size_t record_idx, float scale ) {
	if( !e )
		return false;

	auto *studiohdr = e->model_ptr( )->m_studio_hdr;
	if( !studiohdr )
		return false;

	auto local = c_csplayer::get_local( );
	if( !local )
		return false;

	try {
		for( size_t idx = 0; idx < hitboxes.size( ); ++idx ) {
			int h = hitboxes.at( idx );
			auto *bbox = studiohdr->hitbox( h, 0 );

			// philip015's old hack
			float mod = bbox->m_flRadius != -1.f ? bbox->m_flRadius : 0.f;

			vec3_t max, min;
			math::vector_transform( bbox->bb_min - mod, matrix[ bbox->bone_index ], min );
			math::vector_transform( bbox->bb_max + mod, matrix[ bbox->bone_index ], max );
			auto center = ( min + max ) / 2.f;

			points.push_back( center );

			auto angle = math::calc_angle( center, local->eye_pos( ) );

			vec3_t forward;
			math::angle_to_vector( angle, forward );

			vec3_t right = forward.cross( vec3_t( 0, 0, 1 ) );
			vec3_t left = vec3_t( -right.x, -right.y, right.z );

			vec3_t top = vec3_t( 0, 0, 1 );
			vec3_t bot = vec3_t( 0, 0, -1 );

			// don't bother multipointing if body/head is visible.
			if( g_vars.rage.dynamic_hitbox ) {
				if( g_cl.m_under_tickrate && m_last_target ) {
					if( e != m_last_target ) { // check if we're aiming at the same target and only multipoint this target.
						continue;
					}
				}

				if( hitboxes.empty( ) || points.empty( ) )
					return false;

				if( points.size( ) >= hitboxes.size( ) ) {
					switch( h ) {
						case head:
						case neck:
						case l_chest:
						case u_chest:
						case thorax:
						case pelvis:
							break;
						default:
							continue;
					}

					if( e->is_visible( e, local->eye_pos( ), points.at( head ), MASK_SHOT, local )
						|| e->is_visible( e, local->eye_pos( ), points.at( pelvis ), MASK_SHOT, local ) )
						return true;
				}
			}

			if( g_vars.rage.bodyaim_lethal && e->health( ) < local->get_active_weapon( )->get_weapon_info( )->damage ) {
				hitboxes.at( 0 ) = pelvis;
			}

			if( g_vars.rage.prefer_bodyaim ) {
				if( !( e->flags( ) & FL_ONGROUND ) || e->health( ) < 50 ) {
					hitboxes.at( 0 ) = pelvis;
				}
			}

			if( scale == 0.f )
				continue;

			// optimize points on certain records.
			bool should_multipoint = true;
			if( g_vars.rage.selectively_multipoint_records ) {
				// only use multipoint settings on the first/last 2 records.
				const auto records_size = sorted_records.size( );
				if( records_size > 5 ) {		
					// first two
					if( record_idx == 0 || record_idx == 1 )
						should_multipoint = true;
					// last two
					else if( record_idx == records_size - 1 || record_idx == records_size - 2 )
						should_multipoint = true;
					else
						should_multipoint = false;
				}				
			}

			switch( h ) {
				case head:
					if( !g_vars.rage.head || !should_multipoint )
						continue;
					break;
				case l_chest:
				case u_chest:
				case pelvis:
					if( !g_vars.rage.pelvis || !should_multipoint ) // should always multipoint the torso regardless of the selective multipoint option.
						continue;
					break;
				case l_upperarm:
				case r_upperarm:
					if( !g_vars.rage.arms || !should_multipoint )
						continue;
					break;
				case l_thigh:
				case r_thigh:
					if( !g_vars.rage.legs || !should_multipoint )
						continue;
					break;
				default:
					continue;
			}

			float rs = bbox->m_flRadius * scale;

			if( bbox->m_flRadius == -1.f ) {
	
			}
			else if( bbox->m_flRadius > 0.f ) {
				//// pill.
				if( h == head )
					points.push_back( center + top * rs );

				points.push_back( center + right * rs );
				points.push_back( center + left * rs );
				
			}
		}
	}
	catch( const std::exception &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPTF0( _CRT_ERROR, ex.what( ) );
	}

	return true;
}

void c_ragebot::quickstop( c_base_combat_weapon *local_weapon ) {
	const weapon_info_t *weapon_info = local_weapon->get_weapon_info( );
	if( !weapon_info )
		return;

	// note: scoped weapons use the alternate speed member.
	const float max_speed = local_weapon->has_scope( ) ? weapon_info->max_speed_alt : weapon_info->max_speed;

	if( g_misc.unpredicted_vel.length_2d( ) > max_speed * .34f ) {
		const vec3_t velocity = g_misc.unpredicted_vel;
		const float_t speed = g_cl.m_local->velocity( ).length( );

		vec3_t direction;
		math::vector_angle( velocity, direction );

		direction.y = m_cmd->m_viewangles.y - direction.y;

		vec3_t forward;
		math::angle_to_vector( direction, forward );
		vec3_t negated_direction = forward * -speed;

		const float factor = std::max( negated_direction.x, negated_direction.y ) / 450.f;
		negated_direction *= factor;

		m_cmd->m_forwardmove = negated_direction.x;
		m_cmd->m_sidemove = negated_direction.y;
	}
}

void c_ragebot::auto_revolver( c_base_combat_weapon *local_weapon, c_user_cmd *cmd ) {
	if( !local_weapon || local_weapon->item_index( ) != WEAPON_REVOLVER )
		return;

	if( !( TIME_TO_TICKS( g_csgo.m_global_vars->m_cur_time - local_weapon->ready_time( ) ) <= -1 ) )
		return;

	cmd->m_buttons |= IN_ATTACK;
}

bool c_ragebot::is_valid( c_csplayer *player ) {
	if( !player || player->is_dormant( ) )
		return false;

	if( !player->alive( ) || player->flags( ) & FL_FROZEN )
		return false;

	if( player->team( ) == g_cl.m_local->team( ) && !g_vars.rage.teammate )
		return false;

	/*if( player->survival_team() != -1 )
		return false;*/

	if( player->immune( ) )
		return false;

	return true;
}
