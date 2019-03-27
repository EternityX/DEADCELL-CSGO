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
	void nightmode( int override_brightness = 0 );
	void transparent_props( int override_transparency = 0 );
	void capsule_overlay( C_CSPlayer * e, float duration, std::array<matrix3x4_t, 128> mat );
	void no_smoke( ClientFrameStage_t stage );
	void disable_assert( );
};

extern c_misc g_misc;