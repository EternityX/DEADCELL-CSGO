#include "visuals.h"
#include "../autowall/autowall.h"

c_visuals g_visuals;

void c_visuals::run( ) {
	if( g_vars.visuals.misc.remove_scope )
		draw_scope( );

	activation_type( );

	for( int i = 1; i <= g_csgo.m_entity_list->GetHighestEntityIndex( ); i++ ) {
		C_CSPlayer *entity = g_csgo.m_entity_list->Get< C_CSPlayer >( i );
		if( !entity )
			continue;

		if( g_vars.visuals.radar )
			entity->spotted( ) = true;

		player( entity );
		world( entity );
	}

	// glow should be called inside do_post_screen_effects, but this works for now... i guess...
	if( g_vars.visuals.glow )
		handle_glow( !ctx.m_enabled );

	if( g_vars.visuals.extra.speclist )
		draw_spectators( );

	if( g_vars.visuals.spread_circle )
		draw_crosshair( );

	if( g_vars.visuals.hitmarker )
		draw_hitmarker( );
}

bool c_visuals::world_to_screen( const vec3_t &origin, vec3_t &screen ) {
	// going to use the outdated matrix for now...
	const D3DMATRIX &matrix = g_csgo.m_engine->world_to_screen_matrix_d3d( );

	const float width = matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ];

	if( width > 0.01f ) {
		float inverse = 1.f / width;

		// todo: only grab display_size once.
		OSHGui::Drawing::SizeF display_size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

		screen.x = static_cast< float >( display_size.Width / 2 + ( 0.5 * ( ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) * inverse ) * display_size.Width + 0.5 ) );
		screen.y = static_cast< float >( display_size.Height / 2 - ( 0.5 * ( ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) * inverse ) * display_size.Height + 0.5 ) );
		screen.z = 0.0f;

		return true;
	}

	return false;
}

bool c_visuals::calculate_bbox( C_BaseEntity *entity, bbox_t &box ) {
	matrix3x4_t &tran_frame = entity->coord_frame( );

	vec3_t min = entity->mins( );
	vec3_t max = entity->maxs( );

	vec3_t screen_boxes[ 8 ];

	vec3_t points[] = {
		vec3_t( min.x, min.y, min.z ),
		vec3_t( min.x, max.y, min.z ),
		vec3_t( max.x, max.y, min.z ),
		vec3_t( max.x, min.y, min.z ),
		vec3_t( max.x, max.y, max.z ),
		vec3_t( min.x, max.y, max.z ),
		vec3_t( min.x, min.y, max.z ),
		vec3_t( max.x, min.y, max.z )
	};

	for( int i = 0; i <= 7; i++ )
		if( !world_to_screen( math::vector_transform( points[ i ], tran_frame ), screen_boxes[ i ] ) )
			return false;

	vec3_t box_array[] = {
		screen_boxes[ 3 ], // fl
		screen_boxes[ 5 ], // br
		screen_boxes[ 0 ], // bl
		screen_boxes[ 4 ], // fr
		screen_boxes[ 2 ], // fr
		screen_boxes[ 1 ], // br
		screen_boxes[ 6 ], // bl
		screen_boxes[ 7 ] // fl
	};

	float left = screen_boxes[ 3 ].x, bottom = screen_boxes[ 3 ].y, right = screen_boxes[ 3 ].x, top = screen_boxes[ 3 ].y;

	for( int i = 0; i <= 7; i++ ) {
		if( left > box_array[ i ].x )
			left = box_array[ i ].x;

		if( bottom < box_array[ i ].y )
			bottom = box_array[ i ].y;

		if( right < box_array[ i ].x )
			right = box_array[ i ].x;

		if( top > box_array[ i ].y )
			top = box_array[ i ].y;
	}

	box.x = static_cast< int >( left );
	box.y = static_cast< int >( top );
	box.w = static_cast< int >( right ) - left;
	box.h = static_cast< int >( bottom ) - top;

	return true;
}

