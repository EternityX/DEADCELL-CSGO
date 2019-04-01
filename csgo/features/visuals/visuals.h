#pragma once
#include "../../inc.hpp"

class c_visuals {
private:
	struct bbox_t {
		int x, y, w, h;
	};

	struct context_t {
		bool m_enabled;
		int offset{ 0 };
		int flag_count{ 0 };
	} m_ctx;

	void player( C_CSPlayer *entity );
	void draw_healthbar( C_CSPlayer * entity, float x, float y, float w, float h );
	bool calculate_bbox( C_BaseEntity *entity, bbox_t &box ) const;
	void activation_type();
	void run_dormancy_fade( C_CSPlayer *entity, int idx );
	void draw_box( OSHColor color, int index, float x, float y, float w, float h );
	void ammo_bar( C_BaseCombatWeapon *weapon, C_CSPlayer *player, OSHColor color, float x, float y, float w, float h, int clip );
	void flash_bar( C_CSPlayer * player, OSHColor color, float x, float y, float w, float h );
	void weapon_name( C_BaseCombatWeapon *weapon, C_CSPlayer *player, OSHColor color, float x, float y, float w, float h );
	void draw_flags( C_CSPlayer *player, OSHColor color, float x, float y, float w, float h );
	void draw_spectators( ) const;

	void world( C_BaseEntity * entity );
	void draw_scope( ) const;
	void draw_crosshair( ) const;
	static void draw_hitmarker( );
	void draw_bomb_timer( float time_left ) const;

	std::array< float, 64 > m_alpha;
public:
	static bool world_to_screen( const vec3_t &origin, vec3_t &screen );
	void run( );
	void watermark( ) const;
	void handle_glow( );
};

extern c_visuals g_visuals;