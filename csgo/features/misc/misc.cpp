#include "misc.h"
#include "../anti-aim/antiaim.h"
#include "../entity listener/ent_listener.h"

c_misc g_misc;

void c_misc::bunnyhop( c_user_cmd *cmd ) {
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

void c_misc::automatic_fire( c_base_combat_weapon *active_weapon, c_user_cmd *cmd ) {
	if( !active_weapon || !cmd )
		return;

	const weapon_info_t *wep_info = active_weapon->get_weapon_info( );

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
	auto local = c_csplayer::get_local( );

	if( !local )
		return;

	if( g_vars.misc.thirdperson_key != 0 )
	{
		static bool is_clicked = false;
		static bool is_down = false;
		
		bool key_pressed = g_input.key_pressed( g_vars.misc.thirdperson_key );
		is_clicked = !key_pressed && is_down;
		is_down = key_pressed;

		if( is_clicked )
			g_vars.misc.thirdperson = !g_vars.misc.thirdperson;
	}
	

	static vec3_t vecAngles;

	g_csgo.m_engine->get_viewangles( vecAngles );

	if( g_vars.misc.thirdperson && local->alive( ) ) {
		if( !g_csgo.m_input->m_camera_in_thirdperson ) {
			auto get_correct_distance = [ & ]( float ideal_distance ) -> float {
				vec3_t inverseAngles;
				g_csgo.m_engine->get_viewangles( inverseAngles );

				inverseAngles.x *= -1.f, inverseAngles.y += 180.f;

				vec3_t direction;
				math::angle_to_vector( inverseAngles, direction );

				ray_t ray;
				trace_t trace;
				c_trace_filter_skip_entity filter( local );

				ray.init( local->eye_pos( ), local->eye_pos( ) + ( direction * ideal_distance ) );

				g_csgo.m_engine_trace->trace_ray( ray, MASK_SHOT, &filter, &trace );

				return ( ideal_distance * trace.m_fraction ) - 10.f;
			};

			vecAngles.z = get_correct_distance( 60.f );

			g_csgo.m_input->m_camera_in_thirdperson = true;

			g_csgo.m_input->m_camera_offset = vec3_t( vecAngles.x, vecAngles.y, vecAngles.z );
		}
	}
	else {
		g_csgo.m_input->m_camera_in_thirdperson = false;

		g_csgo.m_input->m_camera_offset = vec3_t( vecAngles.x, vecAngles.y, 0 );
	}

}

void c_misc::thirdperson( c_view_setup *setup ) {
	auto local = c_csplayer::get_local( );
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

		auto spec = g_csgo.m_entity_list->get< c_csplayer >( spechandle );
		if( !spec )
			return;

		static bool once2 = false;

		if( once2 )
			local->observer_mode( ) = 5;

		if( local->observer_mode( ) == 4 )
			once2 = true;

		static vec3_t angles;
		g_csgo.m_engine->get_viewangles( angles );
		setup->m_angles = angles;

		trace_t tr;
		ray_t ray;

		vec3_t forward, right, up;
		math::angle_to_vectors( angles, &forward, &right, &up );

		vec3_t cam_offset = spec->eye_pos( ) + ( forward * -120.f ) + ( right ) + ( up );

		ray.init( spec->eye_pos( ), cam_offset );
		c_trace_filter_world_only trace_filter;
		trace_filter.m_skip = spec;
		g_csgo.m_engine_trace->trace_ray( ray, MASK_SHOT, &trace_filter, &tr );

		setup->m_origin = tr.m_endpos;
	}
	else if( once ) {
		local->observer_mode( ) = local->alive( ) ? 0 : 4;
		once = false;
	}
}

void c_misc::strafe( c_user_cmd *cmd ) {
	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;

	if( local->flags( ) & FL_ONGROUND && !( cmd->m_buttons & IN_JUMP ) )
		return;

	if( local->get_move_type( ) & ( MOVETYPE_LADDER | MOVETYPE_NOCLIP ) )
		return;

	vec3_t viewangles;
	g_csgo.m_engine->get_viewangles( viewangles );

	static bool side_switch = false;
	side_switch = !side_switch;

	cmd->m_forwardmove = 0.f;
	cmd->m_sidemove = side_switch ? 450.f : -450.f;

	float velocity_yaw = local->velocity( ).y;

	float rotation = util::misc::clamp< float >( math::rad_to_deg( std::atan2( 15.f, local->velocity( ).length_2d( ) ) ), 0.f, 90.f );

	float abs_yaw = std::fabs( math::normalize_angle( velocity_yaw - viewangles.y ) );

	float ideal_yaw_rotation = ( side_switch ? rotation : -rotation ) + ( abs_yaw < 5.f ? velocity_yaw : viewangles.y );

	float cos_rot = std::cos( math::deg_to_rad( viewangles.y - ideal_yaw_rotation ) );
	float sin_rot = std::sin( math::deg_to_rad( viewangles.y - ideal_yaw_rotation ) );

	cmd->m_forwardmove = ( cos_rot * cmd->m_forwardmove ) - ( sin_rot * cmd->m_sidemove );
	cmd->m_sidemove = ( sin_rot * cmd->m_forwardmove ) + ( cos_rot * cmd->m_sidemove );
}

