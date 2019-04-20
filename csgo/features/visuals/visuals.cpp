#include "visuals.h"
#include <mutex>

#include "../autowall/autowall.h"
#include "../misc/misc.h"
#include "../ragebot/ragebot.h"

c_visuals g_visuals;

void c_visuals::run( ) {
	if( g_vars.visuals.misc.remove_scope && g_vars.visuals.misc.remove_scope < 3 )
		draw_scope( );

	activation_type( );

	std::array<int, 4> m_placed;
	for ( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; i++ )
	{
		auto& data = m_vis_data.at( i );
		if ( !data.m_valid )
			continue;

		for ( auto placed : m_placed )
			placed = 0;

		while ( data.m_texts.size() )
		{
			text_t text;
			if ( !data.m_texts.wait_for( text, std::chrono::milliseconds( 5 ) ) )
				continue;

			switch ( text.m_pos )
			{
				case e_vis_pos::POS_TOP:
				{
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ),
										  text.m_col,
										  ImColorARGB( 220, 10, 10, 10 ),
										  data.m_bbox.x + data.m_bbox.w * 0.5, data.m_bbox.y - 12 - 8 * m_placed[ e_vis_pos::POS_TOP ]++, CENTERED_X | DROPSHADOW, text.m_text );
				}
			}
		}

		for ( auto placed : m_placed )
			placed = 0;

		/// do bars here

	}

	/*for( int i = 1; i <= g_csgo.m_entity_list->get_highest_entity_index( ); i++ ) {
		auto entity = g_csgo.m_entity_list->get< c_csplayer >( i );
		if( !entity )
			continue;

		if( g_vars.visuals.radar )
			entity->spotted( ) = true;

		player( entity );
		world( entity );
		g_misc.nightmode( entity );
	}*/

	for ( auto& data : m_vis_data )
		data.clear();

	if( g_vars.visuals.extra.speclist )
		draw_spectators( );

	if( g_vars.visuals.visualize_spread )
		draw_crosshair( );

	if( g_vars.visuals.hitmarker )
		draw_hitmarker( );
}

bool c_visuals::world_to_screen( const vec3_t &origin, vec3_t &screen ) {
	// going to use the outdated matrix for now...
	const D3DMATRIX &matrix = g_csgo.m_engine->world_to_screen_matrix_d3d( );

	const float width = matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ];

	if( width > 0.01f ) {
		const float inverse = 1.f / width;

		const ImVec2 display_size = ImGui::GetIO( ).DisplaySize;

		screen.x = static_cast< float >( display_size.x / 2 + ( 0.5 * ( ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) * inverse ) * display_size.x + 0.5 ) );
		screen.y = static_cast< float >( display_size.y / 2 - ( 0.5 * ( ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) * inverse ) * display_size.y + 0.5 ) );
		screen.z = 0.0f;

		return true;
	}

	return false;
}

void c_visuals::queue()
{
	if ( !g_csgo.m_engine->is_in_game() || !g_csgo.m_engine->is_connected() )
		return;

	for ( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; i++ )
	{
		auto entity = g_csgo.m_entity_list->get< c_csplayer >( i );
		if ( !entity )
			continue;

		if ( !calculate_bbox( entity, m_vis_data[ i ].m_bbox ) )
			m_vis_data[ i ].m_valid = false;

		m_vis_data[ i ].m_texts.post( text_t( entity->get_info().m_player_name, ImColorARGB( 255, 255, 255, 255 ) ) );

		m_vis_data[ i ].m_valid = true;
	}
}

bool c_visuals::calculate_bbox( c_base_entity *entity, bbox_t &box ) const {
	matrix3x4_t &tran_frame = entity->coord_frame( );

	const vec3_t min = entity->mins( );
	const vec3_t max = entity->maxs( );

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
	box.w = static_cast< int >( right ) - static_cast< int >( left );
	box.h = static_cast< int >( bottom ) - static_cast< int >( top );

	return true;
}

void c_visuals::activation_type( ) {
	static bool is_down = false;
	static bool is_clicked = false;

	// hotkey stuff.
	if( g_vars.visuals.activation_type == 0 /* always */ ) {
		m_ctx.m_enabled = true;
	}
	else if( g_vars.visuals.activation_type == 1 /* on-key */ ) {
		if( g_input.key_pressed( g_vars.visuals.activation_key ) )
			m_ctx.m_enabled = true;
		else
			m_ctx.m_enabled = false;
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
			m_ctx.m_enabled = !m_ctx.m_enabled;
	}
}

