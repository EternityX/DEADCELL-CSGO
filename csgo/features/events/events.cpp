#include "events.h"
#include "../misc/misc.h"
#include "../ragebot/ragebot.h"

#pragma comment( lib, "Winmm.lib" )

c_event_listener g_events;

c_event_listener::c_event_listener( ) { }

c_event_listener::~c_event_listener( ) {
	if( !m_eventlistener )
		return;

	//g_csgo.m_game_event->RemoveListener( this );
}

bool c_event_listener::setup( ) {
	m_eventlistener = new c_event_listener( );
	if( !m_eventlistener )
		return false;

	g_csgo.m_game_event->add_listener( this, "player_hurt", false );
	g_csgo.m_game_event->add_listener( this, "item_purchase", false );
	g_csgo.m_game_event->add_listener( this, "bullet_impact", false );
	g_csgo.m_game_event->add_listener( this, "weapon_fire", false );
	g_csgo.m_game_event->add_listener( this, "player_footstep", false );

	if( !g_csgo.m_game_event->find_listener( this, "player_hurt" ) || !g_csgo.m_game_event->find_listener( this, "item_purchase" )
		|| !g_csgo.m_game_event->find_listener( this, "bullet_impact" ) || !g_csgo.m_game_event->find_listener( this, "weapon_fire" ) ) {
		_RPT0( _CRT_ERROR, "Failed to setup event listener(s). Ignoring this message will result in instability or features not working." );
		return false;
	}

	return true;
}

bool c_event_listener::remove( ) {
	g_csgo.m_game_event->remove_listener( this );

	return true;
}

char *hitgroup_to_name( const int hitgroup ) {
	switch( hitgroup ) {
		case HITGROUP_HEAD:
			return "head";
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return "leg";
		case HITGROUP_STOMACH:
			return "stomach";
		default:
			return "body";
	}
}

