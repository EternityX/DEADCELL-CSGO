#include "chams.h"

c_chams g_chams;

void c_chams::on_sceneend( ) {
	if( !g_vars.visuals.chams.enabled )
		return;

	const auto local = C_CSPlayer::get_local( );
	if( !local )
		return;

	push_players( );

	static std::vector< IMaterial * > materials;

	static bool once{ false };
	if( !once ) {
		materials.push_back( create_material( true, false, false ) ); // material.
		materials.push_back( create_material( false, false, false ) ); // flat.
		materials.push_back( create_material( true, false, true ) ); // material zignore.
		materials.push_back( create_material( false, false, true ) ); // flat zignore.
		once = true;
	}

	for( auto &e : m_players ) {
		auto ent = e.first;

		if( ent == local )
			continue;

		OSHColor c1 = OSHColor::FromARGB( g_vars.visuals.chams.vis_color );
		OSHColor c2 = OSHColor::FromARGB( g_vars.visuals.chams.hid_color );
		float vis_color[ 3 ] = { c1.GetRed( ), c1.GetGreen( ), c1.GetBlue( ) };
		float hid_color[ 3 ] = { c2.GetRed( ), c2.GetGreen( ), c2.GetBlue( ) };

		

		// you can change the material's shader values using SetShaderAndParams and passing in a keyvalue pointer
		// example : 
		// static auto kv = static_cast<KeyValues*>( g_csgo.m_memalloc->Alloc( 36 ) );
		// kv->SetInt( "$ignorez", 1 );
		// this would update the material in real time without having to create a new material, obviously this can be used with more relevant
		// shader params such as reflection for example, would allow us to control it with a slider instead of having it as a static value 
		// when the material is created

		if( g_vars.visuals.chams.twopass ) {
			g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
			g_csgo.m_model_render->ForcedMaterialOverride( g_vars.visuals.chams.type ? materials.at( 3 ) : materials.at( 2 ) );
			g_csgo.m_render_view->set_color_modulation( hid_color );
			m_applied = true;
			ent->DrawModel( STUDIO_RENDER, 255 );
			m_applied = false;
		}

		g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
		g_csgo.m_model_render->ForcedMaterialOverride( g_vars.visuals.chams.type ? materials.at( 1 ) : materials.at( 0 ) );
		g_csgo.m_render_view->set_color_modulation( vis_color );
		m_applied = true;
		ent->DrawModel( STUDIO_RENDER, 255 );
		m_applied = false;
	}

	if( !m_players.empty( ) )
		m_players.clear( );

	g_csgo.m_model_render->ForcedMaterialOverride( nullptr );
}

bool c_chams::on_dme( IMatRenderContext *ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld ) {
	auto player = g_csgo.m_entity_list->Get< C_CSPlayer >( pInfo.m_entity_index );
	auto local = C_CSPlayer::get_local( );
	if( !local || !player || !pInfo.m_model )
		return true;

	std::string model_name = g_csgo.m_model_info->GetModelName( ( model_t* )pInfo.m_model );

	if( pInfo.m_entity_index == g_csgo.m_engine->GetLocalPlayer( ) && g_csgo.m_input->m_fCameraInThirdPerson && g_vars.visuals.chams.local ) {
		static auto mat = create_material( true, false, false, 7 );
		g_csgo.m_model_render->ForcedMaterialOverride( mat );
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
	for( int index = 1; index <= g_csgo.m_global_vars->m_max_clients; index++ ) {
		auto player = g_csgo.m_entity_list->Get< C_CSPlayer >( index );

		if( !player || !player->alive( ) || player->IsDormant( ) || ( ( player->team( ) == C_CSPlayer::get_local( )->team( ) ) && !g_vars.visuals.chams.teammates ) )
			continue;

		if( player->immune( ) )
			continue;

		float dist = player->abs_origin( ).distance( C_CSPlayer::get_local( )->abs_origin( ) );
		m_players.emplace_back( player, dist );
	}

	std::sort( m_players.begin( ), m_players.end( ), [ ]( const std::pair< C_CSPlayer*, float > &left, const std::pair< C_CSPlayer*, float > &right ) {
		return left.second < right.second;
	} );
}

IMaterial *c_chams::create_material( bool shade, bool wireframe, bool ignorez, int rimlight_boost ) const {
	static const std::string material_name = "game_material.vmt";
	const std::string material_type = shade ? "VertexLitGeneric" : "UnlitGeneric";

	std::string material_data;

	material_data += "\"" + material_type + "\"\n{\n";
	material_data += "\t\"$basetexture\" \"vgui/white_additive\"\n";
	material_data += "\t\"$envmap\" \"\"\n";
	material_data += "\t\"$model\" \"1\"\n";
	material_data += "\t\"$flat\" \"1\"\n";
	material_data += "\t\"$nocull\" \"0\"\n";
	material_data += "\t\"$selfillum\" \"1\"\n";
	material_data += "\t\"$halflambert\" \"1\"\n";
	material_data += "\t\"$znearer\" \"0\"\n";
	material_data += "\t\"$rimlightboost\" \"" + std::to_string( rimlight_boost ) + "\"\n";
	material_data += "\t\"$rimlightexponent\" \"" + std::to_string( 4 ) + "\"\n";
	material_data += "\t\"$ambientreflectionboost\" \"0.2\"\n";
	material_data += "\t\"$envmaplightscale\" \"" + std::to_string( 0.1 ) + "\"\n";
	material_data += "\t\"$wireframe\" \"" + std::to_string( wireframe ) + "\"\n";
	material_data += "\t\"$ignorez\" \"" + std::to_string( ignorez ) + "\"\n";
	material_data += "}\n";

	auto kv = static_cast<KeyValues*>( g_csgo.m_memalloc->Alloc( 36 ) );
	kv->Init( material_type.c_str( ) );
	kv->LoadFromBuffer( material_name.c_str( ), material_data.c_str( ) );

	return g_csgo.m_material_system->CreateMaterial( material_name.c_str( ), kv );
}