void c_misc::auto_zeus( c_user_cmd *cmd ) {
	auto local = c_csplayer::get_local( );
	if( !local || !local->alive( ) )
		return;
	auto weapon = local->get_active_weapon( );
	if( !weapon )
		return;
	if( weapon->item_index( ) != WEAPON_TASER )
		return;

	for( auto &entry : g_listener.m_players ) {
		auto target = g_csgo.m_entity_list->get< c_csplayer >( entry.m_idx );

		if( !target || target->is_dormant( ) || target->team( ) == local->team( ) || target->immune( ) )
			continue;
		std::vector< int > hitboxes = { pelvis, u_chest, l_chest };
		std::array< matrix3x4_t, 128 > matrix = {};
		std::vector< vec3_t > points;
		vec3_t best_point;

		if( !target->setup_bones( matrix.data( ), 128, 0x100, target->simtime( ) ) )
			continue;

		auto studiohdr = target->model_ptr( )->m_studio_hdr;
		if( !studiohdr )
			continue;

		for( auto &h : hitboxes ) {
			auto *bbox = studiohdr->hitbox( h, 0 );

			vec3_t max, min;
			math::vector_transform( bbox->bb_min, matrix.at( bbox->bone_index ), min );
			math::vector_transform( bbox->bb_max, matrix.at( bbox->bone_index ), max );
			auto center = ( min + max ) / 2.f;

			points.push_back( center );
		}

		auto is_visible = [ & ]( const vec3_t &start, const vec3_t &end, c_csplayer *target ) -> bool {
			ray_t ray;
			trace_t tr;
			ray.init( start, end );

			c_trace_filter_skip_entity filter( g_cl.m_local );

			g_csgo.m_engine_trace->trace_ray( ray, MASK_SHOT, &filter, &tr );

			return ( tr.m_hit_entity == target || tr.m_fraction > 0.97f );
		};

		for( auto &p : points ) {
			if( is_visible( local->eye_pos( ), p, target ) ) {
				best_point = p;
			}
		}

		vec3_t aim_angle = math::calc_angle( local->eye_pos( ), best_point );
		aim_angle.clamp( );

		vec3_t forward;
		math::angle_to_vector( aim_angle, forward );
		forward *= 183;

		vec3_t eye_pos = local->eye_pos( );

		ray_t ray;
		ray.init( eye_pos, eye_pos + forward );

		c_trace_filter filter;
		filter.m_skip = local;

		c_game_trace trace;
		g_csgo.m_engine_trace->trace_ray( ray, MASK_SHOT, &filter, &trace );

		if( !local->can_shoot( weapon ) )
			return;

		if( trace.m_hit_entity == target ) {
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

void c_misc::fix_movement( c_user_cmd *cmd, vec3_t wish_angle ) const {
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

void c_misc::nightmode( c_base_entity *ent, float override_brightness ) {
	if( !g_csgo.m_engine->is_connected( ) || !g_cl.m_local /* || !ent */ )
		return;

	auto client_class = ent->get_client_class( );
	if( !client_class || client_class->m_class_id != CEnvTonemapController )
		return;
	
	float brightness = g_vars.misc.nightmode / 100.f;
	if( !g_vars.misc.nightmode )
		brightness = 0.001f;
	
	if( override_brightness > 0.f )
		brightness = override_brightness / 100.f;
	
	// will not work if post processing is disabled.
	auto tonemap_controller = reinterpret_cast< c_env_tonemap_controller * >( ent );
	
	tonemap_controller->use_custom_exposure_min( ) = 1;
	tonemap_controller->use_custom_exposure_max( ) = 1;
	
	tonemap_controller->custom_exposure_min( ) = brightness;
	tonemap_controller->custom_exposure_max( ) = brightness;
	
	// disable fast path.
	//static cvar *r_drawspecificstaticprop = g_csgo.m_convar->find_var( "r_drawspecificstaticprop" );
	//r_drawspecificstaticprop->set_value( 0 );

	//float brightness = g_vars.misc.nightmode / 100.f;
	//
	//if( override_brightness > 0.f )
	//	brightness = override_brightness / 100.f;
	//
	//for( int i = g_csgo.m_material_system->first_material( ); i != g_csgo.m_material_system->invalid_material( ); i = g_csgo.m_material_system->next_material( i ) ) {
	//	i_material *material = g_csgo.m_material_system->find_material( i );
	//	if( !material || material->is_error_material( ) || std::strstr( material->get_name( ), "flashlight" ) )
	//		continue;
	//
	//	if( std::strstr( material->get_texture_group_name( ), "StaticProp" ) )
	//		material->color_modulate( brightness + 0.05f, brightness + 0.05f, brightness + 0.05f );
	//
	//	if( std::strstr( material->get_texture_group_name( ), "World" ) )
	//		brightness < 0.10f ? material->color_modulate( 0.01f, 0.01f, 0.01f ) : material->color_modulate( brightness, brightness, brightness );
	//
	//	if( std::strstr( material->get_name( ), "glass" ) || std::strstr( material->get_name( ), "decals" ) || std::strstr( material->get_name( ), "door" ) )
	//		brightness < 0.15f ? material->color_modulate( 0.01f, 0.01f, 0.01f ) : material->color_modulate( brightness, brightness, brightness );
	//	
	//}
}

void c_misc::transparent_props( float override_transparency ) {
	if( !g_csgo.m_engine->is_connected( ) || !g_cl.m_local )
		return;

	// disable fast path.
	static cvar *r_drawspecificstaticprop = g_csgo.m_convar->find_var( "r_drawspecificstaticprop" );

	if( g_vars.misc.prop_transparency == 100.f && g_vars.misc.nightmode == 100.f )
		r_drawspecificstaticprop->set_value( -1 );
	else
		r_drawspecificstaticprop->set_value( 0 );

	float translucency = g_vars.misc.prop_transparency / 100.f;

	if( override_transparency > 0.f )
		translucency = override_transparency / 100.f;

	for( int i = g_csgo.m_material_system->first_material( ); i != g_csgo.m_material_system->invalid_material( ); i = g_csgo.m_material_system->next_material( i ) ) {
		i_material *material = g_csgo.m_material_system->find_material( i );
		if( !material || material->is_error_material( ) || std::strstr( material->get_name( ), "flashlight" ) )
			continue;

		if( std::strstr( material->get_texture_group_name( ), "StaticProp" ) )
			material->alpha_modulate( translucency );
	}

	g_cl.m_should_update_materials = false;
}

void c_misc::capsule_overlay( c_csplayer *e, float duration, matrix3x4_t *mat ) {
	if( !e )
		return;

	studiohdr_t *studio_model = e->model_ptr( )->m_studio_hdr;
	if( !studio_model )
		return;

	mstudiohitboxset_t *hitboxset = studio_model->hitbox_set( e->hitbox_set( ) );
	if( !hitboxset )
		return;

	vec3_t hullcolor[ 8 ] = 
	{
		vec3_t( 1.0, 1.0, 1.0 ),
		vec3_t( 1.0, 0.5, 0.5 ),
		vec3_t( 0.5, 1.0, 0.5 ),
		vec3_t( 1.0, 1.0, 0.5 ),
		vec3_t( 0.5, 0.5, 1.0 ),
		vec3_t( 1.0, 0.5, 1.0 ),
		vec3_t( 0.5, 1.0, 1.0 ),
		vec3_t( 1.0, 1.0, 1.0 )
	};

	int r = 0;
	int g = 0;
	int b = 255;

	for( int i = 0; i < hitboxset->numhitboxes; i++ ) {
		mstudiobbox_t *h = hitboxset->pHitbox( i );
		if( !h )
			continue;

		vec3_t min = math::vector_transform( h->bb_min, mat[ h->bone_index ] );
		vec3_t max = math::vector_transform( h->bb_max, mat[ h->bone_index ] );

		int j = ( h->m_iGroup % 8 );

		r = static_cast< int >( 255.0f * hullcolor[ j ][ 0 ] );
		g = static_cast< int >( 255.0f * hullcolor[ j ][ 1 ] );
		b = static_cast< int >( 255.0f * hullcolor[ j ][ 2 ] );

		float radius = h->m_flRadius;

		if( radius != -1.f ){
			g_csgo.m_debug_overlay->add_capsule_overlay_visible( min, max, h->m_flRadius, r, g, b, 150, duration );
		}

	}
}

void c_misc::no_smoke( client_frame_stage_t stage ) {
	if( stage != FRAME_RENDER_START || !g_csgo.m_engine->is_connected( ) )
		return;

	static const std::vector< const char* > vistasmoke_mats = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if ( !g_vars.visuals.misc.remove_smoke ) {
		for ( auto mat_s : vistasmoke_mats ) {
			i_material *mat = g_csgo.m_material_system->get_material( mat_s, TEXTURE_GROUP_OTHER );
			mat->set_material_var_flag( MATERIAL_VAR_NO_DRAW, false );
			mat->set_material_var_flag( MATERIAL_VAR_WIREFRAME, false );
		}
	}
	else if ( g_vars.visuals.misc.remove_smoke == 1 ) {
		for ( auto mat_s : vistasmoke_mats ) {
			i_material *mat = g_csgo.m_material_system->get_material( mat_s, TEXTURE_GROUP_OTHER );
			mat->set_material_var_flag( MATERIAL_VAR_NO_DRAW, true );
			mat->set_material_var_flag( MATERIAL_VAR_WIREFRAME, false );
		}
	}
	else {
		for ( auto mat_s : vistasmoke_mats ) {
			i_material *mat = g_csgo.m_material_system->get_material( mat_s, TEXTURE_GROUP_OTHER );
			mat->set_material_var_flag( MATERIAL_VAR_NO_DRAW, false );
			mat->set_material_var_flag( MATERIAL_VAR_WIREFRAME, true );
		}
	}
}

c_flashlight_effect* c_misc::create_flashlight( int idx, const char* texture_name, float fov, float far_z, float linear_atten ) {
	static auto constructor = pattern::find( g_csgo.m_client_dll, "55 8B EC F3 0F 10 45 ?? B8" );

	// we need to use the engine memory management if we are calling the destructor later
	c_flashlight_effect *flashlight_ptr = reinterpret_cast< c_flashlight_effect* >( g_csgo.m_memalloc->alloc( sizeof( c_flashlight_effect ) ) );

	if ( !flashlight_ptr )
		return nullptr;

	// we need to call this function in a non standard way
	__asm
	{
		movss xmm3, fov
		mov ecx, flashlight_ptr
		push linear_atten
		push far_z
		push texture_name
		push idx
		call constructor
	}

	return flashlight_ptr;
}

void c_misc::flashlight( ) {
	auto destroy_flashlight = [ ]( c_flashlight_effect* flashlight_ptr ) {
		static auto destroy_fn = pattern::find< void( __thiscall* )( c_flashlight_effect*, c_flashlight_effect* ) >( g_csgo.m_client_dll, "56 8B F1 E8 ?? ?? ?? ?? 8B 4E 28" );

		// the flash light destructor handles the memory management, so we dont need to free the allocated memory
		// call the destructor of the flashlight
		destroy_fn( flashlight_ptr, flashlight_ptr );
	};
	auto update_flashlight = [ ]( c_flashlight_effect* flashlight_ptr, const vec3_t &pos, const vec3_t &forward, const vec3_t &right, const vec3_t &up ) {
		// here we tell to the compiler wich calling convention we will use to call the function and the paramaters needed (note the __thiscall*)
		typedef void( __thiscall* update_light_t )( void*, int, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, float, float, float, bool, const char* );

		static update_light_t update_light = NULL;

		if ( !update_light ) {
			// here we have to deal with a call instruction (E8 rel32)
			auto call_instruction = pattern::find( g_csgo.m_client_dll, "E8 ?? ?? ?? ?? 8B 06 F3 0F 10 46" ); // get the instruction address
			uintptr_t rel_address = *reinterpret_cast< uintptr_t* >( call_instruction + 1 ); // read the rel32
			uintptr_t next_instruction = call_instruction + 5; // get the address of next instruction
			update_light = reinterpret_cast< update_light_t >( next_instruction + rel_address ); // our function address will be nextInstruction + relativeAddress
		}

		update_light( flashlight_ptr, flashlight_ptr->m_idx, pos, forward, right, up, flashlight_ptr->m_fov, flashlight_ptr->m_far_z, flashlight_ptr->m_linear_atten, flashlight_ptr->m_casts_shadow, flashlight_ptr->m_texture_name );
	};

	auto local = c_csplayer::get_local( );
	if ( !local || !local->alive( ) )
		return;

	static c_flashlight_effect *flashlight = NULL;
	static float last_tick = 0;

	if ( g_input.key_pressed( g_vars.misc.flashlight_key ) && GetTickCount( ) > last_tick + 250 ) {
		if ( !flashlight ) {
			flashlight = create_flashlight( local->get_index( ), "effects/flashlight001", 35, 5000, 1000 );
		}
		else {
			destroy_flashlight( flashlight );
			flashlight = NULL;
		}

		last_tick = GetTickCount( );
	}

	if ( flashlight ) {
		vec3_t f, r, u;
		vec3_t viewAngles;

		g_csgo.m_engine->get_viewangles( viewAngles );
		math::angle_to_vectors( viewAngles, &f, &r, &u );

		flashlight->m_is_on = true;
		flashlight->m_casts_shadow = false;
		flashlight->m_fov = 35;
		update_flashlight( flashlight, local->eye_pos( ), f, r, u );
	}
}