void c_visuals::activation_type( ) {
	static bool is_down = false;
	static bool is_clicked = false;

	// hotkey stuff.
	if( g_vars.visuals.activation_type == 0 /* always */ ) {
		ctx.m_enabled = true;
	}
	else if( g_vars.visuals.activation_type == 1 /* on-key */ ) {
		if( g_input.key_pressed( g_vars.visuals.activation_key ) )
			ctx.m_enabled = true;
		else
			ctx.m_enabled = false;
	}
	else if( g_vars.visuals.activation_type == 2 /* toggle */ ) {
		if( g_input.key_pressed( g_vars.visuals.activation_key ) ) {
			is_clicked = false;
			is_down = true;
		}
		else if( !g_input.key_pressed( g_vars.visuals.activation_key ) && is_down ) {
			is_clicked = true;
			is_down = false;
		}
		else {
			is_clicked = false;
			is_down = false;
		}

		if( is_clicked )
			ctx.m_enabled = !ctx.m_enabled;
	}
}

void c_visuals::player( C_CSPlayer *e ) {
	auto local = C_CSPlayer::get_local( );
	if( !e->is_valid_player( false, false ) || !local )
		return;

	if( e->IsDormant( ) )
		return;
	if (g_vars.visuals.teammates) {
		if (local->team() == e->team())
			return;
	}
	auto index = e->GetIndex( );
	float step = 255.f * ( 20.f / 15.f ) * g_csgo.m_global_vars->m_frametime;

	g_vars.visuals.dormancy_fade ? e->IsDormant( ) 
		? m_alpha[ index ] -= step : m_alpha[ index ] += step * 5.f : e->IsDormant( )
		? m_alpha[ index ] = 0.f : m_alpha[ index ] = 220.f;

	if( m_alpha[ index ] > 220.f )
		m_alpha[ index ] = 220.f;
	if( m_alpha[ index ] < 0.f )
		m_alpha[ index ] = 0.f;

	if( m_alpha[ index ] == 0.f )
		return;

	if( !ctx.m_enabled )
		return;

	if( !local->alive( ) ) {
		const auto spec_handle = local->observer_handle( );
		auto spec = g_csgo.m_entity_list->Get< C_BaseEntity >( spec_handle );
		if( spec ) {
			if( spec == e )
				return;
		}
	}

	if( e == local )
		return;

	if( g_vars.visuals.filter == 1 && e->team( ) == local->team( ) )
		return;

	ctx.offset = 0;
	ctx.flag_count = 0;

	bbox_t box{};
	if( !calculate_bbox( e, box ) )
		return;

	const OSHColor name_color = OSHColor::FromARGB( g_vars.visuals.name_color, m_alpha[ index ] );
	const OSHColor box_color = OSHColor::FromARGB( g_vars.visuals.box_color, m_alpha[ index ] );
	const OSHColor ammo_bar_color = OSHColor::FromARGB( g_vars.visuals.ammo_bar_color, m_alpha[ index ] );
	const OSHColor weapon_color = OSHColor::FromARGB( g_vars.visuals.weapon_color, m_alpha[ index ] );
	const OSHColor flag_color = OSHColor::FromARGB( g_vars.visuals.flag_color, m_alpha[ index ] );
	const OSHColor money_color = OSHColor::FromARGB( g_vars.visuals.money_color, m_alpha[ index ] );
	const OSHColor flashed_color = OSHColor::FromARGB( g_vars.visuals.flash_bar_color, m_alpha[ index ] );

	auto weapon = e->get_active_weapon( );
	if( !weapon )
		return;

	const WeaponInfo_t *info = g_csgo.m_weapon_system->GetWpnData( weapon->item_index( ) );
	if( !info )
		return;

	if( g_vars.visuals.bbox )
		draw_box( box_color, e->GetIndex( ), box.x, box.y, box.w, box.h );

	if( g_vars.visuals.name ) {
		std::string name = e->get_info( ).m_szPlayerName;

		if( name.length( ) > 17 )
			name = name.substr( 0, 17 ) + "...";

		//std::transform( name.begin( ), name.end( ), name.begin( ), std::tolower );
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ),
		                      name_color,
		                      OSHColor::FromARGB( 220, 10, 10, 10 ),
		                      box.x + box.w * 0.5f, box.y - 12, CENTERED_X | DROPSHADOW,
		                      e->get_info( ).xuid_low == 0 ? name.append( " ( BOT ) " ) : name );
	}

	if( g_vars.visuals.healthbar )
		draw_healthbar( e, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.flash_bar )
		flash_bar( e, flashed_color, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.ammo_bar )
		ammo_bar( weapon, e, ammo_bar_color, box.x, box.y, box.w, box.h, weapon->clip( ) );

	if( g_vars.visuals.weapon )
		weapon_name( weapon, e, weapon_color, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.money )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
		                      money_color,
		                      OSHColor::FromARGB( 220, 10, 10, 10 ),
		                      box.x + box.w + 3, box.y + ctx.flag_count++ * 8, OUTLINED, "$%i", e->money( ) );

	if( g_vars.visuals.flags )
		draw_flags( e, flag_color, box.x, box.y, box.w, box.h );
}

