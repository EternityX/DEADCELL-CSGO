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

	g_csgo.m_game_event->AddListener( this, "player_hurt", false );
	g_csgo.m_game_event->AddListener( this, "item_purchase", false );
	g_csgo.m_game_event->AddListener( this, "bullet_impact", false );
	g_csgo.m_game_event->AddListener( this, "weapon_fire", false );

	if( !g_csgo.m_game_event->FindListener( this, "player_hurt" ) || !g_csgo.m_game_event->FindListener( this, "item_purchase" )
		|| !g_csgo.m_game_event->FindListener( this, "bullet_impact" ) || !g_csgo.m_game_event->FindListener( this, "weapon_fire" ) ) {
		_RPT0( _CRT_ERROR, "Failed to setup event listener(s). Ignoring this message will result in instability or features not working." );
		return false;
	}

	return true;
}

bool c_event_listener::remove( ) {
	g_csgo.m_game_event->RemoveListener( this );

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


void c_event_listener::FireGameEvent( IGameEvent *m_event ) {
	if( !m_event )
		return;

	if( !strcmp( m_event->GetName( ), "bullet_impact" ) ) {
		int user_id = m_event->GetInt( "userid" );

		auto ent = g_csgo.m_entity_list->Get< C_CSPlayer >( g_csgo.m_engine->GetPlayerForUserID( user_id ) );
		if( !ent )
			return;

		auto local = C_CSPlayer::get_local( );
		if ( !local || local != ent )
			return;

		vec3_t position{ m_event->GetFloat( "x" ), m_event->GetFloat( "y" ), m_event->GetFloat( "z" ) };

		if( g_vars.misc.bullet_impacts )
			g_csgo.m_debug_overlay->AddBoxOverlay( position, vec3_t( -2, -2, -2 ), vec3_t( 2, 2, 2 ), vec3_t( 0, 0, 0 ), 0, 0, 255, 127, g_vars.misc.bullet_impacts_duration );

		if( g_vars.visuals.impact ) {
			BeamInfo_t beam_info;
			beam_info.m_nType = TE_BEAMPOINTS;
			beam_info.m_pszModelName = "sprites/physbeam.vmt";
			beam_info.m_nModelIndex = -1;
			beam_info.m_flHaloScale = 0.f;
			beam_info.m_flLife = 4.f;
			beam_info.m_flWidth = 2.f;
			beam_info.m_flEndWidth = 2.f;
			beam_info.m_flFadeLength = 0.1f;
			beam_info.m_flAmplitude = 2.f;
			beam_info.m_flBrightness = 140.f;
			beam_info.m_flSpeed = 0.2f;
			beam_info.m_nStartFrame = 0;
			beam_info.m_flFrameRate = 0.f;
			beam_info.m_flRed = util::misc::get_random_float_range( 0.1f, 1.f ) * 255.f;
			beam_info.m_flGreen = util::misc::get_random_float_range( 0.1f, 1.f ) * 255.f;
			beam_info.m_flBlue = util::misc::get_random_float_range( 0.1f, 1.f ) * 255.f;
			beam_info.m_nSegments = 2;
			beam_info.m_bRenderable = true;
			beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

			// create beam backwards because it looks nicer.
			beam_info.m_vecStart = position;
			beam_info.m_vecEnd = local->eye_pos( );

			Beam_t *beam = g_csgo.m_render_beams->CreateBeamPoints( beam_info );
			if( beam )
				g_csgo.m_render_beams->DrawBeam( beam );
		}
	}

	// todo: look into this, weapon_fire isn't getting fired?
	if ( !strcmp( m_event->GetName( ), "weapon_fire" ) ) {
		auto local = C_CSPlayer::get_local( );
		if( !local || !local->alive( ) )
			return;

		if( !g_vars.misc.bullet_impacts )
			return;

		int user_id = m_event->GetInt( "userid" );
		if( user_id != g_csgo.m_engine->GetLocalPlayer( ) )
			return;

		trace_t trace;
		Ray_t ray;
		CTraceFilter filter;
		filter.m_skip = local;

		vec3_t rem, forward, right, up,
			src = local->eye_pos( );

		vec3_t view_angles = g_cl.m_cmd->m_viewangles;
		view_angles += local->punch_angle( ) * 2.f;

		math::angle_to_vectors( view_angles, &forward, &right, &up );

		forward *= local->get_active_weapon( )->get_weapon_info( )->range;
		rem = src + forward;

		g_csgo.m_engine_trace->TraceRay( Ray_t{ src, rem }, 0x46004003, &filter, &trace );
		g_csgo.m_debug_overlay->AddBoxOverlay( trace.endpos, vec3_t( -2, -2, -2 ), vec3_t( 2, 2, 2 ), vec3_t( 0, 0, 0 ), 255, 0, 0, 127, g_vars.misc.bullet_impacts_duration );
	}

	if( !strcmp( m_event->GetName( ), "player_hurt" ) ) {
		int attacker = g_csgo.m_engine->GetPlayerForUserID( m_event->GetInt( "attacker" ) );
		int target = g_csgo.m_engine->GetPlayerForUserID( m_event->GetInt( "userid" ) );
		if( attacker ==  g_csgo.m_engine->GetLocalPlayer( ) && target != g_csgo.m_engine->GetLocalPlayer( ) ) {

			auto *ent = g_csgo.m_entity_list->Get< C_CSPlayer >( g_csgo.m_engine->GetPlayerForUserID( target ) );

			player_info_t info;
			if( g_csgo.m_engine->GetPlayerInfo( target, &info ) && ent ){
				if( g_vars.misc.hitmarker_sound ) {
					std::string sound_path = "hitsounds\\" + g_menu.m_selected_sound_text;

					// you can do this better but im lazy.
					PlaySoundA( sound_path.c_str(), nullptr, SND_ASYNC );
				}

				std::string s = info.m_szPlayerName;
				std::transform( s.begin( ), s.end( ), s.begin( ), std::tolower );

				if( g_vars.visuals.hitmarker )
					g_cl.m_hitmarker_alpha = 1.f;

				if( g_vars.misc.log_damage )
					g_notify.add( true, OSHGui::Drawing::Color::FromARGB( 220, 249, 44, 69 ), "hit %s in the %s for %i damage.", s.c_str( ),
						hitgroup_to_name( m_event->GetInt( "hitgroup" ) ), m_event->GetInt( "dmg_health" ) );
			}
		}
	}
	else if( !strcmp( m_event->GetName( ), "item_purchase" ) ) {
		int user =  g_csgo.m_engine->GetPlayerForUserID( m_event->GetInt( "userid" ) );

		auto local = C_CSPlayer::get_local( );
		if ( !local || local->team( ) == m_event->GetInt( "team" ) )
			return;

		player_info_t info;
		if( g_csgo.m_engine->GetPlayerInfo( user, &info ) ){
			if( g_vars.misc.log_purchases )
				g_notify.add( true, OSHGui::Drawing::Color::FromARGB( 220, 249, 44, 69 ), "%s bought %s", info.m_szPlayerName, m_event->GetString( "weapon" ) );
		}
	}
}