#include "chams.h"
#include "../entity listener/ent_listener.h"

c_chams g_chams;

void c_chams::on_sceneend( ) {
	if( !g_vars.visuals.chams.enabled )
		return;

	const auto local = c_csplayer::get_local( );
	if( !local )
		return;

	push_players( );

	static std::pair< i_material *, i_material * > material;

	static bool once{ false };
	if( !once ) {
		m_shaded_mat = std::make_pair( create_material( shader_type_t::VertexLitGeneric, false ), 
									   create_material( shader_type_t::VertexLitGeneric, true ) );

		m_flat_mat = std::make_pair( create_material( shader_type_t::UnlitGeneric, false ), 
									 create_material( shader_type_t::UnlitGeneric, true ) );

		m_modulate_mat = std::make_pair( create_material( shader_type_t::Modulate, false ), 
										 create_material( shader_type_t::Modulate, true ) );

		once = true;
	}

	for( auto &e : m_players ) {
		auto ent = e.first;
		if( ent == local )
			continue;

		switch( g_vars.visuals.chams.type ) {
			case 0: material = m_shaded_mat; break; // vertex lit
			case 1: material = m_flat_mat; break; // unlit
			case 2: material = m_modulate_mat; break; // modulate
			default: break;
		}

		if( g_vars.visuals.chams.type == 0 ) {
			static auto kv = static_cast<key_values *>( g_csgo.m_memalloc->alloc( 36 ) );
			kv->init( "VertexLitGeneric" );

			std::string param = "["
				+ std::to_string( g_vars.visuals.chams.reflectivity ) + " "
				+ std::to_string( g_vars.visuals.chams.reflectivity ) + " "
				+ std::to_string( g_vars.visuals.chams.reflectivity ) + "]";

			kv->set_string( "$basetexture", "vgui/white_additive" );
			kv->set_string( "$envmaptint", param.c_str( ) );
			kv->set_string( "$envmap", "env_cubemap" );

			material.first->set_shader_and_params( kv );
			material.second->set_shader_and_params( kv );
		}

		OSHColor c1 = OSHColor::FromARGB( g_vars.visuals.chams.vis_color );
		OSHColor c2 = OSHColor::FromARGB( g_vars.visuals.chams.hid_color );
		float vis_color[ 3 ] = { c1.GetRed( ), c1.GetGreen( ), c1.GetBlue( ) };
		float hid_color[ 3 ] = { c2.GetRed( ), c2.GetGreen( ), c2.GetBlue( ) };		

		if( g_vars.visuals.chams.twopass ) {
			g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
			g_csgo.m_model_render->forced_material_override( material.second );
			g_csgo.m_render_view->set_color_modulation( hid_color );
			m_applied = true;
			ent->draw_model( STUDIO_RENDER, 255 );
			m_applied = false;
		}

		g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
		g_csgo.m_model_render->forced_material_override( material.first );
		g_csgo.m_render_view->set_color_modulation( vis_color );	
		m_applied = true;
		ent->draw_model( STUDIO_RENDER, 255 );
		m_applied = false;
	}

	if( !m_players.empty( ) )
		m_players.clear( );

	g_csgo.m_model_render->forced_material_override( nullptr );
}

bool c_chams::on_dme( IMatRenderContext *ctx, void *state, const model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld ) {
	auto player = g_csgo.m_entity_list->get< c_csplayer >( pInfo.m_entity_index );
	auto local = c_csplayer::get_local( );
	if( !local || !player || !pInfo.m_model )
		return true;

	std::string model_name = g_csgo.m_model_info->get_model_name( ( model_t* )pInfo.m_model );

	if( pInfo.m_entity_index == g_csgo.m_engine->get_local_player( ) && g_csgo.m_input->m_camera_in_thirdperson && g_vars.visuals.chams.local ) {
		static auto mat = create_material( shader_type_t::VertexLitGeneric, false, false );
		g_csgo.m_model_render->forced_material_override( mat );
		auto color = OSHColor::FromARGB( g_vars.visuals.chams.local_col );
		float col[ 3 ] = { color.GetRed( ), color.GetGreen( ), color.GetBlue( ) };
		g_csgo.m_render_view->set_color_modulation( col );
		if( local->is_scoped( ) )
			g_csgo.m_render_view->set_blend( 0.4f );
	}

	/*if( g_vars.visuals.chams.enabled && model_name.find( "models/player" ) != std::string::npos ) {
		if( model_name.find( "shadow" ) != std::string::npos )
			return true;

		if( m_applied )
			return true;
		if( player->team( ) != local->team( ) )
			return false;
		if( ( player->team( ) == local->team( ) ) && g_vars.visuals.chams.teammates && player != C_CSPlayer::get_local( ) )
			return false;
	}*/

	return true;
}

void c_chams::push_players( ) {
	for( auto &entry : g_listener.m_players ) {
		auto player = g_csgo.m_entity_list->get< c_csplayer >( entry.m_idx );

		if( !player || !player->alive( ) || player->is_dormant( ) || ( ( player->team( ) == c_csplayer::get_local( )->team( ) ) && !g_vars.visuals.chams.teammates ) )
			continue;

		if( player->immune( ) )
			continue;

		float dist = player->abs_origin( ).distance( c_csplayer::get_local( )->abs_origin( ) );
		m_players.emplace_back( player, dist );
	}

	std::sort( m_players.begin( ), m_players.end( ), [ ]( const std::pair< c_csplayer*, float > &left, const std::pair< c_csplayer*, float > &right ) {
		return left.second < right.second;
	} );
}

i_material *c_chams::create_material( shader_type_t shade, bool ignorez, bool wireframe ) {
	static const std::string material_name = "game_material.vmt";
	std::string shade_type;

	switch( shade ) {
		case VertexLitGeneric: shade_type = "VertexLitGeneric"; break;
		case UnlitGeneric: shade_type = "UnlitGeneric"; break;
		case Modulate: shade_type = "Modulate"; break;
		default: break;
	}

	std::string material_data;

	material_data += "\"" + shade_type + "\"\n{\n";
	material_data += "\t\"$basetexture\" \"vgui/white_additive\"\n";
	material_data += "\t\"$envmap\" \"env_cubemap\"\n";
	material_data += "\t\"$envmaptint\" \"[.8 .8 .8]\"\n";
	/*material_data += "\t\"$envmapcontrast\" \"1\"\n";
	material_data += "\t\"$envmapsaturation\" \"1.0\"\n";*/

	material_data += "\t\"$model\" \"1\"\n";
	material_data += "\t\"$flat\" \"1\"\n";
	material_data += "\t\"$selfillum\" \"1\"\n";
	material_data += "\t\"$selfillumtint\" \"[8 8 8]\"\n";
	material_data += "\t\"$halflambert\" \"1\"\n";
	material_data += "\t\"$wireframe\" \"" + std::to_string( wireframe ) + "\"\n";
	material_data += "\t\"$ignorez\" \"" + std::to_string( ignorez ) + "\"\n";

	material_data += "\t\"$mod2x\" \"1\"\n";
	material_data += "\t\"$nocull\" \"1\"\n"; // for Modulate shader	

	material_data += "\"Proxies\"\n{\n";
	// add proxies here.
	material_data += "}\n"; // end of proxies.

	material_data += "}\n"; // end of shader type.

	auto kv = static_cast< key_values* >( g_csgo.m_memalloc->alloc( 36 ) );
	kv->init( shade_type.c_str( ) );
	kv->load_from_buffer( material_name.c_str( ), material_data.c_str( ) );

	return g_csgo.m_material_system->create_material( material_name.c_str( ), kv );
}
