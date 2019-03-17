#include "ragebot.h"

c_ragebot g_ragebot;

void c_ragebot::work( CUserCmd * cmd ) {
	m_players.clear( );

	if ( !g_vars.rage.enabled )
		return;

	if( g_vars.rage.key && !g_input.key_pressed( g_vars.rage.key ) )
		return;

	m_cmd = cmd;

	select_target( );

	choose_angles( );
}

void c_ragebot::select_target( ) {
	auto local = C_CSPlayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;

	auto wpn_index = weapon->item_index( );

	if( wpn_index == WEAPON_FISTS
		|| wpn_index == WEAPON_SPANNER
		|| wpn_index == WEAPON_HAMMER
		|| wpn_index == WEAPON_AXE
		|| wpn_index == WEAPON_MELEE
		|| wpn_index == WEAPON_BREACHCHARGE
		|| wpn_index == WEAPON_TABLET )
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
			auto e = g_csgo.m_entity_list->Get< C_CSPlayer >( i );

			if( !is_valid( e ) )
				continue;

			float player_best_damage = 0.f;
			vec3_t player_best_point = vec3_t( 0.f, 0.f, 0.f );
			std::array< matrix3x4_t, 128 > _bones = { };
			
			if( !e->SetupBones( _bones.data( ), 128, 0x100, e->simtime( ) + g_csgo.m_global_vars->m_interval_per_tick ) )
				continue;

			std::vector< vec3_t > points;
			if( !get_points_from_hitbox( e, hitboxes, _bones, points, ( g_vars.rage.pointscale / 100.f ) ) )
				continue;

			if( points.empty( ) )
				continue;

			for( auto& p : points ) {
				if( g_vars.visuals.extra.points )
					g_csgo.m_debug_overlay->AddBoxOverlay( p, vec3_t( -0.7f, -0.7f, -0.7f ), vec3_t( 0.7f, 0.7f, 0.7f ), vec3_t( 0.f, 0.f, 0.f ), 0, 255, 0, 100, g_csgo.m_global_vars->m_interval_per_tick * 2 );

				if ( g_autowall.think( p, e, g_vars.rage.min_dmg, true ) ) {
					if( g_autowall.m_autowall_dmg > player_best_damage ) {
						player_best_damage = g_autowall.m_autowall_dmg;
						player_best_point = p;
					}
				}
			}
			float distance = e->abs_origin( ).DistTo( local->abs_origin( ) );
			rage_t data = rage_t( e, i, player_best_damage, player_best_point, distance, _bones );
			m_players.push_back( data );
		}

		std::sort( m_players.begin( ), m_players.end( ), [ & ] ( rage_t &a, rage_t &b ) {
			switch( g_vars.rage.target_selection ) {
				case 0:
					return a.m_damage > b.m_damage;
				case 1:
					return a.distance < b.distance;
				case 2:
					return a.index < b.index;
			}
		} );
	}
	catch( ... ) {
		console::error( "caught exception in select_target( )" );
	}
}

bool c_ragebot::hitchance( vec3_t &angle, C_CSPlayer *ent ) {
	int traces_hit = 0;
	auto local = g_csgo.m_entity_list->Get< C_CSPlayer >( g_csgo.m_engine->GetLocalPlayer( ) );
	if( !local )
		return false;

	vec3_t forward, right, up, eye_position = local->eye_pos( );
	math::angle_to_vectors( angle + local->punch_angle( ) * 2.f, &forward, &right, &up );// maybe add an option to not account for punch.

	C_BaseCombatWeapon *weapon = local->get_active_weapon( );
	if ( !weapon )
		return false;

	weapon->update_accuracy( );
	float weapon_spread = weapon->spread( );
	float weapon_cone = weapon->inaccuracy( );

	auto get_bullet_location = [ & ] ( int seed ) {
		static auto random_seed = ( void( *)( int ) )GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" );
		random_seed( seed );


		float a = util::misc::get_random_float_range( 0.f, 1.f );
		float b = util::misc::get_random_float_range( 0.f, 2.f * math::pi );
		float c = util::misc::get_random_float_range( 0.f, 1.f );
		float d = util::misc::get_random_float_range( 0.f, 2.f * math::pi );

		float generated_spread = a * weapon_spread, generated_cone = c * weapon_cone;

		vec3_t spread = vec3_t (
			std::cos( b ) * generated_spread + std::cos( d ) * generated_cone,
			std::sin( b ) * generated_spread + std::sin( d ) * generated_cone,
			0.f
		);

		return vec3_t ( forward + right * -spread.x + up * -spread.y ).Normalized( );
	};

	for ( int i = 1; i <= 256; i++ ) {
		vec3_t spread_angle;
		vec3_t bullet_end;

		math::vector_angle( get_bullet_location( i ), spread_angle );
		math::angle_to_vector( angle - ( spread_angle - angle ), bullet_end );

		trace_t trace;
		Ray_t ray;
		ray.init( eye_position, eye_position + bullet_end * weapon->get_weapon_info( )->range );
		g_csgo.m_engine_trace->ClipRayToEntity( ray, MASK_SHOT, ent, &trace );

		if ( trace.hit_entity == ent )
			++traces_hit;

		if ( traces_hit >= (int)( g_vars.rage.hitchance * 2.56f ) )
			return true;
	}

	return false;
}

