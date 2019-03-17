#include "chams.h"

c_chams g_chams;

void c_chams::on_sceneend( ){
	if( !g_vars.visuals.chams.enabled )
		return;

	auto local = C_CSPlayer::get_local( );
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

	for( auto &e : m_players ){
		auto ent = e.first;

		if( ent == local )
			continue;

		OSHColor c1 = OSHColor::FromARGB( g_vars.visuals.chams.vis_color );
		OSHColor c2 = OSHColor::FromARGB( g_vars.visuals.chams.hid_color );
		float vis_color[ 3 ] = { c1.GetRed( ), c1.GetGreen( ), c1.GetBlue( ) };
		float hid_color[ 3 ] = { c2.GetRed( ), c2.GetGreen( ), c2.GetBlue( ) };

		if( g_vars.visuals.chams.twopass ){
			g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
			g_csgo.m_model_render->ForcedMaterialOverride( g_vars.visuals.chams.type ? materials.at( 3 ) : materials.at( 2 ) );
			g_csgo.m_render_view->set_color_modulation( hid_color );
			applied = true;
			ent->DrawModel( STUDIO_RENDER, 255 );
			applied = false;
		}

		g_csgo.m_render_view->set_blend( g_vars.visuals.chams.alpha / 100.f );
		g_csgo.m_model_render->ForcedMaterialOverride( g_vars.visuals.chams.type ? materials.at( 1 ) : materials.at( 0 ) );
		g_csgo.m_render_view->set_color_modulation( vis_color );
		applied = true;
		ent->DrawModel( STUDIO_RENDER, 255 );
		applied = false;
	}

	if ( !m_players.empty(  ) )
		m_players.clear( );

	g_csgo.m_model_render->ForcedMaterialOverride( nullptr );
}

bool c_chams::on_dme( IMatRenderContext *ctx, void *state,const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld ){
	auto player = reinterpret_cast< C_CSPlayer* >( g_csgo.m_entity_list->GetClientEntity( pInfo.m_entity_index ) );
	auto local = C_CSPlayer::get_local( );
	if ( !local || !player || !pInfo.m_model )
		return true;

	std::string model_name = g_csgo.m_model_info->GetModelName( (model_t*)pInfo.m_model );

	if( pInfo.m_entity_index == g_csgo.m_engine->GetLocalPlayer( ) && g_csgo.m_input->m_fCameraInThirdPerson && g_vars.visuals.chams.local ){
		static auto mat = create_material( true, false, false, 7 );
		g_csgo.m_model_render->ForcedMaterialOverride( mat );
		auto color = OSHColor::FromARGB( g_vars.visuals.chams.local_col ); 
		float col[ 3 ] = { color.GetRed( ), color.GetGreen( ), color.GetBlue( ) };
		g_csgo.m_render_view->set_color_modulation( col );
		if( local->is_scoped( ) )
			g_csgo.m_render_view->set_blend( 0.4f );
	}

	if ( g_vars.visuals.chams.enabled && model_name.find( "models/player" ) != std::string::npos ) {
		if ( model_name.find( "shadow" ) != std::string::npos )
			return true;

		if ( applied )
			return true;
		else if ( player->team( ) != local->team( ) )
			return false;
		else if( ( player->team( ) == local->team( ) ) && g_vars.visuals.chams.teammates && player != C_CSPlayer::get_local( ) )
			return false;
	}

	return true;
}

void c_chams::push_players( ){
	for ( int index = 1; index <= g_csgo.m_global_vars->m_max_clients; index++ ) {
		auto player = reinterpret_cast< C_CSPlayer* >( g_csgo.m_entity_list->GetClientEntity( index ) );

		if ( !player || !player->alive( ) || player->IsDormant( ) || ( ( player->team( ) == C_CSPlayer::get_local( )->team( ) ) && !g_vars.visuals.chams.teammates )  )
			continue;

		if( player->immune( ) )
			continue;

		float dist = player->abs_origin( ).DistTo( C_CSPlayer::get_local( )->abs_origin( ) );
		m_players.emplace_back( player, dist );
	}

	std::sort( m_players.begin( ), m_players.end( ), [ ] ( const std::pair<C_CSPlayer*, float> &left, const std::pair<C_CSPlayer*, float> &right ) { return left.second < right.second; } );
}

IMaterial * c_chams::create_material( bool shade, bool wireframe, bool ignorez, int rimlight_boost ){
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

	auto init_key_values = [ ]( KeyValues *keyvalues, const char *key_name  ) -> void {
		using InitKeyValues_t = void(__thiscall *)( void *, const char * );
		static InitKeyValues_t InitKeyValuesEx = nullptr;

		if( !InitKeyValuesEx )
			InitKeyValuesEx = pattern::find< InitKeyValues_t >( g_csgo.m_client_dll, "55 8B EC 51 33 C0 C7 45" );

		InitKeyValuesEx( keyvalues, key_name );
	};
	auto load_from_buffer = [ ]( KeyValues *key_values, const char *resource_name, const char *buf, void *file_sys = nullptr, const char *path_id = nullptr, void *eval_sym_proc = nullptr, void *unk = nullptr ) -> void {
		using LoadFromBuffer_t = void(__thiscall *)( void *, const char *, const char *, void *, const char *, void *, void * );
		static LoadFromBuffer_t LoadFromBufferEx = nullptr;
		
		if( !LoadFromBufferEx )
			LoadFromBufferEx = pattern::find< LoadFromBuffer_t >( g_csgo.m_client_dll, "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" );

		LoadFromBufferEx( key_values, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
	};

	auto *key_values = new KeyValues( );
	init_key_values( key_values, material_type.c_str( ) );
	load_from_buffer( key_values, material_name.c_str( ), material_data.c_str( ) );

	return g_csgo.m_material_system->CreateMaterial( material_name.c_str( ), key_values );
}