void c_event_listener::fire_game_event( i_game_event *m_event ) {
	if( !m_event )
		return;

	if( !strcmp( m_event->get_name( ), "bullet_impact" ) ) {
		int user_id = m_event->get_int( "userid" );

		auto ent = g_csgo.m_entity_list->get< c_csplayer >( g_csgo.m_engine->get_player_for_user_id( user_id ) );
		if( !ent )
			return;

		auto local = c_csplayer::get_local( );
		if ( !local || local != ent )
			return;

		vec3_t position{ m_event->get_float( "x" ), m_event->get_float( "y" ), m_event->get_float( "z" ) };

		if( g_vars.misc.bullet_impacts )
			g_csgo.m_debug_overlay->add_box_overlay( position, vec3_t( -2, -2, -2 ), vec3_t( 2, 2, 2 ), vec3_t( 0, 0, 0 ), 0, 0, 255, 127, g_vars.misc.bullet_impacts_duration );

		if( g_vars.visuals.impact ) {
			beam_info_t beam_info;
			beam_info.m_type = TE_BEAMPOINTS;
			beam_info.m_model_name = "sprites/white.vmt";
			beam_info.m_model_index = -1;
			beam_info.m_halo_scale = 0.f;
			beam_info.m_life = 4.f;
			beam_info.m_width = 1.f;
			beam_info.m_end_width = 1.f;
			beam_info.m_fade_length = 1.f;
			beam_info.m_amplitude = 2.f;
			beam_info.m_brightness = 110.f;
			beam_info.m_speed = 0.2f;
			beam_info.m_start_frame = 0;
			beam_info.m_frame_rate = 0.f;
			beam_info.m_red = util::misc::get_random_float_range( 0.5f, 1.f ) * 255.f;
			beam_info.m_green = util::misc::get_random_float_range( 0.5f, 1.f ) * 255.f;
			beam_info.m_blue = util::misc::get_random_float_range( 0.5f, 1.f ) * 255.f;
			beam_info.m_num_segments = 2;
			beam_info.m_renderable = true;
			beam_info.m_flags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

			// create beam backwards because it looks nicer.
			beam_info.m_start = position;
			beam_info.m_end = local->eye_pos( );

			beam_t *beam = g_csgo.m_render_beams->create_beam_points( beam_info );
			if( beam )
				g_csgo.m_render_beams->draw_beam( beam );
		}
	}

	if( !strcmp( m_event->get_name( ), "weapon_fire" ) ) {
		int user_id = m_event->get_int( "userid" );

		if( !g_vars.misc.bullet_impacts )
			return;

		auto local = c_csplayer::get_local( );
		if( !local || !local->alive( ) )
			return;

		auto ent = g_csgo.m_entity_list->get< c_csplayer >( g_csgo.m_engine->get_player_for_user_id( user_id ) );
		if( !ent || local != ent )
			return;

		trace_t trace;
		ray_t ray;
		c_trace_filter filter;
		filter.m_skip = local;

		vec3_t rem, forward, right, up,
			src = local->eye_pos( );

		static cvar* weapon_recoil_scale = g_csgo.m_convar->find_var("weapon_recoil_scale");

		vec3_t view_angles = g_cl.m_cmd->m_viewangles;
		view_angles += local->punch_angle( ) * weapon_recoil_scale->get_float();

		math::angle_to_vectors( view_angles, &forward, &right, &up );

		forward *= local->get_active_weapon( )->get_weapon_info( )->range;
		rem = src + forward;

		g_csgo.m_engine_trace->trace_ray( ray_t{ src, rem }, 0x46004003, &filter, &trace );
		g_csgo.m_debug_overlay->add_box_overlay( trace.m_endpos, vec3_t( -2, -2, -2 ), vec3_t( 2, 2, 2 ), vec3_t( 0, 0, 0 ), 255, 0, 0, 127, g_vars.misc.bullet_impacts_duration );
	}

	if( !strcmp( m_event->get_name( ), "player_hurt" ) ) {
		int attacker = g_csgo.m_engine->get_player_for_user_id( m_event->get_int( "attacker" ) );
		int target = g_csgo.m_engine->get_player_for_user_id( m_event->get_int( "userid" ) );
		if( attacker == g_csgo.m_engine->get_local_player( ) && target != g_csgo.m_engine->get_local_player( ) ) {
			auto *ent = g_csgo.m_entity_list->get< c_csplayer >( target );

			player_info_t info;
			if( g_csgo.m_engine->get_player_info( target, &info ) && ent ){
				if( g_vars.misc.hitmarker_sound ) {
					std::string sound_path = "hitsounds\\" + g_menu.m_selected_sound_text;

					// you can do this better but im lazy.
					PlaySoundA( sound_path.c_str(), nullptr, SND_ASYNC );
				}

				std::string s = info.m_player_name;
				std::transform( s.begin( ), s.end( ), s.begin( ), tolower );

				if( g_vars.visuals.hitmarker )
					g_cl.m_hitmarker_alpha = 1.f;

				if( g_vars.misc.log_damage )
					g_notify.add( true, OSHGui::Drawing::Color::FromARGB( 220, 249, 44, 69 ), "hit %s in the %s for %i damage.", s.c_str( ),
						hitgroup_to_name( m_event->get_int( "hitgroup" ) ), m_event->get_int( "dmg_health" ) );
			}
		}
	}
	else if( !strcmp( m_event->get_name( ), "item_purchase" ) ) {
		int user =  g_csgo.m_engine->get_player_for_user_id( m_event->get_int( "userid" ) );

		auto local = c_csplayer::get_local( );
		if ( !local || local->team( ) == m_event->get_int( "team" ) )
			return;

		player_info_t info;
		if( g_csgo.m_engine->get_player_info( user, &info ) ){
			if( g_vars.misc.log_purchases )
				g_notify.add( true, OSHGui::Drawing::Color::FromARGB( 220, 249, 44, 69 ), "%s bought %s", info.m_player_name, m_event->get_string( "weapon" ) );
		}
	}

	if( !strcmp( m_event->get_name(), "player_footstep" ) ) {
		/*int user_id = m_event->get_int( "userid" );
		auto ent = g_csgo.m_entity_list->get< C_CSPlayer >( g_csgo.m_engine->GetPlayerForUserID( user_id ) );
		if( !ent )
			return;

		auto local = C_CSPlayer::get_local( );
		if( !local || ent == local )
			return;

		if( g_vars.visuals.filter == 1 && ent->team( ) == local->team( ) )
			return;

		BeamInfo_t beam_info;

		beam_info.m_nType = TE_BEAMRINGPOINT;
		beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
		beam_info.m_nModelIndex = g_csgo.m_model_info->GetModelIndex( "sprites/purplelaser1.vmt" );
		beam_info.m_nHaloIndex = -1;
		beam_info.m_flHaloScale = 5;
		beam_info.m_flLife = 5.f;
		beam_info.m_flWidth = 5.f;
		beam_info.m_flFadeLength = 1.f;
		beam_info.m_flAmplitude = 0.f;
		beam_info.m_flRed = util::misc::get_random_float_range( 0.1f, 1.f ) * 255.f;
		beam_info.m_flGreen = util::misc::get_random_float_range( 0.1f, 1.f ) * 255.f;
		beam_info.m_flBlue = util::misc::get_random_float_range( 0.1f, 1.f ) * 255.f;
		beam_info.m_flBrightness = 255.f;
		beam_info.m_flSpeed = 0.f;
		beam_info.m_nStartFrame = 0;
		beam_info.m_flFrameRate = 1.f;
		beam_info.m_nSegments = -1;
		beam_info.m_nFlags = FBEAM_FADEOUT;
		beam_info.m_vecCenter = ent->abs_origin( ) + vec3_t( 0, 0, 5 );
		beam_info.m_flStartRadius = 0.f;
		beam_info.m_flEndRadius = 75.f;

		const auto beam = g_csgo.m_render_beams->CreateBeamRingPoint( beam_info );

		if( beam )
			g_csgo.m_render_beams->DrawBeam( beam );*/
	}
}