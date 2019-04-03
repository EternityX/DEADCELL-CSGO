#include "misc.h"
#include "../anti-aim/antiaim.h"

c_misc g_misc;

void c_misc::bunnyhop( CUserCmd *cmd ) {
	if( !g_cl.m_local )
		return;

	uint8_t move_type = g_cl.m_local->get_move_type( );
	if( move_type == MOVETYPE_LADDER || move_type == MOVETYPE_NOCLIP )
		return;

	static bool last_jumped = false, should_fake = false;
	if( !last_jumped && should_fake ) {
		should_fake = false;
		cmd->m_buttons |= IN_JUMP;
	}
	else if( cmd->m_buttons & IN_JUMP ) {
		if( g_cl.m_local->flags( ) & FL_ONGROUND ) {
			last_jumped = true;
			should_fake = true;
		}
		else {
			cmd->m_buttons &= ~IN_JUMP;
			last_jumped = false;
		}
	}
	else {
		last_jumped = false;
		should_fake = false;
	}
}

void c_misc::automatic_fire( C_BaseCombatWeapon *active_weapon, CUserCmd *cmd ) {
	if( !active_weapon || !cmd )
		return;

	const WeaponInfo_t *wep_info = active_weapon->get_weapon_info( );

	if( wep_info->full_auto || !( wep_info->type == WEAPONTYPE_PISTOL ) || !( cmd->m_buttons & IN_ATTACK ) )
		return;

	float next_wep_attack = active_weapon->next_attack( ) - g_csgo.m_global_vars->m_cur_time;
	float next_local_attack = g_cl.m_local->next_attack( ) - g_csgo.m_global_vars->m_cur_time;
	if( next_wep_attack >= 0.f || next_local_attack >= 0.f )
		cmd->m_buttons &= ~IN_ATTACK;
	else
		cmd->m_buttons |= IN_ATTACK;
}

void c_misc::thirdperson( ) {
	auto local = C_CSPlayer::get_local( );

	if( !local )
		return;

	if( g_vars.misc.thirdperson_key != 0 )
	{
		static bool is_clicked = false;
		static bool is_down = false;
		
		bool key_pressed = g_input.key_pressed(g_vars.misc.thirdperson_key);
		is_clicked = !key_pressed && is_down;
		is_down = key_pressed;

		if (is_clicked)
			g_vars.misc.thirdperson = !g_vars.misc.thirdperson;
	}
	

	static vec3_t vecAngles;

	g_csgo.m_engine->GetViewAngles( vecAngles );

	if( g_vars.misc.thirdperson && local->alive( ) ) {
		if( !g_csgo.m_input->m_fCameraInThirdPerson ) {
			auto GetCorrectDistance = [ & ]( float ideal_distance ) -> float {
				vec3_t inverseAngles;
				g_csgo.m_engine->GetViewAngles( inverseAngles );

				inverseAngles.x *= -1.f, inverseAngles.y += 180.f;

				vec3_t direction;
				math::angle_to_vector( inverseAngles, direction );

				Ray_t ray;
				trace_t trace;
				CTraceFilterSkipEntity filter( local );

				ray.init( local->eye_pos( ), local->eye_pos( ) + ( direction * ideal_distance ) );

				g_csgo.m_engine_trace->TraceRay( ray, MASK_SHOT, &filter, &trace );

				return ( ideal_distance * trace.fraction ) - 10.f;
			};

			vecAngles.z = GetCorrectDistance( 60.f );

			g_csgo.m_input->m_fCameraInThirdPerson = true;

			g_csgo.m_input->m_vecCameraOffset = vec3_t( vecAngles.x, vecAngles.y, vecAngles.z );
		}
	}
	else {
		g_csgo.m_input->m_fCameraInThirdPerson = false;

		g_csgo.m_input->m_vecCameraOffset = vec3_t( vecAngles.x, vecAngles.y, 0 );
	}

}

void c_misc::thirdperson( CViewSetup *setup ) {
	auto local = C_CSPlayer::get_local( );
	if( !local )
		return;

	static bool once = false;
	static bool once2 = false;

	if( g_vars.misc.thirdperson_dead ) {
		once = true;

		if( local->alive( ) ) {
			once2 = false;
			return;
		}

		auto spechandle = local->observer_handle( );

		auto spec = g_csgo.m_entity_list->Get< C_CSPlayer >( spechandle );
		if( !spec )
			return;

		static bool once2 = false;

		if( once2 )
			local->observer_mode( ) = 5;

		if( local->observer_mode( ) == 4 )
			once2 = true;

		static vec3_t angles;
		g_csgo.m_engine->GetViewAngles( angles );
		setup->m_angles = angles;

		trace_t tr;
		Ray_t ray;

		vec3_t forward, right, up;
		math::angle_to_vectors( angles, &forward, &right, &up );

		vec3_t cam_offset = spec->eye_pos( ) + ( forward * -120.f ) + ( right ) + ( up );

		ray.init( spec->eye_pos( ), cam_offset );
		CTraceFilterWorldOnly traceFilter;
		traceFilter.m_skip = spec;
		g_csgo.m_engine_trace->TraceRay( ray, MASK_SHOT, &traceFilter, &tr );

		setup->m_origin = tr.endpos;
	}
	else if( once ) {
		local->observer_mode( ) = local->alive( ) ? 0 : 4;
		once = false;
	}
}

