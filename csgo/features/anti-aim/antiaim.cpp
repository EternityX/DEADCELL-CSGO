#include "antiaim.h"

c_antiaim g_antiaim;

bool c_antiaim::allow( CUserCmd *ucmd ){
	if( !g_vars.antiaim.enabled )
		return false;

	auto local = C_CSPlayer::get_local( );
	if ( !local || local->flags( ) & FL_FROZEN || !local->alive( ) ) {
		return false;
	}

	if( local->get_move_type( ) & ( MOVETYPE_LADDER || MOVETYPE_NOCLIP ) ){
		return false;
	}

	auto weapon = local->get_active_weapon( );
	if( !weapon ){
		return false;
	}

	if( ucmd->m_buttons & IN_USE ){
		return false;
	}

	if( weapon->get_weapon_info( )->type == WEAPONTYPE_KNIFE ){
		float next_secondary_attack = weapon->next_sec_attack( ) - g_csgo.m_global_vars->m_cur_time;
		float next_primary_attack = weapon->next_attack( ) - g_csgo.m_global_vars->m_cur_time;
		if( ( ucmd->m_buttons & IN_ATTACK && next_primary_attack <= 0.f ) || ( ucmd->m_buttons & IN_ATTACK2 && next_secondary_attack <= 0.f ) ){
			return false;
		}
	}

	if( ucmd->m_buttons & IN_ATTACK && local->can_shoot( weapon ) ){
		return false;
	}

	if ( local->get_active_weapon( )->is_being_thrown( ) ) {
		return false;
	}

	return true;
}

void c_antiaim::adjust_yaw( CUserCmd *ucmd ) {
	switch( g_vars.antiaim.yaw ){
		case 1:
			at_target( ) != 0.f ? m_input.y = at_target( ) - 180.f + g_vars.antiaim.jitter : m_input.y += 180.f + g_vars.antiaim.jitter;
			break;
		default : break;
	}
}

void c_antiaim::adjust_pitch( CUserCmd *ucmd ) {
	switch( g_vars.antiaim.pitch ){
		case 1:
			m_input.x = 89.f;
			break;
		default : break;
	}
}

void c_antiaim::set_angles( CUserCmd *ucmd ) {
	if( !allow( ucmd ) )
		return;

	m_input = ucmd->m_viewangles;

	adjust_yaw( ucmd );

	adjust_pitch( ucmd );

	ucmd->m_viewangles = m_input;
}


float c_antiaim::at_target( ) {
	auto local = C_CSPlayer::get_local( );
	if ( !local )
		return 0.f;

	float best_fov = 360.f;
	vec3_t best_angles = vec3_t( 0.0f, 0.f, 0.0f );

	for ( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
		auto player = g_csgo.m_entity_list->Get< C_CSPlayer >( i );

		if ( !player || player == local ) {
			continue;
		}
		if ( !player->is_valid_player( false, true ) ) {
			continue;
		}
		if ( player->team( ) == local->team( ) ) {
			continue;
		}

		vec3_t local_position = local->abs_origin( );
		vec3_t target_position = player->abs_origin( );

		vec3_t viewangle;
		g_csgo.m_engine->GetViewAngles( viewangle );

		const float fov = math::get_fov( viewangle, math::calc_angle( local->eye_pos( ), player->eye_pos( ) ) );

		if ( fov < best_fov ) {
			best_fov = fov;
			best_angles = math::to_angle( target_position - local_position );

			if ( !math::normalize_angles( best_angles ) )
				return 0.f;
		}
	}

	return best_angles.y;
}