void c_visuals::run_dormancy_fade( c_csplayer *entity, const int idx ) {
	const float step = 255.f * ( 20.f / 15.f ) * g_csgo.m_global_vars->m_frametime;

	g_vars.visuals.dormancy_fade ? entity->is_dormant( ) 
		? m_alpha.at( idx ) -= step : m_alpha.at( idx ) += step * 5.f : entity->is_dormant( )
		? m_alpha.at( idx ) = 0.f : m_alpha.at( idx ) = 220.f;

	if( m_alpha.at( idx ) > 220.f )
		m_alpha.at( idx ) = 220.f;
	if( m_alpha.at( idx ) < 0.f )
		m_alpha.at( idx ) = 0.f;
}

void c_visuals::player( c_csplayer *entity ) {
	const auto index = entity->get_index( );

	auto local = c_csplayer::get_local( );
	if( !entity->is_valid_player( false, true ) || !local || entity == local )
		return;

	if( g_vars.visuals.filter == 1 && entity->team( ) == local->team( ) )
		return;

	run_dormancy_fade( entity, index );	

	if( !m_ctx.m_enabled )
		return;

	if( !local->alive( ) ) {
		const auto spec_handle = local->observer_handle( );
		const auto spec = g_csgo.m_entity_list->get< c_base_entity >( spec_handle );
		if( spec == entity )
			return;
	}

	bbox_t box{};
	if( !calculate_bbox( entity, box ) )
		return;

	c_base_combat_weapon *weapon = entity->get_active_weapon( );
	if( !weapon )
		return;

	weapon_info_t *info = g_csgo.m_weapon_system->get_weapon_data( weapon->item_index( ) );
	if( !info )
		return;

	auto name_color     = ImColorARGB( g_vars.visuals.name_color, m_alpha.at( index ) );
	auto box_color      = ImColorARGB( g_vars.visuals.box_color, m_alpha.at( index ) );
	auto ammo_bar_color = ImColorARGB( g_vars.visuals.ammo_bar_color, m_alpha.at( index ) );
	auto weapon_color   = ImColorARGB( g_vars.visuals.weapon_color, m_alpha.at( index ) );
	auto flag_color     = ImColorARGB( g_vars.visuals.flag_color, m_alpha.at( index ) );
	auto money_color    = ImColorARGB( g_vars.visuals.money_color, m_alpha.at( index ) );
	auto flashed_color  = ImColorARGB( g_vars.visuals.flash_bar_color, m_alpha.at( index ) );

	if( g_vars.visuals.bbox )
		draw_box( box_color, index, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.name ) {
		std::string name = entity->get_info( ).m_player_name;

		if( name.length( ) > 17 )
			name = name.substr( 0, 17 ) + "...";

		//std::transform( name.begin( ), name.end( ), name.begin( ), tolower );
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ),
		                      name_color,
		                      ImColorARGB( 220, 10, 10, 10 ),
		                      box.x + box.w * 0.5, box.y - 12, CENTERED_X | DROPSHADOW,
		                      entity->get_info( ).xuid_low == 0 ? name.append( " ( BOT ) " ) : name );
	}

	if( g_vars.visuals.healthbar )
		draw_healthbar( entity, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.flash_bar )
		flash_bar( entity, flashed_color, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.ammo_bar )
		ammo_bar( weapon, entity, ammo_bar_color, box.x, box.y, box.w, box.h, weapon->clip( ) );

	if( g_vars.visuals.weapon )
		weapon_name( weapon, entity, weapon_color, box.x, box.y, box.w, box.h );

	if( g_vars.visuals.money )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
		                      money_color,
		                      ImColorARGB( 220, 10, 10, 10 ),
		                      box.x + box.w + 3, box.y + m_ctx.flag_count++ * 8, OUTLINED, "$%i", entity->money( ) );

	if( g_vars.visuals.flags )
		draw_flags( entity, flag_color, box.x, box.y, box.w, box.h );

	/// even if no flags are drawn the flag counter ( used above in money ) and
	/// the offset ( used for e.g. weapon/ammo ) has to be reset
	m_ctx.flag_count = 0;
	m_ctx.offset = 0;
}