void c_misc::strafe( CUserCmd *cmd ) {
	auto local = C_CSPlayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	if( local->flags( ) & FL_ONGROUND && !( cmd->m_buttons & IN_JUMP ) )
		return;

	if( local->get_move_type( ) & ( MOVETYPE_LADDER || MOVETYPE_NOCLIP ) )
		return;

	vec3_t viewangles;
	g_csgo.m_engine->GetViewAngles( viewangles );

	static bool side_switch = false;
	side_switch = !side_switch;

	cmd->m_forwardmove = 0.f;
	cmd->m_sidemove = side_switch ? 450.f : -450.f;

	float velocity_yaw = local->velocity( ).y;

	float rotation = util::misc::clamp< float >( math::rad_to_deg( std::atan2( 15.f, local->velocity( ).Length2D( ) ) ), 0.f, 90.f );

	float abs_yaw = std::fabs( math::normalize_angle( velocity_yaw - viewangles.y ) );

	float ideal_yaw_rotation = ( side_switch ? rotation : -rotation ) + ( abs_yaw < 5.f ? velocity_yaw : viewangles.y );

	float cos_rot = std::cos( math::deg_to_rad( viewangles.y - ideal_yaw_rotation ) );
	float sin_rot = std::sin( math::deg_to_rad( viewangles.y - ideal_yaw_rotation ) );

	cmd->m_forwardmove = ( cos_rot * cmd->m_forwardmove ) - ( sin_rot * cmd->m_sidemove );
	cmd->m_sidemove = ( sin_rot * cmd->m_forwardmove ) + ( cos_rot * cmd->m_sidemove );
}

void c_misc::auto_zeus( CUserCmd *cmd ) {
	auto local = C_CSPlayer::get_local( );
	if( !local || !local->alive( ) )
		return;
	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;
	if( weapon->item_index( ) != WEAPON_TASER )
		return;

	for( int i = 1; i <= g_csgo.m_global_vars->m_max_clients; ++i ) {
		auto target = g_csgo.m_entity_list->Get< C_CSPlayer >( i );

		if( !target || target->IsDormant( ) || target->team( ) == local->team( ) || target->immune( ) )
			continue;
		std::vector< int > hitboxes = { pelvis, u_chest, l_chest };
		std::array< matrix3x4_t, 128 > matrix = {};
		std::vector< vec3_t > points;
		vec3_t best_point;

		if( !target->SetupBones( matrix.data( ), 128, 0x100, target->simtime( ) ) )
			continue;

		auto studiohdr = target->studio_hdr( );
		if( !studiohdr )
			continue;

		for( auto &h : hitboxes ) {
			auto *bbox = studiohdr->pHitbox( h, 0 );

			vec3_t max, min;
			math::vector_transform( bbox->bb_min, matrix.at( bbox->bone_index ), min );
			math::vector_transform( bbox->bb_max, matrix.at( bbox->bone_index ), max );
			auto center = ( min + max ) / 2.f;

			points.push_back( center );
		}

		auto is_visible = [ & ]( const vec3_t &start, const vec3_t &end, C_CSPlayer *target ) -> bool {
			Ray_t ray;
			trace_t tr;
			ray.init( start, end );

			CTraceFilterSkipEntity filter( g_cl.m_local );

			g_csgo.m_engine_trace->TraceRay( ray, MASK_SHOT, &filter, &tr );

			return ( tr.hit_entity == target || tr.fraction > 0.97f );
		};

		for( auto &p : points ) {
			if( is_visible( local->eye_pos( ), p, target ) ) {
				best_point = p;
			}
		}

		vec3_t aim_angle = math::calc_angle( local->eye_pos( ), best_point );
		aim_angle.Clamp( );

		vec3_t forward;
		math::angle_to_vector( aim_angle, forward );
		forward *= 183;

		vec3_t eye_pos = local->eye_pos( );

		Ray_t ray;
		ray.init( eye_pos, eye_pos + forward );

		CTraceFilter filter;
		filter.m_skip = local;

		CGameTrace trace;
		g_csgo.m_engine_trace->TraceRay( ray, MASK_SHOT, &filter, &trace );

		if( !local->can_shoot( weapon ) )
			return;

		if( trace.hit_entity == target ) {
			cmd->m_buttons |= IN_ATTACK;

			static auto firing = false;
			if( cmd->m_buttons & IN_ATTACK ) {
				if( firing ) {
					cmd->m_buttons &= ~IN_ATTACK;
				}
			}
			firing = ( cmd->m_buttons & IN_ATTACK ) != 0;

			if( cmd->m_buttons & IN_ATTACK ) {
				cmd->m_viewangles = aim_angle;
			}
		}
	}
}

