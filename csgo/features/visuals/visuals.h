#pragma once
#include "../../inc.h"


class c_visuals {
private:
	struct bbox_t {
		int x, y, w, h;
	};

	enum e_vis_pos
	{
		POS_BOT,
		POS_RIGHT,
		POS_TOP,
		POS_LEFT
	};

	struct text_t
	{
		text_t() = default;
		~text_t() = default;

		text_t( std::string text, ImU32 col ) : m_col( col ), m_text( std::move( text ) ), m_pos( e_vis_pos::POS_TOP ) {};
		text_t( std::string text, e_vis_pos pos, ImU32 col ) : m_col( col ), m_text( std::move( text ) ), m_pos( pos ) {};

		ImU32 m_col;
		std::string m_text;
		e_vis_pos m_pos;
	};

	struct bar_t
	{
		bar_t() = default;
		~bar_t() = default;

		bar_t( float progress, ImU32 col ) : m_col( col ), m_progress( progress ), m_number( -1 ), m_pos( e_vis_pos::POS_TOP ) {};
		bar_t( float progress, int num, ImU32 col ) : m_col( col ), m_progress( progress ), m_number( num ), m_pos( e_vis_pos::POS_TOP ) {};
		bar_t( float progress, e_vis_pos pos, ImU32 col ) : m_col( col ), m_progress( progress ), m_number( -1 ), m_pos( pos ) {};
		bar_t( float progress, int num, e_vis_pos pos, ImU32 col ) : m_col( col ), m_progress( progress ), m_number( num ), m_pos( pos ) {};

		ImU32 m_col;
		float m_progress;
		int m_number;
		e_vis_pos m_pos;
	};

	struct vis_data_t
	{
		vis_data_t() = default;
		~vis_data_t() = default;

		void clear() { m_texts.clear(); m_bars.clear(); }

		bool m_valid;
		msg_queue<text_t> m_texts;
		msg_queue<bar_t> m_bars;
		bbox_t m_bbox;
	};

	struct context_t {
		bool m_enabled;
		int offset{ 0 };
		int flag_count{ 0 };
	} m_ctx;

	void player( c_csplayer *entity );
	void draw_healthbar( c_csplayer * entity, float x, float y, float w, float h );
	bool calculate_bbox( c_base_entity *entity, bbox_t &box ) const;
	void activation_type();
	void run_dormancy_fade( c_csplayer *entity, int idx );
	void draw_box( const ImU32 &color, int index, float x, float y, float w, float h );
	void ammo_bar( c_base_combat_weapon *weapon, c_csplayer *player, const ImU32 &color, float x, float y, float w, float h, int clip );
	void flash_bar( c_csplayer * player, const ImU32 &color, float x, float y, float w, float h );
	void weapon_name( c_base_combat_weapon *weapon, c_csplayer *player, const ImU32 &color, float x, float y, float w, float h );
	void draw_flags( c_csplayer *player, const ImU32 &color, float x, float y, float w, float h );
	void draw_spectators( ) const;

	void world( c_base_entity * entity );
	void draw_scope( ) const;
	void draw_crosshair( ) const;
	static void draw_hitmarker( );
	void draw_bomb_timer( float time_left ) const;

	std::array< float, 64 > m_alpha;
public:
	std::array<vis_data_t, 64> m_vis_data;

	static bool world_to_screen( const vec3_t &origin, vec3_t &screen );
	void queue();
	void run( );
	void watermark( ) const;
	void handle_glow( );
};

extern c_visuals g_visuals;