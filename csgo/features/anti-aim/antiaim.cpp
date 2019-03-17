#include "antiaim.h"

c_antiaim g_antiaim;

// removed most of the code here due to spaghetti.

void c_antiaim::ensure_sanity( CUserCmd *ucmd ) {
	
}

float at_target( ) {
	auto local = C_CSPlayer::get_local( );
	if( !local )
		return 0.f;

	float best_fov = 360.f;
	vec3_t best_angles = vec3_t( 0.0f, 0.f, 0.0f );

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
		C_CSPlayer *player = static_cast< C_CSPlayer * >( g_csgo.m_entity_list->GetClientEntity( i ) );

		if( !player || player == local ) {
			continue;
		}
		if( !player->is_valid_player( false, true ) ) {
			continue;
		}
		if( player->team( ) == local->team( ) ) {
			continue;
		}

		vec3_t local_position = local->abs_origin( );
		vec3_t target_position = player->abs_origin( );

		vec3_t viewangle;
		g_csgo.m_engine->GetViewAngles( viewangle );

		float fov = math::get_fov( viewangle, math::calc_angle( local->eye_pos( ), player->eye_pos( ) ) );

		if( fov < best_fov ) {
			best_fov = fov;
			best_angles = math::to_angle( target_position - local_position );

			if( !math::normalize_angles( best_angles ) )
				return 0.f;
		}
	}

	return best_angles.y;
}

float c_antiaim::calculate_max_desync_angle( C_CSPlayer *local, CCSGOPlayerAnimState *anim_state ) {
	return 0.f;
}

void c_antiaim::adjust_yaw( CUserCmd *ucmd ) {

}

void c_antiaim::adjust_pitch( CUserCmd *ucmd ) {

}

void c_antiaim::set_angles( CUserCmd *ucmd, bool& bSendPacket ) {
	
	if(bSendPacket ){
		if (ucmd->m_buttons & IN_ATTACK || ucmd->m_buttons & IN_USE)
			return;
	switch (g_vars.antiaim.yaw) {
		case 1:
			ucmd->m_viewangles.y += 180;
			break;
		}
	switch (g_vars.antiaim.pitch) {
	case 1:
		ucmd->m_viewangles.x = 89;
		
		break;
	}
		
	_real = ucmd->m_viewangles;
	}
}

void c_antiaim::manual( CUserCmd *ucmd ) {

}

void c_antiaim::modify_packets( CUserCmd *ucmd ) {
	
}

float c_antiaim::best_head_yaw( ) {
	return 0.f;
}