void c_visuals::handle_glow( bool remove ) {
	auto local = C_CSPlayer::get_local( );
	if( !local )
		return;

	for( int i = 0; i < g_csgo.m_glow_obj_manager->m_size; i++ ) {
		if( g_csgo.m_glow_obj_manager->m_glow_object_definitions[ i ].is_unused( ) || !g_csgo.m_glow_obj_manager->m_glow_object_definitions[ i ].get_entity( ) )
			continue;

		auto &glow_object = g_csgo.m_glow_obj_manager->m_glow_object_definitions[ i ];
		auto entity = reinterpret_cast< C_CSPlayer* >( glow_object.m_ent );

		if( !entity )
			continue;

		const auto client_class = entity->GetClientClass( );
		if( !client_class )
			continue;

		switch( client_class->m_ClassID ) {
			case CCSPlayer: {
				if( !entity->is_valid_player( false, true ) )
					continue;

				if( g_vars.visuals.filter == 1 && entity->team( ) == local->team( ) )
					continue;

				const float flash_alpha = entity->get_flashed( );

				// if player is flashed, display their flashed opacity instead of default glow color.
				flash_alpha > 50.f ? 
					glow_object.set( 1.f, 1.f, 1.f, flash_alpha / 255.f, 0 ) : 
					glow_object.set( g_vars.visuals.glow_color[ 1 ] / 255.f, g_vars.visuals.glow_color[ 2 ] / 255.f, g_vars.visuals.glow_color[ 3 ] / 255.f, 0.65f, 0 );

				break;
			}
				// vc++ doesn't support ranged switch cases...
				// todo: clean this up and glow weapons more efficiently.
			case CAK47: {
				glow_object.set( 1.f, 1.f, 1.f, 0.65f, 0 );
				break;
			}
			default:
				break;
		}
	}
}

void c_visuals::draw_healthbar( C_CSPlayer *entity, float x, float y, float w, float h ) {
	int hp = std::clamp( entity->health( ), 0, 100 );
	const OSHColor health_color = OSHColor::FromARGB( 220, 255 - hp * 2.55, hp * 2.55, 0 );

	const int height = hp * static_cast< int >( h ) / 100;

	g_renderer.filled_rect( OSHColor::FromARGB( 220, 10, 10, 10 ), x - 6, y - 1, 4, h + 2 );
	g_renderer.filled_rect_gradient( OSHGui::Drawing::ColorRectangle( health_color, health_color - OSHColor::FromARGB( 0, 50, 50, 0 ) ), x - 5, y + h - height, 2, height );

	if( hp >= 90 || hp <= 10 )
		return;

	g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
	                      OSHColor::FromARGB( 220, 255, 255, 255 ),
	                      OSHColor::FromARGB( 220, 10, 10, 10 ),
	                      x - 8, y + h - height - 6, OUTLINED, "%i", hp );
}

void c_visuals::draw_box( OSHColor color, int index, float x, float y, float w, float h ) {
	g_renderer.rect( OSHColor::FromARGB( 220, 10, 10, 10 ), x - 1, y - 1, w + 2, h + 2 );
	g_renderer.rect( OSHColor::FromARGB( 220, 10, 10, 10 ), x + 1, y + 1, w - 2, h - 2 );
	g_renderer.rect( color, x, y, w, h );
}

