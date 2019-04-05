#pragma once

class c_glow_object_mngr {
private:

public:
	static const int GLOW_FOR_ALL_SPLIT_SCREEN_SLOTS = -1;

	enum glow_style : int {
		GLOW_STYLE_NORMAL = 0,
		GLOW_STYLE_MODEL,
		GLOW_STYLE_INLINE,
		GLOW_STYLE_INLINE_PULSATE,
		GLOW_STYLE_MAX
	};

	class glow_object_definition_t {
	public:
		c_base_entity *m_ent;
		vec3_t m_glow_color;
		float m_glow_alpha;
		PAD( 4 )
		float unk1;
		float m_bloom_amount;
		float m_localplayeriszeropoint3;
		bool m_render_when_occluded;
		bool m_render_when_unoccluded;
		bool m_full_bloom_render;
		PAD( 1 )
		int m_full_bloom_stencil_test_value;
		int m_style;
		int m_split_screen_slot;
		int m_next_free_slot;

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;

		void set( float r, float g, float b, float a, int style ) {
			m_glow_color = vec3_t( r, g, b );
			m_glow_alpha = a;
			m_render_when_occluded = true;
			m_render_when_unoccluded = false;
			m_bloom_amount = 1.0f;
			m_style = style;
		}

		bool should_draw( int slot ) const {
			return get_entity() && ( m_split_screen_slot == GLOW_FOR_ALL_SPLIT_SCREEN_SLOTS || m_split_screen_slot == slot ) && (
				m_render_when_occluded || m_render_when_unoccluded );
		}

		c_base_entity *get_entity() const {
			return m_ent;
		}

		bool is_unused() const {
			return m_next_free_slot != ENTRY_IN_USE;
		}
	};

	glow_object_definition_t *m_glow_object_definitions;
	int m_max_size;
	int m_pad;
	int m_size;
	glow_object_definition_t *m_glow_object_definitions2;
	int m_current_objects;
};
