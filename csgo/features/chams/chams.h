#pragma once
#include "../../inc.hpp"

class c_chams {
	void push_players( );
	i_material *create_material( bool shade, bool wireframe, bool ignorez, int rimlight_boost = 0 ) const;
	std::vector< std::pair< c_csplayer *, float > > m_players;
	bool m_applied = false;
public:
	void on_sceneend( );
	bool on_dme( IMatRenderContext *ctx, void *state, const model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld );
};

extern c_chams g_chams;