void c_visuals::handle_glow( ) {
	if( !m_ctx.m_enabled )
		return;

	auto local = c_csplayer::get_local( );
	if( !local )
		return;

	for( int i = 0; i < g_csgo.m_glow_obj_manager->m_size; i++ ) {
		if( g_csgo.m_glow_obj_manager->m_glow_object_definitions[ i ].is_unused( ) || !g_csgo.m_glow_obj_manager->m_glow_object_definitions[ i ].get_entity( ) )
			continue;

		auto &glow_object = g_csgo.m_glow_obj_manager->m_glow_object_definitions[ i ];

		auto entity = reinterpret_cast< c_csplayer* >( glow_object.m_ent );
		if( !entity )
			continue;

		const auto client_class = entity->get_client_class( );
		if( !client_class )
			continue;

		switch( client_class->m_class_id ) {
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

void c_visuals::draw_healthbar( c_csplayer *entity, float x, float y, float w, float h ) {
	int hp = std::clamp( entity->health( ), 0, 100 );
	const ImColorARGB health_color = ImColorARGB( 220, 255 - hp * 2.55, hp * 2.55, 0 );

	const int height = hp * static_cast< int >( h ) / 100;

	g_renderer.filled_rect( ImColorARGB( 220, 10, 10, 10 ), x - 6, y - 1, 4, h + 2 );
	g_renderer.filled_rect( health_color, x - 5, y + h - height, 2, height ); // removed gradient (temp)

	if( hp >= 90 || hp <= 10 )
		return;

	g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
	                      ImColorARGB( 220, 255, 255, 255 ),
	                      ImColorARGB( 220, 10, 10, 10 ),
	                      x - 8, y + h - height - 6, OUTLINED, "%i", hp );
}

void c_visuals::draw_box( const ImU32 &color, int index, float x, float y, float w, float h ) {
	g_renderer.rect( ImColorARGB( 220, 10, 10, 10 ), x - 1, y - 1, w + 2, h + 2 );
	g_renderer.rect( ImColorARGB( 220, 10, 10, 10 ), x + 1, y + 1, w - 2, h - 2 );
	g_renderer.rect( color, x, y, w, h );
}

void c_visuals::weapon_name( c_base_combat_weapon *weapon, c_csplayer *player, const ImU32 &color, float x, float y, float w, float h ) {
	auto info = weapon->get_weapon_info( );
	if( !info )
		return;

	const char *name = info->hud_name;
	std::wstring localised_name = g_csgo.m_localize->find( name );

	std::string print = util::misc::unicode_to_ascii( localised_name );
	std::transform( print.begin( ), print.end( ), print.begin( ), toupper );

	g_renderer.ansi_text( g_renderer.get_font( FONT_04B03_6PX ),
	                      color,
	                      ImColorARGB( 220, 10, 10, 10 ),
	                      x + w * 0.5f,
	                      y + h + 2 + m_ctx.offset,
	                      CENTERED_X | OUTLINED,
	                      print );
}

void c_visuals::ammo_bar( c_base_combat_weapon *weapon, c_csplayer *player, const ImU32 &color, float x, float y, float w, float h, int clip ) {
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
	g_renderer.filled_rect( ImColorARGB( 220, 10, 10, 10 ), x - 1, y + h + 2 + m_ctx.offset, w + 2, 4 );

	// ammo bar
	g_renderer.filled_rect( color, x, y + h + 3 + m_ctx.offset, width, 2 ); // removed gradient (temp)

	if( ammo < 5 && ammo != 0 )
		g_renderer.ansi_text( g_renderer.get_font( FONT_04B03_6PX ),
		                      ImColorARGB( 220, 255, 255, 255 ),
		                      ImColorARGB( 220, 10, 10, 10 ),
		                      x + width,
		                      y + h,
		                      OUTLINED,
		                      "%i", ammo );

	m_ctx.offset += 5;
}

void c_visuals::flash_bar( c_csplayer *player, const ImU32 &color, float x, float y, float w, float h ) {
	const auto max_alpha = 255.f;

	float flash_alpha = player->get_flashed( );
	if( flash_alpha < 1.f )
		return;

	float width = w;

	//	calculate width
	width *= ( flash_alpha / 255.f );

	// background
	g_renderer.filled_rect( ImColorARGB( 220, 10, 10, 10 ), x - 1, y + h + 2, w + 2, 4 );

	// flash bar
	g_renderer.filled_rect( color, x, y + h + 3, width, 2 );

	m_ctx.offset += 5;
}

void c_visuals::draw_flags( c_csplayer *player, const ImU32 &color, float x, float y, float w, float h ) {
	m_ctx.offset = 0;
	m_ctx.flag_count = 0;

	std::string armor_flag;
	if( player->helmet( ) )
		armor_flag += "H";

	if( player->armor( ) > 0 )
		armor_flag += "K";

	if( !armor_flag.empty( ) )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
		                      ImColorARGB( 220, 10, 10, 10 ),
		                      x + w + 3, y + m_ctx.flag_count++ * 8, OUTLINED, armor_flag );

	if( player->get_flashed( ) > 140.f )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
		                      ImColorARGB( 220, 10, 10, 10 ),
		                      x + w + 3, y + m_ctx.flag_count++ * 8, OUTLINED, "F" );

	if( player->is_scoped( ) )
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
		                      ImColorARGB( 220, 10, 10, 10 ),
		                      x + w + 3, y + m_ctx.flag_count++ * 8, OUTLINED, "+" );

	if( g_vars.rage.enabled ) {
		if( g_ragebot.m_last_target == player ){
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), color,
							  ImColorARGB( 220, 10, 10, 10 ),
							  x + w + 3, y + m_ctx.flag_count++ * 8, OUTLINED, "TARGET" );
		}
	}
}

