#include "antiaim.h"

c_antiaim g_antiaim;

int chai_curr_x = -1; // makes chaiscript easier
int chai_curr_y = -1; // makes chaiscript easier

bool c_antiaim::allow( c_user_cmd *ucmd ) {
	if( !g_vars.antiaim.enabled )
		return false;

	auto local = c_csplayer::get_local( );
	if( !local || local->flags( ) & FL_FROZEN || !local->alive( ) )
		return false;

	const auto move_type = local->get_move_type( );
	if( move_type == MOVETYPE_LADDER || move_type == MOVETYPE_NOCLIP )
		return false;

	if( ucmd->m_buttons & IN_USE )
		return false;

	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return false;

	if( weapon->get_weapon_info( )->type == WEAPONTYPE_KNIFE ) {
		float next_secondary_attack = weapon->next_sec_attack( ) - g_csgo.m_global_vars->m_cur_time;
		float next_primary_attack = weapon->next_attack( ) - g_csgo.m_global_vars->m_cur_time;

		if( ucmd->m_buttons & IN_ATTACK && next_primary_attack <= 0.f || ucmd->m_buttons & IN_ATTACK2 && next_secondary_attack <= 0.f )
			return false;
	}

	if( ucmd->m_buttons & IN_ATTACK && local->can_shoot( weapon ) ) {
		m_desync_next_tick = true;
		g_cl.m_sendpacket = false;
		return false;
	}

	if( local->get_active_weapon( )->is_being_thrown( ) )
		return false;

	return true;
}

void c_antiaim::chai_yaw( c_user_cmd *ucmd ) {
	if( g_vars.antiaim.yaw > 1 ) {
		// chaiscript antiaim yaw
		if( chai_curr_y != g_vars.antiaim.yaw ) {
			try {
				chai_curr_y = g_vars.antiaim.yaw;
				yaw_aa.chai_init( );
				yaw_aa.chai_load( "profiles//aa/y//" + chai_manager::chai_aa_yaw.at( g_vars.antiaim.yaw ) );
				chai_manager::chai_print( "profiles//aa//y//" + chai_manager::chai_aa_yaw.at( g_vars.antiaim.yaw ) );
			}
			catch( const std::exception &e ) {
				//chai_manager::chai_error(e.what());
			}
		}
		else {
			if( yaw_aa.chai_finished_init ) {
				try {
					auto func = yaw_aa.chai->eval< std::function< void( c_user_cmd *, vec3_t *, bool *, bool * ) > >( "create_move" );
					bool side = g_input.key_pressed( g_vars.antiaim.side_switch_key );
					func( ucmd, &m_input, &g_cl.m_sendpacket, &side );
				}
				catch( const std::exception &e ) {
					//chai_manager::chai_error(e.what());
				}
			}
		}

	}
}

void c_antiaim::chai_pitch( c_user_cmd *ucmd ) {
	if( g_vars.antiaim.pitch > 1 ) {
		// chaiscript antiaim pitch
		if( chai_curr_x != g_vars.antiaim.pitch ) {
			try {
				chai_curr_x = g_vars.antiaim.pitch;
				pitch_aa.chai_init( );
				pitch_aa.chai_load( "profiles//aa/x//" + chai_manager::chai_aa_pitch.at( g_vars.antiaim.pitch ) );
				chai_manager::chai_print( "profiles//aa//x//" + chai_manager::chai_aa_pitch.at( g_vars.antiaim.pitch ) );
			}
			catch( const std::exception &e ) {
				//chai_manager::chai_error(e.what());
			}
		}
		else {
			if( pitch_aa.chai_finished_init ) {
				try {
					auto func = pitch_aa.chai->eval< std::function< void( c_user_cmd *, vec3_t * ) > >( "create_move" );
					func( ucmd, &m_input );
				}
				catch( const std::exception &e ) {
					//chai_manager::chai_error(e.what());
				}
			}
		}

	}
}

void c_antiaim::adjust_yaw( c_user_cmd *ucmd ) {
	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	static bool side = true;
	static int last_tickcount = 0;

	if( g_input.key_pressed( g_vars.antiaim.side_switch_key ) && last_tickcount < int( GetTickCount( ) ) ) {
		side = !side;
		last_tickcount = int( GetTickCount( ) ) + 250;
	}

	if( !g_cl.m_sendpacket && !g_csgo.m_clientstate->m_choked_commands ) {
		switch( g_vars.antiaim.yaw ) {
			case 1:
				if( side )
					at_target( ) != 0.f ? m_input.y = at_target( ) - 90.f : m_input.y -= 90.f;
				else
					at_target( ) != 0.f ? m_input.y = at_target( ) + 90.f : m_input.y += 90.f;
				break;
			default:
				if( g_vars.antiaim.yaw > 1 )
					chai_yaw( ucmd );
				break;
		}

		m_stored_input = m_input;
		math::normalize_angles( m_stored_input );
	}
	else if( g_cl.m_sendpacket ) {
		m_input.y = m_stored_input.y - ( g_cl.m_local->max_desync( ) );
	}
	else if( m_desync_next_tick ) {
		m_input.y = m_stored_input.y - ( g_cl.m_local->max_desync( ) );
		g_cl.m_sendpacket = true;
		m_desync_next_tick = false;
	}

	if( !g_cl.m_sendpacket ) {
		m_input.y = m_stored_input.y;
	}
}

void c_antiaim::adjust_pitch( c_user_cmd *ucmd ) {
	switch( g_vars.antiaim.pitch ) {
		case 1:
			m_input.x = 89.f;
			break;
		default:
			if( g_vars.antiaim.pitch > 1 )
				chai_pitch( ucmd );
			break;
	}
}

void c_antiaim::set_angles( c_user_cmd *ucmd ) {
	if( !allow( ucmd ) )
		return;

	m_input = ucmd->m_viewangles;

	adjust_yaw( ucmd );

	adjust_pitch( ucmd );

	ucmd->m_viewangles = m_input;
}

float c_antiaim::at_target( ) {
	auto local = c_csplayer::get_local( );
	if( !local )
		return 0.f;

	float best_fov = 360.f;
	vec3_t best_angles = vec3_t( 0.0f, 0.f, 0.0f );

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
		auto player = g_csgo.m_entity_list->get< c_csplayer >( i );

		if( !player || player == local )
			continue;
		if( !player->is_valid_player( false, true ) )
			continue;
		if( player->team( ) == local->team( ) )
			continue;

		vec3_t local_position = local->abs_origin( );
		vec3_t target_position = player->abs_origin( );

		vec3_t viewangle;
		g_csgo.m_engine->get_viewangles( viewangle );

		const float fov = math::get_fov( viewangle, math::calc_angle( local->eye_pos( ), player->eye_pos( ) ) );

		if( fov < best_fov ) {
			best_fov = fov;
			best_angles = math::to_angle( target_position - local_position );

			if( !math::normalize_angles( best_angles ) )
				return 0.f;
		}
	}

	return best_angles.y;
}
