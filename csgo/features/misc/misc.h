#pragma once
#include "../../inc.hpp"


class c_misc {

	using RandomFloat_t = float(__cdecl *)( float, float );
	using RandomSeed_t = void(__cdecl *)( int );

	bool m_bWasFrozen;
	vec3_t m_oldangles;
	
public:
	vec3_t m_angle = vec3_t( 0, 0, 0 );
	vec3_t unpredicted_vel;
	void bunnyhop( CUserCmd *cmd );
	void automatic_fire( C_BaseCombatWeapon *active_weapon, CUserCmd *cmd );
	bool halftime_fix(CUserCmd * cmd);
	bool get_spread( vec3_t angles, C_CSPlayer *target, CUserCmd *cmd, int chance );
	void thirdperson( CViewSetup *setup );
	void thirdperson( );
	void strafe( CUserCmd *cmd );
	void auto_zeus( CUserCmd *cmd );
	void fix_movement( CUserCmd *cmd, vec3_t wish_angle ) const;
	void nightmode( int override_brightness = 0 );
	void transparent_props( int override_transparency = 0 );
	void capsule_overlay( C_CSPlayer * e, float duration, std::array<matrix3x4_t, 128> mat );
	void no_smoke( ClientFrameStage_t stage );
};

extern c_misc g_misc;