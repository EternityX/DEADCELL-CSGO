#include "antiaim.h"

c_antiaim g_antiaim;

float at_target( ) {
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

void c_antiaim::adjust_pitch( CUserCmd *ucmd ) {
	switch ( g_vars.antiaim.pitch ) {
	case AA_PITCH_NONE: {
		ucmd->m_viewangles.x;
		break;
	}
	case AA_PITCH_DEFAULT: {
		ucmd->m_viewangles.x = 89.98f;
		break;
	}
	}
}

void c_antiaim::adjust_yaw( CUserCmd *ucmd ) {
	switch ( g_vars.antiaim.yaw ) {
	case AA_YAW_NONE: {
		ucmd->m_viewangles.y;
		break;
	}
	case AA_YAW_BACKWARDS: {
		ucmd->m_viewangles.y += 180.f;
		break;
	}
	}
}

void c_antiaim::set_angles( CUserCmd *ucmd ) {
	if ( !g_cl.m_local || !g_cl.m_local->alive( ) || !g_vars.antiaim.enabled || ( ucmd->m_buttons & IN_ATTACK ) != 0 )
		return;

	adjust_pitch( ucmd );
	adjust_yaw( ucmd );
}

float c_antiaim::best_head_yaw( ) {
	return 0.f;
}