void c_misc::fix_movement( CUserCmd *cmd, vec3_t wish_angle ) const {
	vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	vec3_t viewangles = cmd->m_viewangles;

	math::angle_to_vectors( wish_angle, &view_fwd, &view_right, &view_up );
	math::angle_to_vectors( viewangles, &cmd_fwd, &cmd_right, &cmd_up );

	const float v8 = std::sqrtf( view_fwd.x * view_fwd.x + view_fwd.y * view_fwd.y );
	const float v10 = std::sqrtf( view_right.x * view_right.x + view_right.y * view_right.y );
	const float v12 = std::sqrtf( view_up.z * view_up.z );

	const vec3_t norm_view_fwd( 1.f / v8 * view_fwd.x, 1.f / v8 * view_fwd.y, 0.f );
	const vec3_t norm_view_right( 1.f / v10 * view_right.x, 1.f / v10 * view_right.y, 0.f );
	const vec3_t norm_view_up( 0.f, 0.f, 1.f / v12 * view_up.z );

	const float v14 = std::sqrtf( cmd_fwd.x * cmd_fwd.x + cmd_fwd.y * cmd_fwd.y );
	const float v16 = std::sqrtf( cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y );
	const float v18 = std::sqrtf( cmd_up.z * cmd_up.z );

	const vec3_t norm_cmd_fwd( 1.f / v14 * cmd_fwd.x, 1.f / v14 * cmd_fwd.y, 0.f );
	const vec3_t norm_cmd_right( 1.f / v16 * cmd_right.x, 1.f / v16 * cmd_right.y, 0.f );
	const vec3_t norm_cmd_up( 0.f, 0.f, 1.f / v18 * cmd_up.z );

	const float v22 = norm_view_fwd.x * cmd->m_forwardmove;
	const float v26 = norm_view_fwd.y * cmd->m_forwardmove;
	const float v28 = norm_view_fwd.z * cmd->m_forwardmove;
	const float v24 = norm_view_right.x * cmd->m_sidemove;
	const float v23 = norm_view_right.y * cmd->m_sidemove;
	const float v25 = norm_view_right.z * cmd->m_sidemove;
	const float v30 = norm_view_up.x * cmd->m_upmove;
	const float v27 = norm_view_up.z * cmd->m_upmove;
	const float v29 = norm_view_up.y * cmd->m_upmove;

	cmd->m_forwardmove = norm_cmd_fwd.x * v24 + norm_cmd_fwd.y * v23 + norm_cmd_fwd.z * v25
		+ ( norm_cmd_fwd.x * v22 + norm_cmd_fwd.y * v26 + norm_cmd_fwd.z * v28 )
		+ ( norm_cmd_fwd.y * v30 + norm_cmd_fwd.x * v29 + norm_cmd_fwd.z * v27 );

	cmd->m_sidemove = norm_cmd_right.x * v24 + norm_cmd_right.y * v23 + norm_cmd_right.z * v25
		+ ( norm_cmd_right.x * v22 + norm_cmd_right.y * v26 + norm_cmd_right.z * v28 )
		+ ( norm_cmd_right.x * v29 + norm_cmd_right.y * v30 + norm_cmd_right.z * v27 );

	cmd->m_upmove = norm_cmd_up.x * v23 + norm_cmd_up.y * v24 + norm_cmd_up.z * v25
		+ ( norm_cmd_up.x * v26 + norm_cmd_up.y * v22 + norm_cmd_up.z * v28 )
		+ ( norm_cmd_up.x * v30 + norm_cmd_up.y * v29 + norm_cmd_up.z * v27 );

	cmd->m_forwardmove = util::misc::clamp( cmd->m_forwardmove, -450.f, 450.f );
	cmd->m_sidemove = util::misc::clamp( cmd->m_sidemove, -450.f, 450.f );
	cmd->m_upmove = util::misc::clamp( cmd->m_upmove, -320.f, 320.f );
}

