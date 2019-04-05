#include "ragebot.h"

c_ragebot g_ragebot;

void c_ragebot::work( c_user_cmd * cmd ) {
	m_players.clear( );

	if( !g_vars.rage.enabled )
		return;

	if( g_vars.rage.key && !g_input.key_pressed( g_vars.rage.key ) )
		return;

	m_cmd = cmd;

	select_target( );

	choose_angles( );
}

void c_ragebot::select_target( ) {
	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon || weapon->clip( ) <= 0 )
		return;

	try {
		std::vector< int > hitboxes = { };
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
			if( g_vars.rage.hitbox_head ){
				hitboxes.push_back( head );
				hitboxes.push_back( neck );
			}
			if( g_vars.rage.hitbox_pelvis ){
				hitboxes.push_back( l_chest );
				hitboxes.push_back( u_chest );
				hitboxes.push_back( thorax );
				hitboxes.push_back( pelvis );
			}
			if( g_vars.rage.hitbox_arms ){
				hitboxes.push_back( l_upperarm );
				hitboxes.push_back( r_upperarm );
				hitboxes.push_back( l_forearm );
				hitboxes.push_back( r_forearm );
				hitboxes.push_back( l_hand );
				hitboxes.push_back( r_hand );
			}
			if( g_vars.rage.hitbox_legs ){
				hitboxes.push_back( l_thigh );
				hitboxes.push_back( r_thigh );
				hitboxes.push_back( l_calf );
				hitboxes.push_back( r_calf );
				hitboxes.push_back( l_foot );
				hitboxes.push_back( r_foot );
			}
		}

		for ( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
			auto e = g_csgo.m_entity_list->get< c_csplayer >( i );

			if( !is_valid( e ) )
				continue;

			float player_best_damage = 0.f;
			vec3_t player_best_point = vec3_t( 0.f, 0.f, 0.f );
			matrix3x4_t* _bones;

			int backup_fl = e->flags( );

			e->flags( ) &= ~FL_ONGROUND;

			e->setup_bones( nullptr, -1, 0x7FF00, g_csgo.m_global_vars->m_cur_time );

			e->flags( ) = backup_fl;

			_bones= e->bone_cache( ).base( );

			std::vector< vec3_t > points;
			if( !get_points_from_hitbox( e, hitboxes, _bones, points, ( g_vars.rage.pointscale / 100.f ) ) )
				continue;

			if( points.empty( ) )
				continue;

			for( auto& p : points ) {
				if( g_vars.visuals.extra.points )
					g_csgo.m_debug_overlay->add_box_overlay( p, vec3_t( -0.7f, -0.7f, -0.7f ), vec3_t( 0.7f, 0.7f, 0.7f ), vec3_t( 0.f, 0.f, 0.f ), 0, 255, 0, 100, g_csgo.m_global_vars->m_interval_per_tick * 2 );

				if ( g_autowall.think( p, e, g_vars.rage.min_dmg, true ) ) {
					if( g_autowall.m_autowall_dmg > player_best_damage ) {
						player_best_damage = g_autowall.m_autowall_dmg;
						player_best_point = p;
					}
				}
			}

			m_players.emplace_back( e, i, static_cast< int >( player_best_damage ), player_best_point, e->abs_origin( ).distance( local->abs_origin( ) ), _bones );
		}

		std::sort( m_players.begin( ), m_players.end( ), [ & ] ( rage_t &a, rage_t &b ) {
			switch( g_vars.rage.target_selection ) {
				case 0:
					return a.m_damage > b.m_damage;
				case 1:
					return a.distance < b.distance;
				case 2:
					return a.index < b.index;
				default: return false;
			}
		} );
	}
	catch( const std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT0( _CRT_ERROR, ex.what( ) );
	}
}