void c_visuals::weapon_name( C_BaseCombatWeapon *weapon, C_CSPlayer *player, OSHColor color, float x, float y, float w, float h ) {
	auto info = weapon->get_weapon_info( );
	if( !info )
		return;

	const char *name = info->hud_name;
	wchar_t *localised_name = g_csgo.m_localize->find( name );

	std::string print = util::misc::unicode_to_ascii( std::wstring( localised_name ) );
	std::transform( print.begin( ), print.end( ), print.begin( ), std::toupper );

	g_renderer.ansi_text( g_renderer.get_font( FONT_04B03_6PX ),
	                      color,
	                      OSHColor::FromARGB( 220, 10, 10, 10 ),
	                      x + w * 0.5f,
	                      y + h + 2 + ctx.offset,
	                      CENTERED_X | OUTLINED,
	                      print );
}

void c_visuals::ammo_bar( C_BaseCombatWeapon *weapon, C_CSPlayer *player, OSHColor color, float x, float y, float w, float h, int clip ) {
	auto info = weapon->get_weapon_info( );
	if( !info )
		return;

	if( info->type == WEAPONTYPE_KNIFE )
		return;

	//	setup for drawing
	const auto ammo = clip;
	const auto max_ammo = info->max_clip;
	auto width = w;

	//	get animation layer
	auto layer = player->animoverlays( )[ 1 ];
	const auto activity = player->activity( layer.m_nSequence );

	//	calculate width
	if( activity == 967 && layer.m_flWeight != 0.0f ) {
		width *= layer.m_flCycle;
	}
	else {
		width *= ammo;
		width /= max_ammo;
	}

	// background
	g_renderer.filled_rect( OSHColor::FromARGB( 220, 10, 10, 10 ), x - 1, y + h + 2 + ctx.offset, w + 2, 4 );

	// ammo bar
	g_renderer.filled_rect_gradient( OSHGui::Drawing::ColorRectangle( OSHColor::FromARGB( 220, 10, 10, 10 ),
	                                                                  color,
	                                                                  color,
	                                                                  color ),
																	  x, y + h + 3 + ctx.offset, width, 2 );

	if( ammo < 5 && ammo != 0 )
		g_renderer.ansi_text( g_renderer.get_font( FONT_04B03_6PX ),
		                      OSHColor::FromARGB( 220, 255, 255, 255 ),
		                      OSHColor::FromARGB( 220, 10, 10, 10 ),
		                      x + width,
		                      y + h,
		                      OUTLINED,
		                      "%i", ammo );

	ctx.offset += 5;
}

void c_visuals::flash_bar( C_CSPlayer *player, OSHColor color, float x, float y, float w, float h ) {
	const auto max_alpha = 255.f;

	float flash_alpha = player->get_flashed( );
	if( flash_alpha < 1.f )
		return;

	float width = w;

	//	calculate width
	width *= ( flash_alpha / 255.f );

	// background
	g_renderer.filled_rect( OSHColor::FromARGB( 220, 10, 10, 10 ), x - 1, y + h + 2, w + 2, 4 );

	// flash bar
	g_renderer.filled_rect( color, x, y + h + 3, width, 2 );

	ctx.offset += 5;
}

void c_visuals::draw_flags( C_CSPlayer *player, OSHColor color, float x, float y, float w, float h ) {
	std::string armor_flag;
	if( player->helmet( ) )
		armor_flag += "H";

	if( player->armor( ) > 0 )
		armor_flag += "K";

	if( !armor_flag.empty( ) )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
		                      OSHColor::FromARGB( 220, 10, 10, 10 ),
		                      x + w + 3, y + ctx.flag_count++ * 8, OUTLINED, armor_flag );

	if( player->get_flashed( ) > 140.f )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
		                      OSHColor::FromARGB( 220, 10, 10, 10 ),
		                      x + w + 3, y + ctx.flag_count++ * 8, OUTLINED, "F" );

	if( player->is_scoped( ) )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
		                      OSHColor::FromARGB( 220, 10, 10, 10 ),
		                      x + w + 3, y + ctx.flag_count++ * 8, OUTLINED, "+" );
}

