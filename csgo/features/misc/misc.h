#pragma once
#include "../../inc.hpp"


class c_misc {
public:
	vec3_t unpredicted_vel;

	void bunnyhop( CUserCmd *cmd );
	static void automatic_fire( C_BaseCombatWeapon *active_weapon, CUserCmd *cmd );
	void thirdperson( CViewSetup *setup );
	static void thirdperson( );
	void strafe( CUserCmd *cmd );
	void auto_zeus( CUserCmd *cmd );
	void fix_movement( CUserCmd *cmd, vec3_t wish_angle ) const;
	void nightmode( C_BaseEntity *ent, float override_brightness = 0.f );
	void transparent_props( float override_transparency = 0.f );
	void capsule_overlay( C_CSPlayer * e, float duration, matrix3x4_t* mat );
	void no_smoke( ClientFrameStage_t stage );
	void slow_walk(CUserCmd* cmd);
};

extern c_misc g_misc;