bool c_ragebot::hitchance( vec3_t &angle, c_csplayer *ent ) {
	int traces_hit = 0;
	auto local = c_csplayer::get_local( );
	if( !local )
		return false;

	vec3_t forward, right, up;
	const vec3_t eye_position = local->eye_pos( );
	math::angle_to_vectors( angle + local->punch_angle( ) * 2.f, &forward, &right, &up );// maybe add an option to not account for punch.

	c_base_combat_weapon *weapon = local->get_active_weapon( );
	if ( !weapon )
		return false;

	weapon->update_accuracy( );
	float weapon_spread = weapon->spread( );
	float weapon_cone = weapon->inaccuracy( );

	const auto get_bullet_location = [ & ] ( int seed ) {
		static auto random_seed = reinterpret_cast< void( *)( int ) >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" ) );
		random_seed( seed );


		float a = util::misc::get_random_float_range( 0.f, 1.f );
		float b = util::misc::get_random_float_range( 0.f, 2.f * math::pi );
		float c = util::misc::get_random_float_range( 0.f, 1.f );
		float d = util::misc::get_random_float_range( 0.f, 2.f * math::pi );

		const float generated_spread = a * weapon_spread;
		const float generated_cone = c * weapon_cone;

		const vec3_t spread = vec3_t (
			std::cos( b ) * generated_spread + std::cos( d ) * generated_cone,
			std::sin( b ) * generated_spread + std::sin( d ) * generated_cone,
			0.f
		);

		return vec3_t ( forward + right * -spread.x + up * -spread.y ).Normalized( );
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

void c_ragebot::choose_angles( ){
	c_csplayer*		selected_target = nullptr;
	vec3_t			best_hitboxpos;
	float			best_damage = 0.f;

	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;

	matrix3x4_t* mat = nullptr;

	for( auto &data : m_players ) {
		auto target = data.m_player;

		auto calc_damage = [ & ] (  ) {
			float player_best_damage = data.m_damage;
			vec3_t player_best_point = data.m_bestpoint;

			if ( player_best_damage > best_damage ) {
				selected_target = target;
				best_hitboxpos = player_best_point;
				best_damage = player_best_damage;
				mat = data.mat;
			}
			if ( player_best_damage >= target->health( ) ) {
				selected_target = target;
				best_hitboxpos = player_best_point;
				best_damage = player_best_damage;
				mat = data.mat;
				return true;
			}
			return false;
		};
		
		if( calc_damage( ) )
			break;		
	}
	if ( !selected_target )
		return;

	m_last_target = selected_target;

	if ( !local->can_shoot( weapon ) )
		return;

	if( g_vars.rage.autoscope == 1 && ( !local->is_scoped( ) && weapon->has_scope( ) ) && selected_target ) 
		m_cmd->m_buttons |= IN_ATTACK2;

	vec3_t aim_angle = math::calc_angle( local->eye_pos( ), best_hitboxpos );
	aim_angle.Clamp( );

	if( !hitchance( aim_angle, selected_target ) ){
		if( g_vars.rage.autoscope == 2 && ( !local->is_scoped( ) && weapon->has_scope( ) ) )
			m_cmd->m_buttons |= IN_ATTACK2;

		if( g_vars.rage.autostop )
			quickstop( weapon );

		if( g_vars.rage.auto_fire )
			return;
	}

	if( g_vars.rage.auto_fire && !( m_cmd->m_buttons & IN_ATTACK ) ) {
		m_cmd->m_buttons |= IN_ATTACK;
		shots_fired++;
	}

	if( m_cmd->m_buttons & IN_ATTACK ){
		m_cmd->m_viewangles = aim_angle;

		weapon->update_accuracy( );
		m_cmd->m_viewangles -= local->punch_angle( ) * 2.f;

		if( g_vars.misc.client_hitboxes && mat )
			g_misc.capsule_overlay( selected_target, g_vars.misc.client_hitboxes_duration, mat );

		if( !g_vars.rage.silent )
			g_csgo.m_engine->set_viewangles( m_cmd->m_viewangles );
	}
}

std::vector< lag_record_t > c_ragebot::get_best_records( std::deque< lag_record_t > records ){
	std::vector< lag_record_t > output{ };
	for( size_t i = 0; i < records.size( ); ++i ){
		auto lag_record = records.at( i );

	}

	return output;
}

bool c_ragebot::get_points_from_hitbox( c_csplayer *e, std::vector< int > hitboxes, matrix3x4_t *matrix, std::vector< vec3_t >& points, float scale ) {
	if ( !e )
		return false;

	auto *studiohdr = e->studio_hdr( );
	if ( !studiohdr )
		return false;

	auto local = c_csplayer::get_local( );
	if( !local )
		return false;

	try {
		for( size_t idx = 0; idx < hitboxes.size( ); ++idx ) {
			int h = hitboxes.at( idx );
			auto *bbox = studiohdr->pHitbox( h, 0 );

			vec3_t max, min;
			math::vector_transform( bbox->bb_min, matrix[ bbox->bone_index ], min );
			math::vector_transform( bbox->bb_max, matrix[ bbox->bone_index ], max );
			auto center = ( min + max ) / 2.f;

			points.push_back( center );
			

			// don't bother multipointing if body/head is visible.
			// pretty sure this is called dynamic hitboxes in aimware.
			if( g_vars.rage.dynamic_hitbox ) {
				// framerate is lower than the servers, start optimizing points.
				if( g_cl.m_under_server_tick_rate && m_last_target ) {
					// check if we're aiming at the same target and if we are lets only multipoint this target until they are no longer the target.
					if( e != m_last_target )
						continue;
				}

				if( hitboxes.empty( ) || points.empty( ) )
					return false; // return false, we have nowhere to aim at

				if( points.size( ) >= hitboxes.size( ) ) {
					switch( h ) {
						case head:
						case neck:		
						case l_chest:
						case u_chest:
						case thorax:
						case pelvis:
							break;
						default: continue;
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

			switch( h ) {
				case head:
				case neck:		
					if( !g_vars.rage.head )
						continue;
					break;
				case l_chest:
				case u_chest:
				case thorax:
				case pelvis:
					if( !g_vars.rage.pelvis )
						continue;
					break;
				case l_upperarm:
				case r_upperarm:
					if( !g_vars.rage.arms )
						continue;
					break;
				case l_thigh:
				case r_thigh:
				case l_foot:
				case r_foot:
					if( !g_vars.rage.legs )
						continue;
					break;
				default: continue;
			}

			if( bbox->m_flRadius == -1.f ) {
				/*points.push_back( vec3_t( bbox->bb_min.x, bbox->bb_min.y, bbox->bb_min.z ) * scale );
				points.push_back( vec3_t( bbox->bb_min.x, bbox->bb_max.y, bbox->bb_min.z ) * scale );
				points.push_back( vec3_t( bbox->bb_max.x, bbox->bb_max.y, bbox->bb_min.z ) * scale );
				points.push_back( vec3_t( bbox->bb_max.x, bbox->bb_min.y, bbox->bb_min.z ) * scale );*/
			}
			else if( bbox->m_flRadius > 0.f ) { // pill.
				const float length = ( bbox->bb_min - bbox->bb_max ).Length( ) + bbox->m_flRadius * 2.f;

				if( h != l_upperarm && h != r_upperarm && h != l_thigh && h != r_thigh ) {
					points.push_back( center + vec3_t( length / 3.f, 0.f, 0.f ) );
					points.push_back( center - vec3_t( length / 3.f, 0.f, 0.f ) );
				}

				if( h != l_chest && h != u_chest )
					points.push_back( center + vec3_t( 0.f, 0.f, bbox->m_flRadius * scale ) ); // top

				if( h != u_chest && h != l_chest && h != thorax )
					points.push_back( center - vec3_t( 0.f, 0.f, bbox->m_flRadius * scale ) ); // bottom

				points.push_back( center + vec3_t( 0.f, bbox->m_flRadius * scale, 0.f ) ); // front center
				points.push_back( center - vec3_t( 0.f, bbox->m_flRadius * scale, 0.f ) ); // back center	
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

	if( g_misc.unpredicted_vel.Length2D( ) > max_speed * .34f ) {
		const vec3_t velocity = g_misc.unpredicted_vel;
		const float_t speed = g_cl.m_local->velocity( ).Length( );

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

bool c_ragebot::is_valid( c_csplayer *player ){
	if( !player || player->is_dormant() )
		return false;

	if( !player->alive() || player->flags( ) & FL_FROZEN )
		return false;

	if( player->team( ) == g_cl.m_local->team( ) && !g_vars.rage.teammate )
		return false;

	/*if( player->survival_team() != -1 )
		return false;*/

	if( player->immune( ) )
		return false;

	return true;
}