void c_ragebot::choose_angles( ){
	C_CSPlayer*		selected_target = nullptr;
	vec3_t			best_hitboxpos;
	float			best_damage = 0.f;

	auto local = C_CSPlayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;

	std::array< matrix3x4_t, 128 > mat;

	for ( auto &data : m_players ) {
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
		
		if( calc_damage( ) ) {
			break;
		}
		
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
	
	const WeaponInfo_t *wep_info = weapon->get_weapon_info( );
	if( wep_info->type == WEAPONTYPE_PISTOL && !wep_info->full_auto ){
		static auto firing = false;
		if( m_cmd->m_buttons & IN_ATTACK ) {
			if( firing ) {
				m_cmd->m_buttons &= ~IN_ATTACK;
			}
		}

		firing = ( m_cmd->m_buttons & IN_ATTACK ) != 0;
	}

	if( m_cmd->m_buttons & IN_ATTACK ){
		m_cmd->m_viewangles = aim_angle;

		weapon->update_accuracy( );
		m_cmd->m_viewangles -= local->punch_angle( ) * 2.f;

		if( g_vars.misc.clienthitboxes )
			g_misc.capsule_overlay( selected_target, g_vars.misc.duration, mat );

		if( !g_vars.rage.silent ){
			g_csgo.m_engine->SetViewAngles( m_cmd->m_viewangles );
		}
	}
}

std::vector< lag_record_t > c_ragebot::get_best_records( std::deque< lag_record_t > records ){
	std::vector< lag_record_t > output{ };
	for( size_t i = 0; i < records.size( ); ++i ){
		auto lag_record = records.at( i );

	}

	return output;
}

bool c_ragebot::get_points_from_hitbox( C_CSPlayer *e, std::vector< int > hitboxes, std::array< matrix3x4_t, 128 > matrix, std::vector< vec3_t >& points, float scale ) {
	if ( !e )
		return false;

	auto *studiohdr = e->studio_hdr( );
	if ( !studiohdr )
		return false;

	auto local = C_CSPlayer::get_local( );
	if( !local )
		return false;

	try {
		for( int idx = 0; idx < hitboxes.size( ); ++idx ) {
			int h = hitboxes.at( idx );
			auto *bbox = studiohdr->pHitbox( h, 0 );

			vec3_t max, min;
			math::vector_transform( bbox->bb_min, matrix.at( bbox->bone_index ), min );
			math::vector_transform( bbox->bb_max, matrix.at( bbox->bone_index ), max );
			auto center = ( min + max ) / 2.f;

			points.push_back( center );
			

			// don't bother multipointing if body/head is visible.
			// pretty sure this is called dynamic hitboxes in aimware.
			if( g_vars.rage.dynamic_hitbox ) {
				// get current framerate
				static float framerate;
				framerate = 0.9 * framerate + ( 1.0 - 0.9 ) * g_csgo.m_global_vars->m_absolute_frametime;

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
				float length = ( bbox->bb_min - bbox->bb_max ).Length( ) + bbox->m_flRadius * 2.f;

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
	catch( ... ) {
		console::error( "caught exception in get_points_from_hitbox( )" );
	}

	return true;
}

void c_ragebot::quickstop( C_BaseCombatWeapon *local_weapon ) {
	const WeaponInfo_t *weapon_info = local_weapon->get_weapon_info( );
	if( !weapon_info )
		return;

	// note: scoped weapons use the alternate speed member.
	float max_speed = local_weapon->has_scope( ) ? weapon_info->max_speed_alt : weapon_info->max_speed;

	if( g_misc.unpredicted_vel.Length2D( ) > max_speed * .34f ) {
		vec3_t velocity = g_misc.unpredicted_vel;
		float_t speed = g_cl.m_local->velocity( ).Length( );

		vec3_t direction;
		math::vector_angle( velocity, direction );

		direction.y = m_cmd->m_viewangles.y - direction.y;

		vec3_t forward;
		math::angle_to_vector( direction, forward );
		vec3_t negated_direction = forward * -speed;

		float factor = std::max( negated_direction.x, negated_direction.y ) / 450.f;
		negated_direction *= factor;

		m_cmd->m_forwardmove = negated_direction.x;
		m_cmd->m_sidemove = negated_direction.y;
	}
}

bool c_ragebot::is_valid( C_CSPlayer *player ){
	if( !player || player->IsDormant() )
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