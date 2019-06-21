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
		m_shaded_mat = std::make_pair( create_material( VertexLitGeneric, false ), create_material( VertexLitGeneric, true ) );
		m_flat_mat = std::make_pair( create_material( UnlitGeneric, false ), create_material( UnlitGeneric, true ) );
		m_modulate_mat = std::make_pair( create_material( Modulate, false ), create_material( Modulate, true ) );

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

		// this will only update when the slider(s) value has changed.
		if( m_kv_needs_update && g_vars.visuals.chams.type == 0 ) {
			static auto kv = static_cast< key_values * >( g_csgo.m_memalloc->alloc( 36 ) );
			kv->init( "VertexLitGeneric" );

			auto reflectivity_str = std::to_string( g_vars.visuals.chams.reflectivity / 100.f );
			auto luminance_str = std::to_string( g_vars.visuals.chams.reflectivity / 100.f );

			auto reflectivity = "["
				+ reflectivity_str + " "
				+= reflectivity_str + " "
				+= reflectivity_str + "]";

			auto luminance = "["
				+ luminance_str + " "
				+= luminance_str + " "
				+= luminance_str + "]";

			kv->set_string( "$basetexture", "vgui/white_additive" );
			kv->set_string( "$envmaptint", reflectivity.c_str( ) );
			kv->set_string( "$envmap", "env_cubemap" );

			kv->set_int( "$phong", 1 );
			kv->set_int( "$phongexponent", 15 );
			kv->set_int( "$normalmapalphaenvmask", 1 );
			kv->set_string( "$phongboost", luminance.c_str( ) );
			kv->set_string( "$phongfresnelranges", "[.5 .5 1]" );
			kv->set_int( "$BasemapAlphaPhongMask", 1 );

			kv->set_int( "$rimlight", 1 );
			kv->set_int( "$rimlightexponent", 2 );
			kv->set_string( "$rimlight", reflectivity.c_str( ) );

			kv->set_int( "$model", 1 );
            kv->set_int( "$flat", 1 );
            kv->set_int( "$selfillum", 1 );
			kv->set_int( "$halflambert", 1 );

			material.first->set_shader_and_params( kv );

			kv->set_int( "$ignorez", 1 );

			material.second->set_shader_and_params( kv );

			m_kv_needs_update = false;
		}

		OSHColor c1 = OSHColor::FromARGB( g_vars.visuals.chams.vis_color );
		OSHColor c2 = OSHColor::FromARGB( g_vars.visuals.chams.hid_color );
		float vis_color[ 3 ] = { c1.GetRed( ), c1.GetGreen( ), c1.GetBlue( ) };
		float hid_color[ 3 ] = { c2.GetRed( ), c2.GetGreen( ), c2.GetBlue( ) };		

		if( g_vars.visuals.chams.twopass ) {
			g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
			g_csgo.m_model_render->forced_material_override( material.second );
			g_csgo.m_render_view->set_color_modulation( hid_color );
			ent->draw_model( STUDIO_RENDER, 255 );
		}

		g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
		g_csgo.m_render_view->set_color_modulation( vis_color );
		g_csgo.m_model_render->forced_material_override( material.first );
		g_csgo.m_model_render->forced_material_override( material.first );
		ent->draw_model( STUDIO_RENDER, 255 );
	}

	if( !m_players.empty( ) )
		m_players.clear( );

	g_csgo.m_model_render->forced_material_override( nullptr );
}

bool c_chams::on_dme( uintptr_t ecx, IMatRenderContext *ctx, void *state, model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld, hook::fn::DrawModelExecute_t orig ) {
	auto player = g_csgo.m_entity_list->get< c_csplayer >( pInfo.m_entity_index );
	auto local = c_csplayer::get_local( );
	if( !local || !player || !pInfo.m_model )
		return true;

	std::string model_name = g_csgo.m_model_info->get_model_name( ( model_t* )pInfo.m_model );

	if( pInfo.m_entity_index == g_csgo.m_engine->get_local_player( ) && g_csgo.m_input->m_camera_in_thirdperson && g_vars.visuals.chams.local && model_name.find( "shadow" ) == std::string::npos ) {
		// draw model on last sent origin
		// BUG: bone matrix is incorrect(?) when jumping
		if( local->velocity().length_2d( ) > 0.f ) {
			static i_material *dogtag = g_csgo.m_material_system->get_material( "models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL );

			g_csgo.m_model_render->forced_material_override( dogtag );
			float col[ 3 ] = { 1.f, 1.f, 1.f };
			g_csgo.m_render_view->set_color_modulation( col );

			auto set_matrix_pos = [ & ]( matrix3x4_t &mat, vec3_t origin ) {
				for( size_t i{ }; i < 3; ++i ) {
					mat[ i ][ 3 ] = origin[ i ];
				}
			};

			matrix3x4_t *new_matrix = pCustomBoneToWorld;
			for( size_t i{ }; i < 128; ++i ) {
				set_matrix_pos( new_matrix[ i ], g_cl.m_last_sent_origin );
			}

			orig( ecx, ctx, state, pInfo, new_matrix );
		}
		
		static auto mat = create_material( VertexLitGeneric, false, false );
		g_csgo.m_model_render->forced_material_override( mat );

		auto color = OSHColor::FromARGB( g_vars.visuals.chams.local_col );
		float col[ 3 ] = { color.GetRed( ), color.GetGreen( ), color.GetBlue( ) };
		g_csgo.m_render_view->set_color_modulation( col );

		if( g_vars.visuals.chams.blend_scope && local->is_scoped( ) )
			g_csgo.m_render_view->set_blend( 0.4f );

		orig( ecx, ctx, state, pInfo, pCustomBoneToWorld );
	}

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