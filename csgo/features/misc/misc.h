#pragma once
#include "../../inc.h"


class c_misc {
private:
	c_flashlight_effect* create_flashlight( int idx, const char* texture_name, float fov, float far_z, float linear_atten );
public:
	vec3_t unpredicted_vel;

	void bunnyhop( c_user_cmd *cmd );
	static void automatic_fire( c_base_combat_weapon *active_weapon, c_user_cmd *cmd );
	void thirdperson( c_view_setup *setup );
	static void thirdperson( );
	void strafe( c_user_cmd *cmd );
	void auto_zeus( c_user_cmd *cmd );
	void fix_movement( c_user_cmd *cmd, vec3_t wish_angle ) const;
	void nightmode( c_base_entity *ent, float override_brightness = 0.f );
	void transparent_props( float override_transparency = 0.f );
	void capsule_overlay( c_csplayer * e, float duration, matrix3x4_t* mat );
	void no_smoke( client_frame_stage_t stage );
	void flashlight( );
};

extern c_misc g_misc;