void c_misc::nightmode( C_BaseEntity *ent, float override_brightness ) {
	if( !g_csgo.m_engine->IsConnected( ) || !g_cl.m_local || !ent )
		return;

	auto client_class = ent->GetClientClass( );
	if( !client_class || client_class->m_ClassID != CEnvTonemapController )
		return;

	float brightness = g_vars.misc.nightmode / 100.f;
	if( !g_vars.misc.nightmode )
		brightness = 0.f;

	if( override_brightness > 0.f )
		brightness = override_brightness / 100.f;

	// will not work if post processing is disabled.
	auto tonemap_controller = reinterpret_cast< C_EnvTonemapController * >( ent );

	tonemap_controller->use_custom_exposure_min( ) = 1;
	tonemap_controller->use_custom_exposure_max( ) = 1;

	tonemap_controller->custom_exposure_min( ) = brightness;
	tonemap_controller->custom_exposure_max( ) = brightness;
	
	// disable fast path.
	/*static ConVar *r_drawspecificstaticprop = g_csgo.m_convar->FindVar( "r_drawspecificstaticprop" );
	r_drawspecificstaticprop->SetValue( 0 );

	float brightness = g_vars.misc.nightmode / 100.f;

	if( override_brightness > 0.f )
		brightness = override_brightness / 100.f;

	for( int i = g_csgo.m_material_system->FirstMaterial( ); i != g_csgo.m_material_system->InvalidMaterial( ); i = g_csgo.m_material_system->NextMaterial( i ) ) {
		IMaterial *material = g_csgo.m_material_system->FindMaterial( i );
		if( !material || material->IsErrorMaterial( ) )
			continue;

		if( std::strstr( material->GetTextureGroupName( ), "StaticProp" ) )
			material->ColorModulate( brightness, brightness, brightness + 0.05f );

		if( std::strstr( material->GetTextureGroupName( ), "World" ) )
			brightness < 0.10f ? material->ColorModulate( 0.f, 0.f, 0.05f ) : material->ColorModulate( brightness - 0.10f, brightness - 0.10f, brightness + 0.05f );

		if( std::strstr( material->GetName( ), "glass" ) || std::strstr( material->GetName( ), "decals" ) || std::strstr( material->GetName( ), "door" ) )
			brightness < 0.15f ? material->ColorModulate( 0.f, 0.f, 0.05f ) : material->ColorModulate( brightness - 0.15f, brightness - 0.15f, brightness + 0.05f );
		
	}*/
}

void c_misc::transparent_props( float override_transparency ) {
	if( !g_csgo.m_engine->IsConnected( ) || !g_cl.m_local )
		return;

	// disable fast path.
	static ConVar *r_drawspecificstaticprop = g_csgo.m_convar->FindVar( "r_drawspecificstaticprop" );
	r_drawspecificstaticprop->SetValue( 0 );

	float translucency = g_vars.misc.prop_transparency / 100.f;

	if( override_transparency > 0.f )
		translucency = override_transparency / 100.f;

	for( int i = g_csgo.m_material_system->FirstMaterial( ); i != g_csgo.m_material_system->InvalidMaterial( ); i = g_csgo.m_material_system->NextMaterial( i ) ) {
		IMaterial *material = g_csgo.m_material_system->FindMaterial( i );
		if( !material || material->IsErrorMaterial( ) )
			continue;

		if( std::strstr( material->GetTextureGroupName(), "StaticProp" ) )
			material->AlphaModulate( translucency );
	}

	g_cl.m_should_update_materials = false;
}

void c_misc::capsule_overlay( C_CSPlayer *e, float duration, matrix3x4_t *mat ) {
	if( !e )
		return;

	studiohdr_t *studio_model = e->studio_hdr( );
	if( !studio_model )
		return;

	mstudiohitboxset_t *hitboxset = studio_model->pHitboxSet( e->hitbox_set( ) );
	if( !hitboxset )
		return;

	for( int i = 0; i < hitboxset->numhitboxes; i++ ) {
		mstudiobbox_t *h = hitboxset->pHitbox( i );
		if( !h )
			continue;

		vec3_t min = math::vector_transform( h->bb_min, mat[ h->bone_index ] );
		vec3_t max = math::vector_transform( h->bb_max, mat[ h->bone_index ] );

		if( h->m_flRadius != -1 )
			g_csgo.m_debug_overlay->AddCapsuleOverlayVisible( min, max, h->m_flRadius, 255, 0, 0, 255, duration );
	}
}

void c_misc::no_smoke( ClientFrameStage_t stage ) {
	if( stage != FRAME_RENDER_START || !g_csgo.m_engine->IsConnected( ) )
		return;

	std::vector< const char* > vistasmoke_mats = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	for( auto mat_s : vistasmoke_mats ) {
		IMaterial *mat = g_csgo.m_material_system->GetMaterial( mat_s, TEXTURE_GROUP_OTHER );
		mat->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, g_vars.visuals.misc.remove_smoke );
	}
}