void c_visuals::draw_spectators( ) {
	int spec_count = 0;

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; i++ ) {
		auto *entity = g_csgo.m_entity_list->Get< C_CSPlayer >( i );
		auto *local = g_csgo.m_entity_list->Get< C_CSPlayer >( g_csgo.m_engine->GetLocalPlayer( ) );
		if( !entity || entity->alive( ) || entity == local )
			continue;

		auto *spectator = g_csgo.m_entity_list->Get< C_BaseEntity >( entity->observer_handle( ) );
		if( !spectator )
			continue;

		player_info_s player_info_spec{};
		if( !g_csgo.m_engine->GetPlayerInfo( i, &player_info_spec ) )
			continue;

		if( player_info_spec.m_szPlayerName == "GOTV" )
			continue;

		if( spectator != local )
			continue;

		OSHGui::Drawing::SizeF display_size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_LUCIDA_CONSOLE ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ),
		                      display_size.Width - 150, display_size.Height / 2 + 10 * spec_count - 300, DROPSHADOW, "%s", player_info_spec.m_szPlayerName );

		spec_count++;
	}
}

void c_visuals::world( C_BaseEntity *entity ) {
	auto local = C_CSPlayer::get_local( );
	if( !entity->is_valid_world( true ) || !local )
		return;

	ClientClass *client_class = entity->GetClientClass( );
	if( !client_class )
		return;

	if( g_vars.visuals.objectives ) {
		if( client_class->m_ClassID == CPlantedC4 && entity->bomb_ticking( ) && !entity->bomb_defused( ) ) {
			float m_defuse_time = entity->get_defuse_length( );

			float bomb_duration = entity->blow_time( ) - g_csgo.m_global_vars->m_cur_time;

			if( bomb_duration > 0.f )
				draw_bomb_timer( bomb_duration );
		}
	}

	bbox_t box{};
	if( !calculate_bbox( entity, box ) )
		return;

	auto owner = static_cast< C_BaseEntity * >( g_csgo.m_entity_list->GetClientEntityFromHandle( entity->owner( ) ) );

	if( !owner && ( client_class->m_ClassID != CBaseWeaponWorldModel && ( std::strstr( client_class->m_pNetworkName, "Weapon" )
		|| client_class->m_ClassID == CDEagle || client_class->m_ClassID == CAK47 ) ) ) {

		auto weapon = static_cast< C_BaseCombatWeapon * >( entity );
		if( !weapon )
			return;

		if( !weapon->item_index( ) )
			return;

		const auto info = g_csgo.m_weapon_system->GetWpnData( weapon->item_index( ) );
		if( !info )
			return;

		const auto wpn_index = weapon->item_index( );

		if( info->type == WEAPONTYPE_KNIFE
			|| info->type == WEAPONTYPE_GRENADE
			|| wpn_index == WEAPON_FISTS
			|| wpn_index == WEAPON_SPANNER
			|| wpn_index == WEAPON_HAMMER
			|| wpn_index == WEAPON_AXE
			|| wpn_index == WEAPON_MELEE
			|| wpn_index == WEAPON_BREACHCHARGE
			|| wpn_index == WEAPON_TABLET )
			return;

		const char *name = info->hud_name;
		wchar_t *localised_name = g_csgo.m_localize->find( name );

		auto weapon_name = util::misc::unicode_to_ascii( std::wstring( localised_name ) );
		std::transform( weapon_name.begin( ), weapon_name.end( ), weapon_name.begin( ), std::toupper );
		if( g_vars.visuals.dropped_weapons )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      OSHColor::FromARGB( 200, 255, 255, 255 ),
			                      OSHColor::FromARGB( 130, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y + box.h, CENTERED_X | OUTLINED, weapon_name );

		if( g_vars.visuals.dropped_ammo ) {
			const auto ammo = weapon->clip( );
			const auto max_ammo = info->max_clip;
			auto width = box.w;

			width *= ammo;
			width /= max_ammo;

			// background
			g_renderer.filled_rect( OSHColor::FromARGB( 220, 10, 10, 10 ), box.x - 1, box.y + box.h + 9, box.w + 2, 4 );

			// ammo bar
			g_renderer.filled_rect( OSHColor::FromARGB( g_vars.visuals.dropped_ammo_color ),
			                        box.x, box.y + box.h + 10, width, 2 );

			if( ammo < 10 )
				g_renderer.ansi_text( g_renderer.get_font( FONT_04B03_6PX ),
				                      OSHColor::FromARGB( 220, 255, 255, 255 ),
				                      OSHColor::FromARGB( 220, 10, 10, 10 ),
				                      box.x + width,
				                      box.y + box.h,
				                      OUTLINED,
				                      "%i", ammo );
		}

	}

	if( g_vars.visuals.objectives ) {
		if( client_class->m_ClassID == CPlantedC4 && entity->bomb_ticking( ) && !entity->bomb_defused( ) ) {
			std::string text;
			if( local->team( ) == 3 )
				text += "DEFUSE ";
			else if( local->team( ) == 2 )
				text += "DEFEND ";
			text += "BOMB";

			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      OSHColor::FromARGB( 240, 79, 130, 180 ),
			                      OSHColor::FromARGB( 200, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y - 15.f, CENTERED_X | OUTLINED, text );

			const float bomb_duration = entity->blow_time( ) - g_csgo.m_global_vars->m_cur_time;
			if( bomb_duration > 0.f ) {
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
				                      OSHColor::FromARGB( 240, 255, 255, 255 ),
				                      OSHColor::FromARGB( 200, 10, 10, 10 ),
				                      box.x + box.w * 0.5f, box.y - 5.f, CENTERED_X | OUTLINED, "%2.1f", bomb_duration );
			}
		}

		if( !owner && client_class->m_ClassID == CC4 ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      OSHColor::Lime( ),
			                      OSHColor::FromARGB( 200, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y - 5.f, CENTERED_X | OUTLINED, "BOMB" );
		}

		if( owner && client_class->m_ClassID == CHostageCarriableProp ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      OSHColor::FromARGB( 240, 0, 0, 220 ),
			                      OSHColor::FromARGB( 200, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y - 15.f, CENTERED_X | OUTLINED, "HOSTAGE" );
		}
	}

	if( g_vars.visuals.chicken && !owner && client_class->m_ClassID == CChicken ) {
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
		                      OSHColor::FromARGB( 200, 255, 255, 100 ),
		                      OSHColor::FromARGB( 200, 10, 10, 10 ),
		                      box.x + box.w * 0.5f, box.y - 5.f, CENTERED_X | OUTLINED, "CHICKEN" );
	}

	std::string model_name = g_csgo.m_model_info->GetModelName( entity->GetModel( ) );
	if( g_vars.visuals.projectiles ) {
		if( !model_name.empty( ) ) {
			if( client_class->m_ClassID == CBaseCSGrenadeProjectile ) {
				if( model_name.find( "fraggrenade" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "HEGRENADE" );
				else if( model_name.find( "flashbang" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "FLASHBANG" );
			}

			if( client_class->m_ClassID == CMolotovProjectile ) {
				if( model_name.find( "molotov" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "MOLOTOV" );
				else if( model_name.find( "incendiary" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "INCENDIARY" );
			}

			if( client_class->m_ClassID == CSmokeGrenadeProjectile )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "SMOKE" );
		}
	}

	if( g_vars.misc.dangerzone_menu ) {
		if( entity->origin( ).DistTo( local->origin( ) ) > g_vars.dz.max_item_distance )
			return;

		if( client_class->m_ClassID == CItemCash && g_vars.dz.cash )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 149, 184, 6 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Cash" );

		if( client_class->m_ClassID == CItem_Healthshot && !owner && g_vars.dz.healthshot )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 211, 27, 39 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Healthshot" );

		if( client_class->m_ClassID == CDrone && g_vars.dz.drone ) {
			if( !owner )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Drone" );

			const auto drone_owner = static_cast< C_CSPlayer * >( owner );
			if( drone_owner )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Drone - %s", drone_owner->get_info( ).m_szPlayerName );
		}

		if( client_class->m_ClassID == CDronegun && g_vars.dz.turret ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y - 12, CENTERED_X | DROPSHADOW, "Turret" );

			g_renderer.rect( OSHColor::FromARGB( 200, 10, 10, 10 ), box.x - 1, box.y - 1, box.w + 2, box.h + 2 );
			g_renderer.rect( OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + 1, box.y + 1, box.w - 2, box.h - 2 );
			g_renderer.rect( OSHColor::FromARGB( 200, 255, 255, 255 ), box.x, box.y, box.w, box.h );

			if( entity->dronegun_has_target( ) )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | OUTLINED, "TARGETING" );

			/*int hp = std::clamp( entity->dronegun_health(), 0, 220 );
			OSHColor health_color = OSHColor::FromARGB( 255, 255 - hp * 2.55, hp * 2.55, 0 );

			int height = hp * ( int )box.h / 220;

			g_renderer.filled_rect( OSHColor::FromARGB( 200, 10, 10, 10 ), box.x - 6, box.y - 1, 4, box.h + 2 );
			g_renderer.filled_rect_gradient( OSHGui::Drawing::ColorRectangle( health_color, health_color - OSHColor::FromARGB( 0, 50, 50, 0 ) ), box.x - 5, box.y + box.h - height, 2, height );

			if( hp >= 90 || hp <= 10 )
				return;

			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
				OSHColor::FromARGB( 200, 255, 255, 255 ),
				OSHColor::FromARGB( 200, 10, 10, 10 ),
				box.x - 8, box.y + box.h - height - 6, OUTLINED, "%i", hp );*/
		}

		if( client_class->m_ClassID == CBRC4Target && g_vars.dz.safe )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 149, 184, 6 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Safe" );

		if( client_class->m_ClassID == CPhysPropAmmoBox || client_class->m_ClassID == CPhysPropLootCrate
			|| client_class->m_ClassID == CPhysPropRadarJammer || client_class->m_ClassID == CPhysPropWeaponUpgrade ) {
			if( !g_vars.dz.ammobox || !g_vars.dz.weapon_case )
				return;

			// epic.
			if( !model_name.empty( ) ) {
				if( g_vars.dz.weapon_case && model_name.find( "case_pistol" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Pistol case" );
				else if( g_vars.dz.weapon_case && model_name.find( "case_light_weapon" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Light case" );
				else if( g_vars.dz.weapon_case && model_name.find( "case_heavy_weapon" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Heavy case" );
				else if( g_vars.dz.weapon_case && model_name.find( "case_explosive" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Explosives case" );
				else if( g_vars.dz.tool_case && model_name.find( "case_tools" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Tools case" );
				else if( g_vars.dz.airdrop && model_name.find( "random" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Airdrop" );
				else if( g_vars.dz.armor && model_name.find( "dz_armor_helmet" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Full armor" );
				else if( g_vars.dz.armor && model_name.find( "dz_helmet" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Helmet" );
				else if( g_vars.dz.armor && model_name.find( "dz_armor" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Body armor" );
				else if( g_vars.dz.tablet_upgrade && model_name.find( "upgrade_tablet" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Tablet upgrade" );
				else if( g_vars.dz.contract && model_name.find( "briefcase" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 149, 184, 6 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Contract" );
				else if( g_vars.dz.parachute && model_name.find( "parachutepack" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Parachute" );
				else if( g_vars.dz.cash && model_name.find( "dufflebag" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 149, 184, 6 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Cash dufflebag" );
				else if( g_vars.dz.ammobox && model_name.find( "ammobox" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Ammobox" );
				else if( g_vars.dz.jammer && model_name.find( "jammer" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Radar jammer" );
			}
		}

		if( client_class->m_ClassID == CBreachCharge && g_vars.dz.breach_charge )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Breach charge" );

		if( client_class->m_ClassID == CMelee && !owner && g_vars.dz.melee_weapon )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), OSHColor::FromARGB( 200, 255, 255, 255 ), OSHColor::FromARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Melee weapon" );
	}
}

void c_visuals::draw_bomb_timer( float time_left ) const {
	const auto size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

	// todo: only grab this once on map load.
	ConVar *max_c4_timer = g_csgo.m_convar->FindVar( "mp_c4timer" );
	float x = time_left * size.Width / max_c4_timer->GetFloat( );

	g_renderer.filled_rect( OSHColor::FromARGB( 145, 24, 24, 32 ), 0, 0, size.Width, 3 );
	g_renderer.filled_rect( OSHColor::FromARGB( 255, 113, 59, 198 ), x, 0, time_left * -size.Width / max_c4_timer->GetFloat( ), 3 );
	g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), OSHColor::White( ), OSHColor::Black( ), x - 1, -1, DROPSHADOW, "%2.1f", time_left );
}

void c_visuals::watermark( ) const {
	const bool connected = g_csgo.m_engine->IsConnected( );

	const auto size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

	g_renderer.filled_rect_gradient(
		OSHGui::Drawing::ColorRectangle( OSHColor::FromARGB( 225, 24, 24, 32 ), OSHColor::FromARGB( 10, 24, 24, 32 ),
		                                 OSHColor::FromARGB( 225, 24, 24, 32 ), OSHColor::FromARGB( 10, 24, 24, 32 ) ),
		size.Width - 270, size.Height - size.Height + 20, 250, 15 );

	time_t t = std::time( nullptr );
	tm tm = *std::localtime( &t );

	std::ostringstream oss;
	oss << std::put_time( &tm, "%I:%M%p" );
	auto time = oss.str( );

	if( connected )
		g_renderer.ansi_text( g_renderer.m_fonts[ FONT_VERDANA_7PX ], OSHColor::FromARGB( 220, 255, 255, 255 ),
		                      OSHColor( 0.f, 0.f, 0.f, 0.f ), size.Width - 268, size.Height - size.Height + 22, 0,
		                      "deadcell | %s | out %ims | choked %i", time.c_str( ), g_cl.m_ping, g_csgo.m_clientstate->m_nChokedCommands );
	else
		g_renderer.ansi_text( g_renderer.m_fonts[ FONT_VERDANA_7PX ], OSHColor( 0.8f, 1.f, 1.f, 1.f ),
		                      OSHColor( 0.f, 0.f, 0.f, 0.f ), size.Width - 268, size.Height - size.Height + 22, 0,
		                      "deadcell | unconnected | %s", time.c_str( ) );
}

void c_visuals::draw_scope( ) const {
	auto local = C_CSPlayer::get_local( );
	if( !local || !local->is_scoped( ) || !local->get_active_weapon( ) || !local->get_active_weapon( )->has_sniper_scope( ) )
		return;

	const auto size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

	g_renderer.filled_rect( OSHColor::FromARGB( g_vars.visuals.misc.scope_color ), 0, size.Height * 0.5f, size.Width, 1 );
	g_renderer.filled_rect( OSHColor::FromARGB( g_vars.visuals.misc.scope_color ), size.Width * 0.5f, 0, 1, size.Height );
}

void c_visuals::draw_crosshair( ) const {
	auto local = C_CSPlayer::get_local( );
	if( !local )
		return;

	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;

	const auto size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

	const float spread_distance = ( weapon->inaccuracy( ) + weapon->spread( ) ) * 320.f / std::tan( math::deg_to_rad( g_vars.visuals.effects.camera_fov ) * 0.5f );
	const float spread_radius = size.Height * 2 * 0.002083 * spread_distance;

	g_renderer.circle( OSHColor::FromARGB( g_vars.visuals.spread_circle_color ), size.Width / 2, size.Height / 2, spread_radius );
}

void c_visuals::draw_hitmarker( ) {
	auto size = g_renderer.m_instance->GetRenderer( ).GetDisplaySize( );

	if( g_cl.m_hitmarker_alpha > 0.f ) {
		g_renderer.line( OSHColor( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), size.Width / 2.f - 8.f, size.Height / 2.f - 8.f, size.Width / 2.f - 3.f, size.Height / 2.f - 3.f );
		g_renderer.line( OSHColor( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), size.Width / 2.f - 8.f, size.Height / 2.f + 8.f, size.Width / 2.f - 3.f, size.Height / 2.f + 3.f );
		g_renderer.line( OSHColor( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), size.Width / 2.f + 8.f, size.Height / 2.f - 8.f, size.Width / 2.f + 3.f, size.Height / 2.f - 3.f );
		g_renderer.line( OSHColor( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), size.Width / 2.f + 8.f, size.Height / 2.f + 8.f, size.Width / 2.f + 3.f, size.Height / 2.f + 3.f );

		g_cl.m_hitmarker_alpha -= 1.f / 0.5f * g_csgo.m_global_vars->m_frametime;
	}
}