void c_visuals::draw_spectators( ) const {
	int spec_count = 0;

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; i++ ) {
		auto entity = g_csgo.m_entity_list->get< c_csplayer >( i );
		auto local = g_csgo.m_entity_list->get< c_csplayer >( g_csgo.m_engine->get_local_player( ) );
		if( !entity || entity->alive( ) || entity == local )
			continue;

		const auto spectator = g_csgo.m_entity_list->get< c_base_entity >( entity->observer_handle( ) );
		if( !spectator )
			continue;

		player_info_s player_info_spec{};
		if( !g_csgo.m_engine->get_player_info( i, &player_info_spec ) )
			continue;

		if( player_info_spec.m_player_name == "GOTV" )
			continue;

		if( spectator != local )
			continue;

		const ImVec2 display_size = ImGui::GetIO( ).DisplaySize;
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_LUCIDA_CONSOLE_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ),
		                      display_size.x - 150, display_size.y / 2 + 10 * spec_count - 300, DROPSHADOW, "%s", player_info_spec.m_player_name );

		spec_count++;
	}
}

void c_visuals::world( c_base_entity *entity ) {
	auto local = c_csplayer::get_local( );
	if( !entity->is_valid_world( true ) || !local )
		return;

	client_class *client_class = entity->get_client_class( );
	if( !client_class )
		return;

	if( g_vars.visuals.objectives ) {
		if( client_class->m_class_id == CPlantedC4 && entity->bomb_ticking( ) && !entity->bomb_defused( ) ) {
			float m_defuse_time = entity->get_defuse_length( );

			const float bomb_duration = entity->blow_time( ) - g_csgo.m_global_vars->m_cur_time;

			if( bomb_duration > 0.f )
				draw_bomb_timer( bomb_duration );
		}
	}

	bbox_t box{};
	if( !calculate_bbox( entity, box ) )
		return;

	const auto owner = g_csgo.m_entity_list->get< c_base_entity >( entity->owner( ) );

	if( !owner && ( client_class->m_class_id != CBaseWeaponWorldModel && ( std::strstr( client_class->m_network_name, "Weapon" )
		|| client_class->m_class_id == CDEagle || client_class->m_class_id == CAK47 ) ) ) {

		auto weapon = static_cast< c_base_combat_weapon * >( entity );
		if( !weapon )
			return;

		if( !weapon->item_index( ) )
			return;

		const weapon_info_t *info = g_csgo.m_weapon_system->get_weapon_data( weapon->item_index( ) );
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
		std::transform( weapon_name.begin( ), weapon_name.end( ), weapon_name.begin( ), toupper );
		if( g_vars.visuals.dropped_weapons )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      ImColorARGB( 200, 255, 255, 255 ),
			                      ImColorARGB( 130, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y + box.h, CENTERED_X | OUTLINED, weapon_name );

		if( g_vars.visuals.dropped_ammo ) {
			const auto ammo = weapon->clip( );
			const auto max_ammo = info->max_clip;
			auto width = box.w;

			width *= ammo;
			width /= max_ammo;

			// background
			g_renderer.filled_rect( ImColorARGB( 220, 10, 10, 10 ), box.x - 1, box.y + box.h + 9, box.w + 2, 4 );

			// ammo bar
			g_renderer.filled_rect( ImColorARGB( g_vars.visuals.dropped_ammo_color ),
			                        box.x, box.y + box.h + 10, width, 2 );

			if( ammo < 10 )
				g_renderer.ansi_text( g_renderer.get_font( FONT_04B03_6PX ),
				                      ImColorARGB( 220, 255, 255, 255 ),
				                      ImColorARGB( 220, 10, 10, 10 ),
				                      box.x + width,
				                      box.y + box.h,
				                      OUTLINED,
				                      "%i", ammo );
		}

	}

	if( g_vars.visuals.objectives ) {
		if( client_class->m_class_id == CPlantedC4 && entity->bomb_ticking( ) && !entity->bomb_defused( ) ) {
			std::string text;
			if( local->team( ) == 3 )
				text += "DEFUSE ";
			else if( local->team( ) == 2 )
				text += "DEFEND ";
			text += "BOMB";

			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      ImColorARGB( 240, 79, 130, 180 ),
			                      ImColorARGB( 200, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y - 15.f, CENTERED_X | OUTLINED, text );

			const float bomb_duration = entity->blow_time( ) - g_csgo.m_global_vars->m_cur_time;
			if( bomb_duration > 0.f ) {
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
				                      ImColorARGB( 240, 255, 255, 255 ),
				                      ImColorARGB( 200, 10, 10, 10 ),
				                      box.x + box.w * 0.5f, box.y - 5.f, CENTERED_X | OUTLINED, "%2.1f", bomb_duration );
			}
		}

		if( !owner && client_class->m_class_id == CC4 ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      ImColorARGB( 255, 0, 255, 0 ),
			                      ImColorARGB( 200, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y - 5.f, CENTERED_X | OUTLINED, "BOMB" );
		}

		if( owner && client_class->m_class_id == CHostageCarriableProp ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
			                      ImColorARGB( 240, 0, 0, 220 ),
			                      ImColorARGB( 200, 10, 10, 10 ),
			                      box.x + box.w * 0.5f, box.y - 15.f, CENTERED_X | OUTLINED, "HOSTAGE" );
		}
	}

	if( g_vars.visuals.chicken && !owner && client_class->m_class_id == CChicken ) {
		g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
		                      ImColorARGB( 200, 255, 255, 100 ),
		                      ImColorARGB( 200, 10, 10, 10 ),
		                      box.x + box.w * 0.5f, box.y - 5.f, CENTERED_X | OUTLINED, "CHICKEN" );
	}

	std::string model_name = g_csgo.m_model_info->get_model_name( entity->get_model( ) );
	if( g_vars.visuals.projectiles ) {
		if( !model_name.empty( ) ) {
			if( client_class->m_class_id == CBaseCSGrenadeProjectile ) {
				if( model_name.find( "fraggrenade" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "HEGRENADE" );
				else if( model_name.find( "flashbang" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "FLASHBANG" );
			}

			if( client_class->m_class_id == CMolotovProjectile ) {
				if( model_name.find( "molotov" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "MOLOTOV" );
				else if( model_name.find( "incendiary" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "INCENDIARY" );
			}

			if( client_class->m_class_id == CSmokeGrenadeProjectile )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | OUTLINED, "SMOKE" );
		}
	}

	if( g_vars.misc.dangerzone_menu ) {
		if( entity->origin( ).distance( local->origin( ) ) > g_vars.dz.max_item_distance )
			return;

		if( client_class->m_class_id == CItemCash && g_vars.dz.cash )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 149, 184, 6 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Cash" );

		if( client_class->m_class_id == CItem_Healthshot && !owner && g_vars.dz.healthshot )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 211, 27, 39 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Healthshot" );

		if( client_class->m_class_id == CDrone && g_vars.dz.drone ) {
			if( !owner )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Drone" );

			const auto drone_owner = static_cast< c_csplayer * >( owner );
			if( drone_owner )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Drone - %s", drone_owner->get_info( ).m_player_name );
		}

		if( client_class->m_class_id == CDronegun && g_vars.dz.turret ) {
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y - 12, CENTERED_X | DROPSHADOW, "Turret" );

			g_renderer.rect( ImColorARGB( 200, 10, 10, 10 ), box.x - 1, box.y - 1, box.w + 2, box.h + 2 );
			g_renderer.rect( ImColorARGB( 200, 10, 10, 10 ), box.x + 1, box.y + 1, box.w - 2, box.h - 2 );
			g_renderer.rect( ImColorARGB( 200, 255, 255, 255 ), box.x, box.y, box.w, box.h );

			if( entity->dronegun_has_target( ) )
				g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | OUTLINED, "TARGETING" );

			/*int hp = std::clamp( entity->dronegun_health(), 0, 220 );
			ImColorARGB health_color = ImColorARGB( 255, 255 - hp * 2.55, hp * 2.55, 0 );

			int height = hp * ( int )box.h / 220;

			g_renderer.filled_rect( ImColorARGB( 200, 10, 10, 10 ), box.x - 6, box.y - 1, 4, box.h + 2 );
			g_renderer.filled_rect_gradient( OSHGui::Drawing::ColorRectangle( health_color, health_color - ImColorARGB( 0, 50, 50, 0 ) ), box.x - 5, box.y + box.h - height, 2, height );

			if( hp >= 90 || hp <= 10 )
				return;

			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ),
				ImColorARGB( 200, 255, 255, 255 ),
				ImColorARGB( 200, 10, 10, 10 ),
				box.x - 8, box.y + box.h - height - 6, OUTLINED, "%i", hp );*/
		}

		if( client_class->m_class_id == CBRC4Target && g_vars.dz.safe )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 149, 184, 6 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Safe" );

		if( client_class->m_class_id == CPhysPropAmmoBox || client_class->m_class_id == CPhysPropLootCrate
			|| client_class->m_class_id == CPhysPropRadarJammer || client_class->m_class_id == CPhysPropWeaponUpgrade ) {
			if( !g_vars.dz.ammobox || !g_vars.dz.weapon_case )
				return;

			// epic.
			if( !model_name.empty( ) ) {
				if( g_vars.dz.weapon_case && model_name.find( "case_pistol" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Pistol case" );
				else if( g_vars.dz.weapon_case && model_name.find( "case_light_weapon" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Light case" );
				else if( g_vars.dz.weapon_case && model_name.find( "case_heavy_weapon" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Heavy case" );
				else if( g_vars.dz.weapon_case && model_name.find( "case_explosive" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Explosives case" );
				else if( g_vars.dz.tool_case && model_name.find( "case_tools" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Tools case" );
				else if( g_vars.dz.airdrop && model_name.find( "random" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Airdrop" );
				else if( g_vars.dz.armor && model_name.find( "dz_armor_helmet" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Full armor" );
				else if( g_vars.dz.armor && model_name.find( "dz_helmet" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Helmet" );
				else if( g_vars.dz.armor && model_name.find( "dz_armor" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Body armor" );
				else if( g_vars.dz.tablet_upgrade && model_name.find( "upgrade_tablet" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Tablet upgrade" );
				else if( g_vars.dz.contract && model_name.find( "briefcase" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 149, 184, 6 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Contract" );
				else if( g_vars.dz.parachute && model_name.find( "parachutepack" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Parachute" );
				else if( g_vars.dz.cash && model_name.find( "dufflebag" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 149, 184, 6 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Cash dufflebag" );
				else if( g_vars.dz.ammobox && model_name.find( "ammobox" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Ammobox" );
				else if( g_vars.dz.jammer && model_name.find( "jammer" ) != std::string::npos )
					g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x + box.w * 0.5f, box.y + box.h + 2, CENTERED_X | DROPSHADOW, "Radar jammer" );
			}
		}

		if( client_class->m_class_id == CBreachCharge && g_vars.dz.breach_charge )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Breach charge" );

		if( client_class->m_class_id == CMelee && !owner && g_vars.dz.melee_weapon )
			g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ), ImColorARGB( 200, 255, 255, 255 ), ImColorARGB( 200, 10, 10, 10 ), box.x, box.y - 15.f, CENTERED_X | DROPSHADOW, "Melee weapon" );
	}
}

void c_visuals::draw_bomb_timer( float time_left ) const {
	const ImVec2 display_size = ImGui::GetIO( ).DisplaySize;

	// todo: only grab this once on map load.
	cvar *max_c4_timer = g_csgo.m_convar->find_var( "mp_c4timer" );
	float x = time_left * display_size.x / max_c4_timer->get_float( );

	g_renderer.filled_rect( ImColorARGB( 145, 24, 24, 32 ), 0, 0, display_size.x, 3 );
	g_renderer.filled_rect( ImColorARGB( 255, 113, 59, 198 ), x, 0, time_left * -display_size.x / max_c4_timer->get_float( ), 3 );
	g_renderer.ansi_text( g_renderer.m_fonts.at( FONT_04B03_6PX ), ImColorARGB( 255, 255, 255, 255 ), ImColorARGB( 255, 0, 0, 0 ), x - 1, -1, DROPSHADOW, "%2.1f", time_left );
}

void c_visuals::watermark( ) const {
	const bool connected = g_csgo.m_engine->is_connected( );

	const ImVec2 display_size = ImGui::GetIO( ).DisplaySize;

	g_renderer.filled_rect( ImColorARGB( 225, 24, 24, 32 ), display_size.x - 270, 20, 250, 15 ); // removed gradient (temp)

	time_t t = std::time( nullptr );
	tm tm = *std::localtime( &t );

	std::ostringstream oss;
	oss << std::put_time( &tm, "%I:%M%p" );
	auto time = oss.str( );

	if( connected ) {
		const bool choking = g_csgo.m_clientstate->m_choked_commands > 1;	

		g_renderer.ansi_text( g_renderer.m_fonts[ FONT_VERDANA_7PX ], ImColorARGB( 220, 255, 255, 255 ),
			ImColorARGB( 0.f, 0.f, 0.f, 0.f ), display_size.x - 268, 22, 0,
			"deadcell | %s | out %ims | choking %s", time.c_str(), g_cl.m_ping, choking ? "true" : "false" );
	}
	else {
		g_renderer.ansi_text( g_renderer.m_fonts[ FONT_VERDANA_7PX ], ImColorARGB( 205, 255, 255, 255 ),
			ImColorARGB( 0.f, 0.f, 0.f, 0.f ), display_size.x - 268, 22, 0,
			"deadcell | unconnected | %s", time.c_str() );
	}
}

void c_visuals::draw_scope( ) const {
	auto local = c_csplayer::get_local( );
	if( !local || !local->is_scoped( ) || !local->get_active_weapon( ) || !local->get_active_weapon( )->has_sniper_scope( ) )
		return;

	const ImVec2 display_size = ImGui::GetIO( ).DisplaySize;

	if ( g_vars.visuals.misc.remove_scope == 1 ) {
		g_renderer.filled_rect( ImColorARGB( g_vars.visuals.misc.scope_color ), 0, display_size.y * 0.5f, display_size.x, 1 );
		g_renderer.filled_rect( ImColorARGB( g_vars.visuals.misc.scope_color ), display_size.x * 0.5f, 0, 1, display_size.y );
	}
	else {
		float spread = ( local->get_active_weapon( )->inaccuracy( ) + local->get_active_weapon( )->spread( ) ) * 320;
		float height = std::clamp( spread, 1.f, 25.f );
		float alpha = ( 255.f - ( height * 4.2f ) );

		auto color = ImColorARGB( g_vars.visuals.misc.scope_color, alpha );

		g_renderer.filled_rect( color, 0, ( display_size.y * 0.5f ) - ( height / 2.f ), display_size.x, height );
		g_renderer.filled_rect( color, ( display_size.x * 0.5f ) - ( height / 2.f ), 0, height, display_size.y );
	}
}

void c_visuals::draw_crosshair( ) const {
	auto local = c_csplayer::get_local( );
	if ( !local )
		return;

	auto weapon = local->get_active_weapon( );
	if ( !weapon )
		return;

	static std::vector< std::pair< float, float > > seeds;

	std::once_flag once_flag;
	std::call_once( once_flag, [ & ] {
		for ( int i = 0; i < 256; i++ ) {
			seeds.emplace_back( util::misc::get_random_float_range( 0.0f, 1.0f ), util::misc::get_random_float_range( 0.0f, math::pi_2 ) );
		}
	} );

	const ImVec2 display_size = ImGui::GetIO( ).DisplaySize;

	if ( g_vars.visuals.visualize_spread == 1 ) {
		const float spread_distance = ( weapon->inaccuracy( ) + weapon->spread( ) ) * 320.f / std::tan( math::deg_to_rad( g_vars.visuals.effects.camera_fov ) * 0.5f );
		const float spread_radius = display_size.y * 2 * 0.002083 * spread_distance;

		g_renderer.circle( ImColorARGB( g_vars.visuals.visualize_spread_color, 70.f ), display_size.x / 2, display_size.y / 2, spread_radius );
	}
	else if ( weapon ) {
		int screen_w, screen_h;
		g_csgo.m_engine->get_screen_size( screen_w, screen_h );
		float cross_x = screen_w / 2, cross_y = screen_h / 2;

		float recoil_step = screen_h / g_vars.visuals.effects.camera_fov;

		cross_x -= ( local->punch_angle( ).y * recoil_step );
		cross_y += ( local->punch_angle( ).x * recoil_step );

		weapon->update_accuracy( );
		float inaccuracy = weapon->inaccuracy( );
		float spread = weapon->spread( );

		float cone = inaccuracy * spread;
		cone *= screen_h * 0.7f;
		cone *= 90.f / g_vars.visuals.effects.camera_fov;

		for ( int seed = 0; seed < 256; ++seed ) {
			static auto random_seed = reinterpret_cast< void( *)( int ) >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" ) );
			random_seed( seed );

			float rand_a = math::random_float( 0.f, 1.0f );
			float pi_rand_a = math::random_float( 0.f, math::pi_2 );
			float rand_b = math::random_float( 0.0f, 1.0f );
			float pi_rand_b = math::random_float( 0.f, math::pi_2 );

			float spread_x = cos( pi_rand_a ) * ( rand_a * inaccuracy ) + cos( pi_rand_b ) * ( rand_b * spread );
			float spread_y = sin( pi_rand_a ) * ( rand_a * inaccuracy ) + sin( pi_rand_b ) * ( rand_b * spread );

			float max_x = cos( pi_rand_a ) * cone + cos( pi_rand_b ) * cone;
			float max_y = sin( pi_rand_a ) * cone + sin( pi_rand_b ) * cone;

			float step = screen_h / g_vars.visuals.effects.camera_fov * 90.f;
			float screen_spread_x = static_cast< int >( spread_x * step * 0.7f );
			float screen_spread_y = static_cast< int >( spread_y * step * 0.7f );

			float percentage = ( rand_a * inaccuracy + rand_b * spread ) / ( inaccuracy + spread );

			g_renderer.rect( ImColorARGB( g_vars.visuals.visualize_spread_color, 255 * ( 0.4f + percentage * 0.6f ) * ( 0.1f + percentage * 0.9f ) ), 
							 cross_x + screen_spread_x, cross_y + screen_spread_y, 1, 1 );
		}
	}
}

void c_visuals::draw_hitmarker( ) {
	auto display_size = ImGui::GetIO( ).DisplaySize;

	if( g_cl.m_hitmarker_alpha > 0.f ) {
		g_renderer.line( ImColorARGB( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), display_size.x / 2.f - 8.f, display_size.y / 2.f - 8.f, display_size.x / 2.f - 3.f, display_size.y / 2.f - 3.f );
		g_renderer.line( ImColorARGB( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), display_size.x / 2.f - 8.f, display_size.y / 2.f + 8.f, display_size.x / 2.f - 3.f, display_size.y / 2.f + 3.f );
		g_renderer.line( ImColorARGB( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), display_size.x / 2.f + 8.f, display_size.y / 2.f - 8.f, display_size.x / 2.f + 3.f, display_size.y / 2.f - 3.f );
		g_renderer.line( ImColorARGB( g_cl.m_hitmarker_alpha, 1.f, 1.f, 1.f ), display_size.x / 2.f + 8.f, display_size.y / 2.f + 8.f, display_size.x / 2.f + 3.f, display_size.y / 2.f + 3.f );

		g_cl.m_hitmarker_alpha -= 1.f / 0.5f * g_csgo.m_global_vars->m_frametime;
	}
}

